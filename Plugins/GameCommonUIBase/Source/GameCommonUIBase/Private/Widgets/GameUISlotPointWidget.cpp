// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/GameUISlotPointWidget.h"

#include "Editor/WidgetCompilerLog.h"
#include "Game/GameUILocalPlayer.h"
#include "GameFramework/PlayerState.h"
#include "Misc/UObjectToken.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUISlotPointWidget)

#define LOCTEXT_NAMESPACE "GameUISlot"

/////////////////////////////////////////////////////
// UGameUISlotPointWidget

UGameUISlotPointWidget::UGameUISlotPointWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameUISlotPointWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	ResetExtensionPoint();

	Super::ReleaseSlateResources(bReleaseChildren);
}

TSharedRef<SWidget> UGameUISlotPointWidget::RebuildWidget()
{
	if (!IsDesignTime() && SlotPointTag.IsValid())
	{
		ResetExtensionPoint();
		RegisterExtensionPoint();

		FDelegateHandle Handle = GetOwningLocalPlayer<UGameUILocalPlayer>()->CallAndRegister_OnPlayerStateSet(
			UGameUILocalPlayer::FPlayerStateSetDelegate::FDelegate::CreateUObject(this, &UGameUISlotPointWidget::RegisterSlotPointForPlayerState));
	}

	if (IsDesignTime())
	{
		auto GetExtensionPointText = [this]() { return FText::Format(LOCTEXT("DesignTime_ExtensionPointLabel", "Slot Point\n{0}"), FText::FromName(SlotPointTag.GetTagName())); };

		TSharedRef<SOverlay> MessageBox = SNew(SOverlay);

		MessageBox->AddSlot().Padding(5.0f).HAlign(HAlign_Center).VAlign(VAlign_Center)[SNew(STextBlock).Justification(ETextJustify::Center).Text_Lambda(GetExtensionPointText)];

		return MessageBox;
	}
	else
	{
		return Super::RebuildWidget();
	}
}

void UGameUISlotPointWidget::ResetExtensionPoint()
{
	ResetInternal();

	SlotMapping.Reset();
	for (FGameSlotPointHandle& Handle : ExtensionPointHandles)
	{
		Handle.Unregister();
	}
	ExtensionPointHandles.Reset();
}

void UGameUISlotPointWidget::RegisterExtensionPoint()
{
	if (UGameUISlotSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UGameUISlotSubsystem>())
	{
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add(UUserWidget::StaticClass());
		AllowedDataClasses.Append(DataClasses);

		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPoint(SlotPointTag, ExtensionPointTagMatch, AllowedDataClasses, FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveSlot)));

		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPointForContext(
			SlotPointTag, GetOwningLocalPlayer(), ExtensionPointTagMatch, AllowedDataClasses, FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveSlot)));
	}
}

void UGameUISlotPointWidget::RegisterSlotPointForPlayerState(UGameUILocalPlayer* LocalPlayer, APlayerState* PlayerState)
{
	if (UGameUISlotSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UGameUISlotSubsystem>())
	{
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add(UUserWidget::StaticClass());
		AllowedDataClasses.Append(DataClasses);

		ExtensionPointHandles.Add(
			ExtensionSubsystem->RegisterExtensionPointForContext(SlotPointTag, PlayerState, ExtensionPointTagMatch, AllowedDataClasses, FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveSlot)));
	}
}

void UGameUISlotPointWidget::OnAddOrRemoveSlot(EGameUISlotAction Action, const FGameUISlotRequest& Request)
{
	if (Action == EGameUISlotAction::Added)
	{
		UObject* Data = Request.Data;

		TSubclassOf<UUserWidget> WidgetClass(Cast<UClass>(Data));
		if (WidgetClass)
		{
			UUserWidget* Widget = CreateEntryInternal(WidgetClass);
			SlotMapping.Add(Request.SlotHandle, Widget);
		}
		else if (DataClasses.Num() > 0)
		{
			if (GetWidgetClassForData.IsBound())
			{
				WidgetClass = GetWidgetClassForData.Execute(Data);

				// If the data is irrelevant they can just return no widget class.
				if (WidgetClass)
				{
					if (UUserWidget* Widget = CreateEntryInternal(WidgetClass))
					{
						SlotMapping.Add(Request.SlotHandle, Widget);
						ConfigureWidgetForData.ExecuteIfBound(Widget, Data);
					}
				}
			}
		}
	}
	else
	{
		if (UUserWidget* SlotWidget = SlotMapping.FindRef(Request.SlotHandle))
		{
			RemoveEntryInternal(SlotWidget);
			SlotMapping.Remove(Request.SlotHandle);
		}
	}
}

#if WITH_EDITOR
void UGameUISlotPointWidget::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	// We don't care if the CDO doesn't have a specific tag.
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		if (!SlotPointTag.IsValid())
		{
			TSharedRef<FTokenizedMessage> Message = CompileLog.Error(
				FText::Format(LOCTEXT("UUIExtensionPointWidget_NoTag", "{0} has no SlotPointTag specified - All extension points must specify a tag so they can be located."), FText::FromString(GetName())));
			Message->AddToken(FUObjectToken::Create(this));
		}
	}
}
#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

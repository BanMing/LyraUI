// Copyright Epic Games, Inc. All Rights Reserved.

#include "SubSystem/GameUIMessagingSubsystem.h"

#include "Game/GameUILocalPlayer.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "GameCommonUITags.h"
#include "Panels/Messaging/GameUIGameDialogPanel.h"
#include "NativeGameplayTags.h"
#include "Layout/PrimaryGameLayout.h"
#include "UObject/UObjectHash.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIMessagingSubsystem)

class FSubsystemCollectionBase;
class UClass;

void UGameUIMessagingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ConfirmationDialogClassPtr = ConfirmationDialogClass.LoadSynchronous();
	ErrorDialogClassPtr = ErrorDialogClass.LoadSynchronous();
}

void UGameUIMessagingSubsystem::Deinitialize()
{
	Super::Deinitialize();
	ConfirmationDialogClassPtr = nullptr;
	ErrorDialogClassPtr = nullptr;
}

bool UGameUIMessagingSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<ULocalPlayer>(Outer)->GetGameInstance()->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		// Only create an instance if there is no override implementation defined elsewhere
		return ChildClasses.Num() == 0;
	}

	return false;
}

void UGameUIMessagingSubsystem::ShowConfirmation(UGameUIGameDialogDescriptor* DialogDescriptor, FGameUIMessagingResultDelegate ResultCallback)
{
	if (UGameUILocalPlayer* LocalPlayer = GetLocalPlayer<UGameUILocalPlayer>())
	{
		if (UPrimaryGameLayout* RootLayout = LocalPlayer->GetRootUILayout())
		{
			RootLayout->PushWidgetToLayerStack<UGameUIGameDialogPanel>(
				TAG_UI_LAYER_MODAL, ConfirmationDialogClassPtr, [DialogDescriptor, ResultCallback](UGameUIGameDialogPanel& Dialog) { Dialog.SetupDialog(DialogDescriptor, ResultCallback); });
		}
	}
}

void UGameUIMessagingSubsystem::ShowError(UGameUIGameDialogDescriptor* DialogDescriptor, FGameUIMessagingResultDelegate ResultCallback)
{
	if (UGameUILocalPlayer* LocalPlayer = GetLocalPlayer<UGameUILocalPlayer>())
	{
		if (UPrimaryGameLayout* RootLayout = LocalPlayer->GetRootUILayout())
		{
			RootLayout->PushWidgetToLayerStack<UGameUIGameDialogPanel>(
				TAG_UI_LAYER_MODAL, ErrorDialogClassPtr, [DialogDescriptor, ResultCallback](UGameUIGameDialogPanel& Dialog) { Dialog.SetupDialog(DialogDescriptor, ResultCallback); });
		}
	}
}

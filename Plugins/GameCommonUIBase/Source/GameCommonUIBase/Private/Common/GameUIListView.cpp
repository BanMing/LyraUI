// Copyright Epic Games, Inc. All Rights Reserved.

#include "Common/GameUIListView.h"

#include "Common/GameUIWidgetFactory.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIListView)

#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#endif

#define LOCTEXT_NAMESPACE "GameUIListView"

UGameUIListView::UGameUIListView(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

#if WITH_EDITOR

void UGameUIListView::ValidateCompiledDefaults(IWidgetCompilerLog& InCompileLog) const
{
	Super::ValidateCompiledDefaults(InCompileLog);

	if (FactoryRules.Num() == 0)
	{
		InCompileLog.Error(FText::Format(FText::FromString("{0} has no Factory Rules defined, can't create widgets without them."), FText::FromString(GetName())));
	}
}

#endif

UUserWidget& UGameUIListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSubclassOf<UUserWidget> WidgetClass = DesiredEntryClass;

	for (const UGameUIWidgetFactory* Rule : FactoryRules)
	{
		if (Rule)
		{
			if (const TSubclassOf<UUserWidget> EntryClass = Rule->FindWidgetClassForData(Item))
			{
				WidgetClass = EntryClass;
				break;
			}
		}
	}

	UUserWidget& EntryWidget = GenerateTypedEntry<UUserWidget>(WidgetClass, OwnerTable);

	return EntryWidget;
}

#undef LOCTEXT_NAMESPACE

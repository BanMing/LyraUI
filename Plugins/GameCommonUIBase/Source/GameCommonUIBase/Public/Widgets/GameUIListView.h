// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonListView.h"

#include "GameUIListView.generated.h"

class UUserWidget;
class ULocalPlayer;
class UGameUIWidgetFactory;

UCLASS(meta = (DisableNativeTick))
class GAMECOMMONUIBASE_API UGameUIListView : public UCommonListView
{
	GENERATED_BODY()

public:
	UGameUIListView(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& InCompileLog) const override;
#endif

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	//virtual bool OnIsSelectableOrNavigableInternal(UObject* SelectedItem) override;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category="Entry Creation")
	TArray<TObjectPtr<UGameUIWidgetFactory>> FactoryRules;
};

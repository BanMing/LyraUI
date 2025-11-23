// Fill out your copyright notice in the Description page of Project Settings.
#include "GameUIFunctionLibrary.h"

#include "GameCommonUITags.h"
#include "Subsystem/PrimaryGameLayout.h"

UCommonActivatableWidget* UGameUIFunctionLibrary::PushWidgetToLayerStackForPrimaryPlayer(const UObject* WorldContextObject, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass)
{
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject))
	{
		return RootLayout->PushWidgetToLayerStack<UCommonActivatableWidget>(LayerName, ActivatableWidgetClass.Get());
	}
	return nullptr;
}

void UGameUIFunctionLibrary::FindAndRemoveWidgetFromLayerForPrimaryPlayer(const UObject* WorldContextObject, UCommonActivatableWidget* ActivatableWidget)
{
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject))
	{
		RootLayout->FindAndRemoveWidgetFromLayer(ActivatableWidget);
	}
}

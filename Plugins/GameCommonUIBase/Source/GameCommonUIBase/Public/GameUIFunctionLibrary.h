// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GameUIFunctionLibrary.generated.h"

class UCommonActivatableWidget;
/**
 *
 */
UCLASS()
class GAMECOMMONUIBASE_API UGameUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Game Common UI")
	static UCommonActivatableWidget* PushWidgetToLayerStackForPrimaryPlayer(
		const UObject* WorldContextObject, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> ActivatableWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "Game Common UI")
	static void FindAndRemoveWidgetFromLayerForPrimaryPlayer(const UObject* WorldContextObject, UCommonActivatableWidget* ActivatableWidget);
};

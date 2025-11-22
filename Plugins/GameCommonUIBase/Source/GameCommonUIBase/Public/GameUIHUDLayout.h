// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameUIActivatableWidget.h"

#include "GameUIHUDLayout.generated.h"

class UCommonActivatableWidget;
class UObject;


/**
 * UGameUIHUDLayout
 *
 *	Widget used to lay out the player's HUD (typically specified by an Add Widgets action in the experience)
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "GameUI HUD Layout", Category = "GameUI|HUD"))
class GAMECOMMONUIBASE_API UGameUIHUDLayout : public UGameUIActivatableWidget
{
	GENERATED_BODY()

public:

	UGameUIHUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};

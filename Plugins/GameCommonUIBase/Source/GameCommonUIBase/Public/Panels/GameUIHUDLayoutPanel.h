// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Panels/GameUIActivatablePanel.h"

#include "GameUIHUDLayoutPanel.generated.h"

class UCommonActivatableWidget;
class UObject;

/**
 * UGameUIHUDLayoutPanel
 *
 *	Widget used to lay out the player's HUD (typically specified by an Add Widgets action in the experience)
 */
UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "GameUI HUD Layout", Category = "GameUI|HUD"))
class GAMECOMMONUIBASE_API UGameUIHUDLayoutPanel : public UGameUIActivatablePanel
{
	GENERATED_BODY()

public:
	UGameUIHUDLayoutPanel(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};

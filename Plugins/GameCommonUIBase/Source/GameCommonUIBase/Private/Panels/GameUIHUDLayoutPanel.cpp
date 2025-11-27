// Copyright Epic Games, Inc. All Rights Reserved.

#include "Panels/GameUIHUDLayoutPanel.h"

#include "GameCommonUITags.h"
#include "GameUIFunctionLibrary.h"
#include "Input/CommonUIInputTypes.h"
#include "NativeGameplayTags.h"
#include "Panels/GameUIActivatablePanel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIHUDLayoutPanel)

UGameUIHUDLayoutPanel::UGameUIHUDLayoutPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameUIHUDLayoutPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_ESCAPE), false, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleEscapeAction)));
}

void UGameUIHUDLayoutPanel::HandleEscapeAction()
{
	if (ensure(!EscapeMenuClass.IsNull()))
	{
		UGameUIFunctionLibrary::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_MENU, EscapeMenuClass);
	}
}

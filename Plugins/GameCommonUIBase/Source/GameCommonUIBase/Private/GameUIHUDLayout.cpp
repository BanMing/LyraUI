// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameUIHUDLayout.h"

#include "SubSystem/CommonUIExtensions.h"
#include "Input/CommonUIInputTypes.h"
#include "NativeGameplayTags.h"
#include "GameUIActivatableWidget.h"
#include "GameCommonUITags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIHUDLayout)


UGameUIHUDLayout::UGameUIHUDLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGameUIHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_ESCAPE), false, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleEscapeAction)));
}

void UGameUIHUDLayout::HandleEscapeAction()
{
	if (ensure(!EscapeMenuClass.IsNull()))
	{
		UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_MENU, EscapeMenuClass);
	}
}

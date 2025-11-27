// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widgets/Tab/GameUITabButtonBase.h"

#include "CommonLazyImage.h"
#include "Widgets/Tab/GameUITabListWidgetBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUITabButtonBase)

class UObject;
struct FSlateBrush;

void UGameUITabButtonBase::SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrushFromLazyDisplayAsset(LazyObject);
	}
}

void UGameUITabButtonBase::SetIconBrush(const FSlateBrush& Brush)
{
	if (LazyImage_Icon)
	{
		LazyImage_Icon->SetBrush(Brush);
	}
}

void UGameUITabButtonBase::SetTabLabelInfo_Implementation(const FGameUITabDescriptor& TabLabelInfo)
{
	SetButtonText(TabLabelInfo.TabText);
	SetIconBrush(TabLabelInfo.IconBrush);
}

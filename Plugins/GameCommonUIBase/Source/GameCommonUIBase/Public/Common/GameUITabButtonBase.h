// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameUITabListWidgetBase.h"
#include "Common/GameUIButtonBase.h"

#include "GameUITabButtonBase.generated.h"

class UCommonLazyImage;
class UObject;
struct FFrame;
struct FSlateBrush;

UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick))
class GAMECOMMONUIBASE_API UGameUITabButtonBase : public UGameUIButtonBase, public IGameUITabButtonInterface
{
	GENERATED_BODY()

public:

	void SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject);
	void SetIconBrush(const FSlateBrush& Brush);

protected:

	UFUNCTION()
	virtual void SetTabLabelInfo_Implementation(const FGameUITabDescriptor& TabLabelInfo) override;

private:

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> LazyImage_Icon;
};

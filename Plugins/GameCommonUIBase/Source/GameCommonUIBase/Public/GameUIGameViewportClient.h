// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonGameViewportClient.h"

#include "GameUIGameViewportClient.generated.h"

class UGameInstance;
class UObject;

UCLASS(BlueprintType)
class GAMECOMMONUIBASE_API UGameUIGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	UGameUIGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};

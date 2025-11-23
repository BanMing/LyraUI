// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "GameUIPlayerController.generated.h"

/**
 *
 */
UCLASS()
class GAMECOMMONUIBASE_API AGameUIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void ReceivedPlayer() override;
};

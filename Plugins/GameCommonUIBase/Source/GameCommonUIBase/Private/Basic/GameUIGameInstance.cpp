// Fill out your copyright notice in the Description page of Project Settings.

#include "Basic/GameUIGameInstance.h"

#include "Basic/GameUILocalPlayer.h"
#include "LogGameCommonUIBase.h"
#include "Subsystem/GameUIManagerSubsystem.h"

int32 UGameUIGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
	if (ReturnVal != INDEX_NONE)
	{
		if (!PrimaryPlayer.IsValid())
		{
			UE_LOG(LogGameCommonUIBase, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"), *NewPlayer->GetName());
			PrimaryPlayer = NewPlayer;
		}

		GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UGameUILocalPlayer>(NewPlayer));
	}

	return ReturnVal;
}

bool UGameUIGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if (PrimaryPlayer == ExistingPlayer)
	{
		// TODO: do we want to fall back to another player?
		PrimaryPlayer.Reset();
		UE_LOG(LogGameCommonUIBase, Log, TEXT("RemoveLocalPlayer: Unsetting Primary Player from %s"), *ExistingPlayer->GetName());
	}
	GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<UGameUILocalPlayer>(ExistingPlayer));

	return Super::RemoveLocalPlayer(ExistingPlayer);
}

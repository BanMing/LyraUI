// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/GameUIPlayerController.h"

#include "Game/GameUILocalPlayer.h"

void AGameUIPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (UGameUILocalPlayer* LocalPlayer = Cast<UGameUILocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);
	}
}

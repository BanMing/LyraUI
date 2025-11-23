// Fill out your copyright notice in the Description page of Project Settings.

#include "Basic/GameUIPlayerController.h"

#include "Basic/GameUILocalPlayer.h"

void AGameUIPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (UGameUILocalPlayer* LocalPlayer = Cast<UGameUILocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/SoftObjectPtr.h"
#include "Containers/Ticker.h"

#include "GameUIManagerSubsystem.generated.h"

class FSubsystemCollectionBase;
class UGameUILocalPlayer;
class UGameUIPolicy;
class UObject;

UCLASS(config = GameCommonUI)
class GAMECOMMONUIBASE_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGameUIManagerSubsystem() {}

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	const UGameUIPolicy* GetCurrentUIPolicy() const { return CurrentPolicy; }
	UGameUIPolicy* GetCurrentUIPolicy() { return CurrentPolicy; }

	virtual void NotifyPlayerAdded(UGameUILocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(UGameUILocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(UGameUILocalPlayer* LocalPlayer);

protected:
	void SwitchToPolicy(UGameUIPolicy* InPolicy);

private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();

private:
	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;

	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;

	FTSTicker::FDelegateHandle TickHandle;
};

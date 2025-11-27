// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/World.h"

#include "GameUIPolicy.generated.h"

class UGameUILocalPlayer;
class UGameUIManagerSubsystem;
class ULocalPlayer;
class UPrimaryGameLayout;

/**
 * 
 */
UENUM()
enum class ELocalMultiplayerInteractionMode : uint8
{
	// Fullscreen viewport for the primary player only, regardless of the other player's existence
	PrimaryOnly,

	// Fullscreen viewport for one player, but players can swap control over who's is displayed and who's is dormant
	SingleToggle,

	// Viewports displayed simultaneously for both players
	Simultaneous
};

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPrimaryGameLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;

	FRootViewportLayoutInfo() {}
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{}

	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }
};

UCLASS(Abstract, Blueprintable, Within = GameUIManagerSubsystem)
class GAMECOMMONUIBASE_API UGameUIPolicy : public UObject
{
	GENERATED_BODY()

public:
	template <typename GameUIPolicyClass = UGameUIPolicy>
	static GameUIPolicyClass* GetGameUIPolicyAs(const UObject* WorldContextObject)
	{
		return Cast<GameUIPolicyClass>(GetGameUIPolicy(WorldContextObject));
	}

	static UGameUIPolicy* GetGameUIPolicy(const UObject* WorldContextObject);

public:
	virtual UWorld* GetWorld() const override;
	UGameUIManagerSubsystem* GetOwningUIManager() const;
	UPrimaryGameLayout* GetRootLayout(const UGameUILocalPlayer* LocalPlayer) const;

	ELocalMultiplayerInteractionMode GetLocalMultiplayerInteractionMode() const { return LocalMultiplayerInteractionMode; }

	void RequestPrimaryControl(UPrimaryGameLayout* Layout);

protected:
	void AddLayoutToViewport(UGameUILocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(UGameUILocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	virtual void OnRootLayoutAddedToViewport(UGameUILocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	virtual void OnRootLayoutRemovedFromViewport(UGameUILocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	virtual void OnRootLayoutReleased(UGameUILocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	void CreateLayoutWidget(UGameUILocalPlayer* LocalPlayer);
	TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass(UGameUILocalPlayer* LocalPlayer);

private:
	ELocalMultiplayerInteractionMode LocalMultiplayerInteractionMode = ELocalMultiplayerInteractionMode::PrimaryOnly;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> LayoutClass;

	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;

private:
	void NotifyPlayerAdded(UGameUILocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UGameUILocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UGameUILocalPlayer* LocalPlayer);

	friend class UGameUIManagerSubsystem;
};

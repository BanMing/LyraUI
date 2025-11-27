// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Subsystems/LocalPlayerSubsystem.h"

#include "GameUIMessagingSubsystem.generated.h"

class UGameUIGameDialogPanel;
class FSubsystemCollectionBase;
class UGameUIGameDialogDescriptor;
class UObject;

/** Possible results from a dialog */
UENUM(BlueprintType)
enum class EGameUIMessagingResult : uint8
{
	/** The "yes" button was pressed */
	Confirmed,
	/** The "no" button was pressed */
	Declined,
	/** The "ignore/cancel" button was pressed */
	Cancelled,
	/** The dialog was explicitly killed (no user input) */
	Killed,
	Unknown UMETA(Hidden)
};

DECLARE_DELEGATE_OneParam(FGameUIMessagingResultDelegate, EGameUIMessagingResult /* Result */);

/**
 *
 */
UCLASS(config = GameCommonUI)
class GAMECOMMONUIBASE_API UGameUIMessagingSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UGameUIMessagingSubsystem()
	{
	}

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void ShowConfirmation(UGameUIGameDialogDescriptor* DialogDescriptor, FGameUIMessagingResultDelegate ResultCallback = FGameUIMessagingResultDelegate());
	virtual void ShowError(UGameUIGameDialogDescriptor* DialogDescriptor, FGameUIMessagingResultDelegate ResultCallback = FGameUIMessagingResultDelegate());

private:
	UPROPERTY()
	TSubclassOf<UGameUIGameDialogPanel> ConfirmationDialogClassPtr;

	UPROPERTY()
	TSubclassOf<UGameUIGameDialogPanel> ErrorDialogClassPtr;

	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<UGameUIGameDialogPanel> ConfirmationDialogClass;

	UPROPERTY(config, EditAnywhere)
	TSoftClassPtr<UGameUIGameDialogPanel> ErrorDialogClass;
};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonActivatableWidget.h"
#include "GameUIMessagingSubsystem.h"

#include "GameUIGameDialog.generated.h"

USTRUCT(BlueprintType)
struct FConfirmationDialogAction
{
	GENERATED_BODY()

public:
	/** Required: The dialog option to provide. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameUIMessagingResult Result = EGameUIMessagingResult::Unknown;

	/** Optional: Display Text to use instead of the action name associated with the result. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText OptionalDisplayText;

	bool operator==(const FConfirmationDialogAction& Other) const
	{
		return Result == Other.Result &&
			OptionalDisplayText.EqualTo(Other.OptionalDisplayText);
	}
};

UCLASS()
class GAMECOMMONUIBASE_API UGameUIGameDialogDescriptor : public UObject
{
	GENERATED_BODY()
	
public:
	static UGameUIGameDialogDescriptor* CreateConfirmationOk(const FText& Header, const FText& Body);
	static UGameUIGameDialogDescriptor* CreateConfirmationOkCancel(const FText& Header, const FText& Body);
	static UGameUIGameDialogDescriptor* CreateConfirmationYesNo(const FText& Header, const FText& Body);
	static UGameUIGameDialogDescriptor* CreateConfirmationYesNoCancel(const FText& Header, const FText& Body);

public:
	/** The header of the message to display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Header;
	
	/** The body of the message to display */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Body;

	/** The confirm button's input action to use. */
	UPROPERTY(BlueprintReadWrite)
	TArray<FConfirmationDialogAction> ButtonActions;
};


UCLASS(Abstract)
class GAMECOMMONUIBASE_API UGameUIGameDialog : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UGameUIGameDialog();
	
	virtual void SetupDialog(UGameUIGameDialogDescriptor* Descriptor, FGameUIMessagingResultDelegate ResultCallback);

	virtual void KillDialog();
};

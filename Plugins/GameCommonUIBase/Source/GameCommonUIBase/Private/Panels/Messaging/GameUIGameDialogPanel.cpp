// Copyright Epic Games, Inc. All Rights Reserved.

#include "Panels/Messaging/GameUIGameDialogPanel.h"

#include "SubSystem/GameUIMessagingSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIGameDialogPanel)

#define LOCTEXT_NAMESPACE "Messaging"

UGameUIGameDialogDescriptor* UGameUIGameDialogDescriptor::CreateConfirmationOk(const FText& Header, const FText& Body)
{
	UGameUIGameDialogDescriptor* Descriptor = NewObject<UGameUIGameDialogDescriptor>();
	Descriptor->Header = Header;
	Descriptor->Body = Body;

	FConfirmationDialogAction ConfirmAction;
	ConfirmAction.Result = EGameUIMessagingResult::Confirmed;
	ConfirmAction.OptionalDisplayText = LOCTEXT("Ok", "Ok");

	Descriptor->ButtonActions.Add(ConfirmAction);

	return Descriptor;
}

UGameUIGameDialogDescriptor* UGameUIGameDialogDescriptor::CreateConfirmationOkCancel(const FText& Header, const FText& Body)
{
	UGameUIGameDialogDescriptor* Descriptor = NewObject<UGameUIGameDialogDescriptor>();
	Descriptor->Header = Header;
	Descriptor->Body = Body;

	FConfirmationDialogAction ConfirmAction;
	ConfirmAction.Result = EGameUIMessagingResult::Confirmed;
	ConfirmAction.OptionalDisplayText = LOCTEXT("Ok", "Ok");

	FConfirmationDialogAction CancelAction;
	CancelAction.Result = EGameUIMessagingResult::Cancelled;
	CancelAction.OptionalDisplayText = LOCTEXT("Cancel", "Cancel");

	Descriptor->ButtonActions.Add(ConfirmAction);
	Descriptor->ButtonActions.Add(CancelAction);

	return Descriptor;
}

UGameUIGameDialogDescriptor* UGameUIGameDialogDescriptor::CreateConfirmationYesNo(const FText& Header, const FText& Body)
{
	UGameUIGameDialogDescriptor* Descriptor = NewObject<UGameUIGameDialogDescriptor>();
	Descriptor->Header = Header;
	Descriptor->Body = Body;

	FConfirmationDialogAction ConfirmAction;
	ConfirmAction.Result = EGameUIMessagingResult::Confirmed;
	ConfirmAction.OptionalDisplayText = LOCTEXT("Yes", "Yes");

	FConfirmationDialogAction DeclineAction;
	DeclineAction.Result = EGameUIMessagingResult::Declined;
	DeclineAction.OptionalDisplayText = LOCTEXT("No", "No");

	Descriptor->ButtonActions.Add(ConfirmAction);
	Descriptor->ButtonActions.Add(DeclineAction);

	return Descriptor;
}

UGameUIGameDialogDescriptor* UGameUIGameDialogDescriptor::CreateConfirmationYesNoCancel(const FText& Header, const FText& Body)
{
	UGameUIGameDialogDescriptor* Descriptor = NewObject<UGameUIGameDialogDescriptor>();
	Descriptor->Header = Header;
	Descriptor->Body = Body;

	FConfirmationDialogAction ConfirmAction;
	ConfirmAction.Result = EGameUIMessagingResult::Confirmed;
	ConfirmAction.OptionalDisplayText = LOCTEXT("Yes", "Yes");

	FConfirmationDialogAction DeclineAction;
	DeclineAction.Result = EGameUIMessagingResult::Declined;
	DeclineAction.OptionalDisplayText = LOCTEXT("No", "No");

	FConfirmationDialogAction CancelAction;
	CancelAction.Result = EGameUIMessagingResult::Cancelled;
	CancelAction.OptionalDisplayText = LOCTEXT("Cancel", "Cancel");

	Descriptor->ButtonActions.Add(ConfirmAction);
	Descriptor->ButtonActions.Add(DeclineAction);
	Descriptor->ButtonActions.Add(CancelAction);

	return Descriptor;
}

UGameUIGameDialogPanel::UGameUIGameDialogPanel()
{

}

void UGameUIGameDialogPanel::SetupDialog(UGameUIGameDialogDescriptor* Descriptor, FGameUIMessagingResultDelegate ResultCallback)
{

}

void UGameUIGameDialogPanel::KillDialog()
{

}

#undef LOCTEXT_NAMESPACE

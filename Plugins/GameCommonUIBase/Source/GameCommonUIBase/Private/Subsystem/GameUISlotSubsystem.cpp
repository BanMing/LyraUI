// Copyright Epic Games, Inc. All Rights Reserved.

#include "SubSystem/GameUISlotSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "LogGameCommonUIBase.h"
#include "UObject/Stack.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUISlotSubsystem)

class FSubsystemCollectionBase;

//=========================================================

void FGameSlotPointHandle::Unregister()
{
	if (UGameUISlotSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterSlotPoint(*this);
	}
}

//=========================================================

void FGameUISlotHandle::Unregister()
{
	if (UGameUISlotSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSourcePtr->UnregisterSlot(*this);
	}
}

//=========================================================

bool FGameUISlotPoint::DoesExtensionPassContract(const FGameUISlot* Slot) const
{
	if (UObject* DataPtr = Slot->Data)
	{
		const bool bMatchesContext = (ContextObject.IsExplicitlyNull() && Slot->ContextObject.IsExplicitlyNull()) || ContextObject == Slot->ContextObject;

		// Make sure the contexts match.
		if (bMatchesContext)
		{
			// The data can either be the literal class of the data type, or a instance of the class type.
			const UClass* DataClass = DataPtr->IsA(UClass::StaticClass()) ? Cast<UClass>(DataPtr) : DataPtr->GetClass();
			for (const UClass* AllowedDataClass : AllowedDataClasses)
			{
				if (DataClass->IsChildOf(AllowedDataClass) || DataClass->ImplementsInterface(AllowedDataClass))
				{
					return true;
				}
			}
		}
	}

	return false;
}

//=========================================================

void UGameUISlotSubsystem::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
	if (UGameUISlotSubsystem* ExtensionSubsystem = Cast<UGameUISlotSubsystem>(InThis))
	{
		for (auto MapIt = ExtensionSubsystem->ExtensionPointMap.CreateIterator(); MapIt; ++MapIt)
		{
			for (const TSharedPtr<FGameUISlotPoint>& ValueElement : MapIt.Value())
			{
				Collector.AddReferencedObjects(ValueElement->AllowedDataClasses);
			}
		}

		for (auto MapIt = ExtensionSubsystem->SlotMap.CreateIterator(); MapIt; ++MapIt)
		{
			for (const TSharedPtr<FGameUISlot>& ValueElement : MapIt.Value())
			{
				Collector.AddReferencedObject(ValueElement->Data);
			}
		}
	}
}

void UGameUISlotSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGameUISlotSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FGameSlotPointHandle UGameUISlotSubsystem::RegisterExtensionPoint(
	const FGameplayTag& SlotPointTag, EGameUISlotPointMatch SlotPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback)
{
	return RegisterExtensionPointForContext(SlotPointTag, nullptr, SlotPointTagMatchType, AllowedDataClasses, ExtensionCallback);
}

FGameSlotPointHandle UGameUISlotSubsystem::RegisterExtensionPointForContext(
	const FGameplayTag& SlotPointTag, UObject* ContextObject, EGameUISlotPointMatch SlotPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback)
{
	if (!SlotPointTag.IsValid())
	{
		UE_LOG(LogGameCommonUIBase, Warning, TEXT("Trying to register an invalid extension point."));
		return FGameSlotPointHandle();
	}

	if (!ExtensionCallback.IsBound())
	{
		UE_LOG(LogGameCommonUIBase, Warning, TEXT("Trying to register an invalid extension point."));
		return FGameSlotPointHandle();
	}

	if (AllowedDataClasses.Num() == 0)
	{
		UE_LOG(LogGameCommonUIBase, Warning, TEXT("Trying to register an invalid extension point."));
		return FGameSlotPointHandle();
	}

	FExtensionPointList& List = ExtensionPointMap.FindOrAdd(SlotPointTag);

	TSharedPtr<FGameUISlotPoint>& Entry = List.Add_GetRef(MakeShared<FGameUISlotPoint>());
	Entry->SlotPointTag = SlotPointTag;
	Entry->ContextObject = ContextObject;
	Entry->SlotPointTagMatchType = SlotPointTagMatchType;
	Entry->AllowedDataClasses = AllowedDataClasses;
	Entry->Callback = MoveTemp(ExtensionCallback);

	UE_LOG(LogGameCommonUIBase, Verbose, TEXT("Slot Point [%s] Registered"), *SlotPointTag.ToString());

	NotifySlotPointOfSlots(Entry);

	return FGameSlotPointHandle(this, Entry);
}

FGameUISlotHandle UGameUISlotSubsystem::RegisterSlotAsWidget(const FGameplayTag& SlotPointTag, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterSlotAsData(SlotPointTag, nullptr, WidgetClass, Priority);
}

FGameUISlotHandle UGameUISlotSubsystem::RegisterSlotAsWidgetForContext(const FGameplayTag& SlotPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterSlotAsData(SlotPointTag, ContextObject, WidgetClass, Priority);
}

FGameUISlotHandle UGameUISlotSubsystem::RegisterSlotAsData(const FGameplayTag& SlotPointTag, UObject* ContextObject, UObject* Data, int32 Priority)
{
	if (!SlotPointTag.IsValid())
	{
		UE_LOG(LogGameCommonUIBase, Warning, TEXT("Trying to register an invalid extension."));
		return FGameUISlotHandle();
	}

	if (!Data)
	{
		UE_LOG(LogGameCommonUIBase, Warning, TEXT("Trying to register an invalid extension."));
		return FGameUISlotHandle();
	}

	FExtensionList& List = SlotMap.FindOrAdd(SlotPointTag);

	TSharedPtr<FGameUISlot>& Entry = List.Add_GetRef(MakeShared<FGameUISlot>());
	Entry->SlotPointTag = SlotPointTag;
	Entry->ContextObject = ContextObject;
	Entry->Data = Data;
	Entry->Priority = Priority;

	if (ContextObject)
	{
		UE_LOG(LogGameCommonUIBase, Verbose, TEXT("Slot [%s] @ [%s] Registered"), *GetNameSafe(Data), *SlotPointTag.ToString());
	}
	else
	{
		UE_LOG(LogGameCommonUIBase, Verbose, TEXT("Slot [%s] for [%s] @ [%s] Registered"), *GetNameSafe(Data), *GetNameSafe(ContextObject), *SlotPointTag.ToString());
	}

	NotifySlotPointsOfSlot(EGameUISlotAction::Added, Entry);

	return FGameUISlotHandle(this, Entry);
}

void UGameUISlotSubsystem::NotifySlotPointOfSlots(TSharedPtr<FGameUISlotPoint>& SlotPoint)
{
	for (FGameplayTag Tag = SlotPoint->SlotPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FExtensionList* ListPtr = SlotMap.Find(Tag))
		{
			// Copy in case there are removals while handling callbacks
			FExtensionList SlotArray(*ListPtr);

			for (const TSharedPtr<FGameUISlot>& Slot : SlotArray)
			{
				if (SlotPoint->DoesExtensionPassContract(Slot.Get()))
				{
					FGameUISlotRequest Request = CreateSlotRequest(Slot);
					SlotPoint->Callback.ExecuteIfBound(EGameUISlotAction::Added, Request);
				}
			}
		}

		if (SlotPoint->SlotPointTagMatchType == EGameUISlotPointMatch::ExactMatch)
		{
			break;
		}
	}
}

void UGameUISlotSubsystem::NotifySlotPointsOfSlot(EGameUISlotAction Action, TSharedPtr<FGameUISlot>& Slot)
{
	bool bOnInitialTag = true;
	for (FGameplayTag Tag = Slot->SlotPointTag; Tag.IsValid(); Tag = Tag.RequestDirectParent())
	{
		if (const FExtensionPointList* ListPtr = ExtensionPointMap.Find(Tag))
		{
			// Copy in case there are removals while handling callbacks
			FExtensionPointList ExtensionPointArray(*ListPtr);

			for (const TSharedPtr<FGameUISlotPoint>& SlotPoint : ExtensionPointArray)
			{
				if (bOnInitialTag || (SlotPoint->SlotPointTagMatchType == EGameUISlotPointMatch::PartialMatch))
				{
					if (SlotPoint->DoesExtensionPassContract(Slot.Get()))
					{
						FGameUISlotRequest Request = CreateSlotRequest(Slot);
						SlotPoint->Callback.ExecuteIfBound(Action, Request);
					}
				}
			}
		}

		bOnInitialTag = false;
	}
}

void UGameUISlotSubsystem::UnregisterSlot(const FGameUISlotHandle& SlotHandle)
{
	if (SlotHandle.IsValid())
	{
		checkf(SlotHandle.ExtensionSource == this, TEXT("Trying to unregister an extension that's not from this extension subsystem."));

		TSharedPtr<FGameUISlot> Slot = SlotHandle.DataPtr;
		if (FExtensionList* ListPtr = SlotMap.Find(Slot->SlotPointTag))
		{
			if (Slot->ContextObject.IsExplicitlyNull())
			{
				UE_LOG(LogGameCommonUIBase, Verbose, TEXT("Slot [%s] @ [%s] Unregistered"), *GetNameSafe(Slot->Data), *Slot->SlotPointTag.ToString());
			}
			else
			{
				UE_LOG(LogGameCommonUIBase, Verbose, TEXT("Slot [%s] for [%s] @ [%s] Unregistered"), *GetNameSafe(Slot->Data), *GetNameSafe(Slot->ContextObject.Get()), *Slot->SlotPointTag.ToString());
			}

			NotifySlotPointsOfSlot(EGameUISlotAction::Removed, Slot);

			ListPtr->RemoveSwap(Slot);

			if (ListPtr->Num() == 0)
			{
				SlotMap.Remove(Slot->SlotPointTag);
			}
		}
	}
	else
	{
		UE_LOG(LogGameCommonUIBase, Warning, TEXT("Trying to unregister an invalid Handle."));
	}
}

void UGameUISlotSubsystem::UnregisterSlotPoint(const FGameSlotPointHandle& ExtensionPointHandle)
{
	if (ExtensionPointHandle.IsValid())
	{
		check(ExtensionPointHandle.ExtensionSource == this);

		const TSharedPtr<FGameUISlotPoint> SlotPoint = ExtensionPointHandle.DataPtr;
		if (FExtensionPointList* ListPtr = ExtensionPointMap.Find(SlotPoint->SlotPointTag))
		{
			UE_LOG(LogGameCommonUIBase, Verbose, TEXT("Slot Point [%s] Unregistered"), *SlotPoint->SlotPointTag.ToString());

			ListPtr->RemoveSwap(SlotPoint);
			if (ListPtr->Num() == 0)
			{
				ExtensionPointMap.Remove(SlotPoint->SlotPointTag);
			}
		}
	}
	else
	{
		UE_LOG(LogGameCommonUIBase, Warning, TEXT("Trying to unregister an invalid Handle."));
	}
}

FGameUISlotRequest UGameUISlotSubsystem::CreateSlotRequest(const TSharedPtr<FGameUISlot>& Slot)
{
	FGameUISlotRequest Request;
	Request.SlotHandle = FGameUISlotHandle(this, Slot);
	Request.SlotPointTag = Slot->SlotPointTag;
	Request.Priority = Slot->Priority;
	Request.Data = Slot->Data;
	Request.ContextObject = Slot->ContextObject.Get();

	return Request;
}

FGameSlotPointHandle UGameUISlotSubsystem::K2_RegisterSlotPoint(
	FGameplayTag SlotPointTag, EGameUISlotPointMatch SlotPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDynamicDelegate ExtensionCallback)
{
	return RegisterExtensionPoint(SlotPointTag, SlotPointTagMatchType, AllowedDataClasses,
		FExtendExtensionPointDelegate::CreateWeakLambda(
			ExtensionCallback.GetUObject(), [this, ExtensionCallback](EGameUISlotAction Action, const FGameUISlotRequest& Request) { ExtensionCallback.ExecuteIfBound(Action, Request); }));
}

FGameUISlotHandle UGameUISlotSubsystem::K2_RegisterSlotAsWidget(FGameplayTag SlotPointTag, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterSlotAsWidget(SlotPointTag, WidgetClass, Priority);
}

FGameUISlotHandle UGameUISlotSubsystem::K2_RegisterExtensionAsWidgetForContext(FGameplayTag SlotPointTag, TSubclassOf<UUserWidget> WidgetClass, UObject* ContextObject, int32 Priority)
{
	if (ContextObject)
	{
		return RegisterSlotAsWidgetForContext(SlotPointTag, ContextObject, WidgetClass, Priority);
	}
	else
	{
		FFrame::KismetExecutionMessage(TEXT("A null ContextObject was passed to Register Slot (Widget For Context)"), ELogVerbosity::Error);
		return FGameUISlotHandle();
	}
}

FGameUISlotHandle UGameUISlotSubsystem::K2_RegisterSlotAsData(FGameplayTag SlotPointTag, UObject* Data, int32 Priority)
{
	return RegisterSlotAsData(SlotPointTag, nullptr, Data, Priority);
}

FGameUISlotHandle UGameUISlotSubsystem::K2_RegisterSlotAsDataForContext(FGameplayTag SlotPointTag, UObject* ContextObject, UObject* Data, int32 Priority)
{
	if (ContextObject)
	{
		return RegisterSlotAsData(SlotPointTag, ContextObject, Data, Priority);
	}
	else
	{
		FFrame::KismetExecutionMessage(TEXT("A null ContextObject was passed to Register Slot (Data For Context)"), ELogVerbosity::Error);
		return FGameUISlotHandle();
	}
}

//=========================================================

void UGameUISlotHandleFunctions::Unregister(FGameUISlotHandle& Handle)
{
	Handle.Unregister();
}

bool UGameUISlotHandleFunctions::IsValid(FGameUISlotHandle& Handle)
{
	return Handle.IsValid();
}

//=========================================================

void UGameSlotPointHandleFunctions::Unregister(FGameSlotPointHandle& Handle)
{
	Handle.Unregister();
}

bool UGameSlotPointHandleFunctions::IsValid(FGameSlotPointHandle& Handle)
{
	return Handle.IsValid();
}

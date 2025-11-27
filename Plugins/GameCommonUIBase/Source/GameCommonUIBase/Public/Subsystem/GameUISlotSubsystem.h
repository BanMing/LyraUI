// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystems/WorldSubsystem.h"

#include "GameUISlotSubsystem.generated.h"

class UGameUISlotSubsystem;
struct FGameUISlotRequest;
template <typename T>
class TSubclassOf;

class FSubsystemCollectionBase;
class UUserWidget;
struct FFrame;

// Match rule for extension points
UENUM(BlueprintType)
enum class EGameUISlotPointMatch : uint8
{
	// An exact match will only receive extensions with exactly the same point
	// (e.g., registering for "A.B" will match a broadcast of A.B but not A.B.C)
	ExactMatch,

	// A partial match will receive any extensions rooted in the same point
	// (e.g., registering for "A.B" will match a broadcast of A.B as well as A.B.C)
	PartialMatch
};

// Match rule for extension points
UENUM(BlueprintType)
enum class EGameUISlotAction : uint8
{
	Added,
	Removed
};

DECLARE_DELEGATE_TwoParams(FExtendExtensionPointDelegate, EGameUISlotAction Action, const FGameUISlotRequest& Request);

/*
 *
 */
struct FGameUISlot : TSharedFromThis<FGameUISlot>
{
public:
	/** The extension point this extension is intended for. */
	FGameplayTag SlotPointTag;
	int32 Priority = INDEX_NONE;
	TWeakObjectPtr<UObject> ContextObject;
	// Kept alive by UGameUISlotSubsystem::AddReferencedObjects
	TObjectPtr<UObject> Data = nullptr;
};

/**
 *
 */
struct FGameUISlotPoint : TSharedFromThis<FGameUISlotPoint>
{
public:
	FGameplayTag SlotPointTag;
	TWeakObjectPtr<UObject> ContextObject;
	EGameUISlotPointMatch SlotPointTagMatchType = EGameUISlotPointMatch::ExactMatch;
	TArray<TObjectPtr<UClass>> AllowedDataClasses;
	FExtendExtensionPointDelegate Callback;

	// Tests if the extension and the extension point match up, if they do then this extension point should learn
	// about this extension.
	bool DoesExtensionPassContract(const FGameUISlot* Slot) const;
};

/**
 *
 */
USTRUCT(BlueprintType)
struct GAMECOMMONUIBASE_API FGameSlotPointHandle
{
	GENERATED_BODY()

public:
	FGameSlotPointHandle()
	{
	}

	void Unregister();

	bool IsValid() const
	{
		return DataPtr.IsValid();
	}

	bool operator==(const FGameSlotPointHandle& Other) const
	{
		return DataPtr == Other.DataPtr;
	}
	bool operator!=(const FGameSlotPointHandle& Other) const
	{
		return !operator==(Other);
	}

	friend uint32 GetTypeHash(const FGameSlotPointHandle& Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}

private:
	TWeakObjectPtr<UGameUISlotSubsystem> ExtensionSource;

	TSharedPtr<FGameUISlotPoint> DataPtr;

	friend UGameUISlotSubsystem;

	FGameSlotPointHandle(UGameUISlotSubsystem* InExtensionSource, const TSharedPtr<FGameUISlotPoint>& InDataPtr) : ExtensionSource(InExtensionSource), DataPtr(InDataPtr)
	{
	}
};

template <>
struct TStructOpsTypeTraits<FGameSlotPointHandle> : public TStructOpsTypeTraitsBase2<FGameSlotPointHandle>
{
	enum
	{
		WithCopy = true,	// This ensures the opaque type is copied correctly in BPs
		WithIdenticalViaEquality = true,
	};
};

/**
 *
 */
USTRUCT(BlueprintType)
struct GAMECOMMONUIBASE_API FGameUISlotHandle
{
	GENERATED_BODY()

public:
	FGameUISlotHandle()
	{
	}

	void Unregister();

	bool IsValid() const
	{
		return DataPtr.IsValid();
	}

	bool operator==(const FGameUISlotHandle& Other) const
	{
		return DataPtr == Other.DataPtr;
	}
	bool operator!=(const FGameUISlotHandle& Other) const
	{
		return !operator==(Other);
	}

	friend FORCEINLINE uint32 GetTypeHash(FGameUISlotHandle Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}

private:
	TWeakObjectPtr<UGameUISlotSubsystem> ExtensionSource;

	TSharedPtr<FGameUISlot> DataPtr;

	friend UGameUISlotSubsystem;

	FGameUISlotHandle(UGameUISlotSubsystem* InExtensionSource, const TSharedPtr<FGameUISlot>& InDataPtr) : ExtensionSource(InExtensionSource), DataPtr(InDataPtr)
	{
	}
};

template <>
struct TStructOpsTypeTraits<FGameUISlotHandle> : public TStructOpsTypeTraitsBase2<FGameUISlotHandle>
{
	enum
	{
		WithCopy = true,	// This ensures the opaque type is copied correctly in BPs
		WithIdenticalViaEquality = true,
	};
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FGameUISlotRequest
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameUISlotHandle SlotHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SlotPointTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> Data = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> ContextObject = nullptr;
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FExtendExtensionPointDynamicDelegate, EGameUISlotAction, Action, const FGameUISlotRequest&, ExtensionRequest);

/**
 *
 */
UCLASS()
class GAMECOMMONUIBASE_API UGameUISlotSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	FGameSlotPointHandle RegisterExtensionPoint(const FGameplayTag& SlotPointTag, EGameUISlotPointMatch SlotPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);
	FGameSlotPointHandle RegisterExtensionPointForContext(
		const FGameplayTag& SlotPointTag, UObject* ContextObject, EGameUISlotPointMatch SlotPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);

	FGameUISlotHandle RegisterSlotAsWidget(const FGameplayTag& SlotPointTag, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);
	FGameUISlotHandle RegisterSlotAsWidgetForContext(const FGameplayTag& SlotPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);
	FGameUISlotHandle RegisterSlotAsData(const FGameplayTag& SlotPointTag, UObject* ContextObject, UObject* Data, int32 Priority);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot")
	void UnregisterSlot(const FGameUISlotHandle& SlotHandle);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot")
	void UnregisterSlotPoint(const FGameSlotPointHandle& ExtensionPointHandle);

	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void NotifySlotPointOfSlots(TSharedPtr<FGameUISlotPoint>& SlotPoint);
	void NotifySlotPointsOfSlot(EGameUISlotAction Action, TSharedPtr<FGameUISlot>& Slot);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot", meta = (DisplayName = "Register Slot Point"))
	FGameSlotPointHandle K2_RegisterSlotPoint(FGameplayTag SlotPointTag, EGameUISlotPointMatch SlotPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDynamicDelegate ExtensionCallback);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot", meta = (DisplayName = "Register Slot (Widget)"))
	FGameUISlotHandle K2_RegisterSlotAsWidget(FGameplayTag SlotPointTag, TSubclassOf<UUserWidget> WidgetClass, int32 Priority = -1);

	/**
	 * Registers the widget (as data) for a specific player.  This means the extension points will receive a UIExtensionForPlayer data object
	 * that they can look at to determine if it's for whatever they consider their player.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot", meta = (DisplayName = "Register Slot (Widget For Context)"))
	FGameUISlotHandle K2_RegisterExtensionAsWidgetForContext(FGameplayTag SlotPointTag, TSubclassOf<UUserWidget> WidgetClass, UObject* ContextObject, int32 Priority = -1);

	/**
	 * Registers the extension as data for any extension point that can make use of it.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot", meta = (DisplayName = "Register Slot (Data)"))
	FGameUISlotHandle K2_RegisterSlotAsData(FGameplayTag SlotPointTag, UObject* Data, int32 Priority = -1);

	/**
	 * Registers the extension as data for any extension point that can make use of it.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot", meta = (DisplayName = "Register Slot (Data For Context)"))
	FGameUISlotHandle K2_RegisterSlotAsDataForContext(FGameplayTag SlotPointTag, UObject* ContextObject, UObject* Data, int32 Priority = -1);

	FGameUISlotRequest CreateSlotRequest(const TSharedPtr<FGameUISlot>& Slot);

private:
	typedef TArray<TSharedPtr<FGameUISlotPoint>> FExtensionPointList;
	TMap<FGameplayTag, FExtensionPointList> ExtensionPointMap;

	typedef TArray<TSharedPtr<FGameUISlot>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> SlotMap;
};

UCLASS()
class GAMECOMMONUIBASE_API UGameUISlotHandleFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UGameUISlotHandleFunctions()
	{
	}

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot")
	static void Unregister(UPARAM(ref) FGameUISlotHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot")
	static bool IsValid(UPARAM(ref) FGameUISlotHandle& Handle);
};

UCLASS()
class GAMECOMMONUIBASE_API UGameSlotPointHandleFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UGameSlotPointHandleFunctions()
	{
	}

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot")
	static void Unregister(UPARAM(ref) FGameSlotPointHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Slot")
	static bool IsValid(UPARAM(ref) FGameSlotPointHandle& Handle);
};

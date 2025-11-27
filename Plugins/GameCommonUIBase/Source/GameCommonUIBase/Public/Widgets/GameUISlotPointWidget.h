// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/DynamicEntryBoxBase.h"
#include "SubSystem/GameUISlotSubsystem.h"

#include "GameUISlotPointWidget.generated.h"

class IWidgetCompilerLog;

class UGameUILocalPlayer;
class APlayerState;

/**
 * A slot that defines a location in a layout, where content can be added later
 */
UCLASS()
class GAMECOMMONUIBASE_API UGameUISlotPointWidget : public UDynamicEntryBoxBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(TSubclassOf<UUserWidget>, FOnGetWidgetClassForData, UObject*, DataItem);
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnConfigureWidgetForData, UUserWidget*, Widget, UObject*, DataItem);

	UGameUISlotPointWidget(const FObjectInitializer& ObjectInitializer);

	//~UWidget interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	//~End of UWidget interface

private:
	void ResetExtensionPoint();
	void RegisterExtensionPoint();
	void RegisterSlotPointForPlayerState(UGameUILocalPlayer* LocalPlayer, APlayerState* PlayerState);
	void OnAddOrRemoveSlot(EGameUISlotAction Action, const FGameUISlotRequest& Request);

protected:
	/** The tag that defines this extension point */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Slot")
	FGameplayTag SlotPointTag;

	/** How exactly does the extension need to match the extension point tag. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Slot")
	EGameUISlotPointMatch ExtensionPointTagMatch = EGameUISlotPointMatch::ExactMatch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Slot")
	TArray<TObjectPtr<UClass>> DataClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Slot", meta = (IsBindableEvent = "True"))
	FOnGetWidgetClassForData GetWidgetClassForData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Slot", meta = (IsBindableEvent = "True"))
	FOnConfigureWidgetForData ConfigureWidgetForData;

	TArray<FGameSlotPointHandle> ExtensionPointHandles;

	UPROPERTY(Transient)
	TMap<FGameUISlotHandle, TObjectPtr<UUserWidget>> SlotMapping;
};

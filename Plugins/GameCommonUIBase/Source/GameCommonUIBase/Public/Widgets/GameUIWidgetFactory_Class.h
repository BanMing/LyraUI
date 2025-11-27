// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameUIWidgetFactory.h"
#include "Templates/SubclassOf.h"
#include "UObject/SoftObjectPtr.h"

#include "GameUIWidgetFactory_Class.generated.h"

class UObject;
class UUserWidget;

UCLASS()
class GAMECOMMONUIBASE_API UGameUIWidgetFactory_Class : public UGameUIWidgetFactory
{
	GENERATED_BODY()

public:
	UGameUIWidgetFactory_Class() { }

	virtual TSubclassOf<UUserWidget> FindWidgetClassForData_Implementation(const UObject* Data) const override;
	
protected:
	UPROPERTY(EditAnywhere, Category = ListEntries, meta = (AllowAbstract))
	TMap<TSoftClassPtr<UObject>, TSubclassOf<UUserWidget>> EntryWidgetForClass;
};

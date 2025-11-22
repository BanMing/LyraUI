// Copyright Epic Games, Inc. All Rights Reserved.

#include "Common/GameUIWidgetFactory.h"
#include "Templates/SubclassOf.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIWidgetFactory)

class UUserWidget;

TSubclassOf<UUserWidget> UGameUIWidgetFactory::FindWidgetClassForData_Implementation(const UObject* Data) const
{
	return TSubclassOf<UUserWidget>();
}

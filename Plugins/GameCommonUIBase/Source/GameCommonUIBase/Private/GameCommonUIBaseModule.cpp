// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameCommonUIBaseModule.h"

#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "Modules/ModuleManager.h"
#include "SubSystem/GameUIManagerSubsystem.h"

#define LOCTEXT_NAMESPACE "FGameCommonUIBaseModule"

void FGameCommonUIBaseModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "GameCommonUI", LOCTEXT("RuntimeSettingsName", "Game Common UI"), LOCTEXT("RuntimeSettingsDescription", "Configure the Game Common UI plugin"),
			GetMutableDefault<UGameUIManagerSubsystem>());
	}
}

void FGameCommonUIBaseModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "GameCommonUI");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameCommonUIBaseModule, GameCommonGameUIase)
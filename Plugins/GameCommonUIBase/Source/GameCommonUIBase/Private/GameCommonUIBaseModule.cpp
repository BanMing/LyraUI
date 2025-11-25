// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameCommonUIBaseModule.h"

#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "Modules/ModuleManager.h"
#include "SubSystem/GameUIManagerSubsystem.h"
#include "Messaging/GameUIMessagingSubsystem.h"

#define LOCTEXT_NAMESPACE "FGameCommonUIBaseModule"

void FGameCommonUIBaseModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "GameUIManagerSubsystem", LOCTEXT("RuntimeSettingsName", "GameUIManagerSubsystem"), LOCTEXT("RuntimeSettingsDescription", "Configure the Game Common UI plugin"),
			GetMutableDefault<UGameUIManagerSubsystem>());

		SettingsModule->RegisterSettings("Project", "Plugins", "GameUIMessagingSubsystem", LOCTEXT("RuntimeSettingsName", "GameUIMessagingSubsystem"), LOCTEXT("RuntimeSettingsDescription", "Configure the Game Common UI plugin"),
			GetMutableDefault<UGameUIMessagingSubsystem>());
	}
}

void FGameCommonUIBaseModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "GameUIManagerSubsystem");
		SettingsModule->UnregisterSettings("Project", "Plugins", "GameUIMessagingSubsystem");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameCommonUIBaseModule, GameCommonUIBase)
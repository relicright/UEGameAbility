// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"

/**
 * Asset manager is set in the config files -> DefaultEngine.ini
 * @return 
 */
UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>( GEngine->AssetManager );
	return *AuraAssetManager;
}

/**
 * Virtual void that is called during the initial loading of the asset manager.
 * Initialized the native gameplay tags used for the input and ASC.
 */
void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

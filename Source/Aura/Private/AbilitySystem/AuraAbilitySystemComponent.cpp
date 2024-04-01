// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include <AuraGameplayTags.h>
#include <AbilitySystem/Abilities/AuraGameplayAbility.h>

/**
 * Called when the ASC is set on either the character or AI to initialize delegates.
 * Set a delegate for when an effect is applied to self.
 */
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
	
}

/**
 * Function that is bound to the AbilityActorInfoSet() delegate in order to act when the Effect has been applied to self (this ASC).
 * Broadcast tags to the widget controller -- 
 * @param ASC ASC Component effect was applied on
 * @param EffectSpec EffectSpec used on the ASC
 * @param ActiveEffectHandle Effect Handle
 */
void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	// Broadcast the tag to the overlay widget controller or other widgets
	EffectAssetTags.Broadcast(TagContainer);
}

/**
 * Grant abilities to the owner of this ASC by looping through the passed in UGameplayAbility array.
 * @param StartupAbilities Array of abilities to grant to the character.
 */
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{		
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

/**
 * Loop through ASC's Activatable abilities to see if any match the passed in input tag.  If there is an exact match,
 * try to activate that ability from the ASC.
 * @param InputTag Ability that is linked to the InputTag 
 */
void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

/**
 * Loop through ASC's Activatable abilities to see if any match the passed in input tag.  If there is an exact match,
 * call AbilitySpecInputReleased using the AbilitySpec that matches that tag to deactivate it.
 * @param InputTag Ability that is linked to the InputTag 
 */
void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Player/AuraPlayerState.h>
#include <Player/AuraPlayerController.h>
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

}

/**
 * When the pawn is Possessed, the ASC actor info is set for the server,
 * but the client will be set with OnRep_PlayerState.
 * @param NewController 
 */
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();

	// Add abilities this character can use
	AddCharacterAbilities();
}

/**
 * When the PlayerState is replicated/set, the client will set the ASC ability actor infor
 */
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init ability actor info for the client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

/**
 * Setup references for the ASC and the Attribute Set on AuraCharacterBase.
 * Set the ASC ability actor info using the playerstate as the owner and the pawn as the avatar.
 * Initialize the overlay widget
 */
void AAuraCharacter::InitAbilityActorInfo() 
{
	// Get and set the player state, ability system comp and attribute set from the player state
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	// Sets the actor info to its ASC and binds delegates, and more.
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// Initialize the Widget and WidgetController 
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD =	Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// Set all of the default attributes by applying a GameEffect to the character giving them their attributes.
	InitializeDefaultAttributes();  // Used to initialize the attributes of aura - uses game play effect in the editor to set values
}

// Fill out your copyright notice in the Description page of Project Settings.



#include "AbilitySystem/AbilistyTask/TargetDataUnderMouse.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystemComponent.h"

/**
 * Returns a new AbilityTask cast to UTargetDataUnderMouse
 * @param OwningAbility Ability that owns this task
 * @return 
 */
UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		// On the server:  Listen for target data
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

		// Binds OnTargetDataReplicatedCallback function to the AbilityTargetDataSetDelegate
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			SpecHandle,
			ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// Calls the Target data confirm/cancel events if they have been set.  Otherwise, wait for the remote player data to be received
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

/**
 * Use for the local client to send mouse cursor data to the server using prediction and broadcast data locally
 */
void UTargetDataUnderMouse::SendMouseCursorData() const
{
	// Playing the prediction window says everything below is the window of prediction for this ability task
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// Get the result under the cursor using the GetHitResultUnderCursor functions on the PlayerController
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// Target data handle that stores FGameplayAbilityTargetData_SingleTargetHit which used the CursorHit as the stored data
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	// Replicate target data to the server using a prediction key
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		DataHandle,
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);

	// Broadcast the data locally if the ability is still active
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		validData.Broadcast(DataHandle);
	}	
}

/**
 * Once the data is received on the server, consume the replicated target data using the AbilitySpecHandle and prediction key and broadcast the data.
 * @param DataHandle The data sent to the server requiring broadcasting
 * @param ActivationTag Activation tag sent with the AbilityTargetDataSetDelegate
 */
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) const
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		validData.Broadcast(DataHandle);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class AAuraPlayerState;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;
	virtual  void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);
	
protected: 

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr <UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr <UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr <UInputAction> ShiftAction;

	UPROPERTY(EditAnywhere, Category = "Input", ReplicatedUsing = "OnRep_PlayerState")
		TObjectPtr <AAuraPlayerState> AuraPlayerState;

	void Move(const FInputActionValue& InputActionValue);

	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; }
	bool bShiftKeyDown = false;

	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::Zero();
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};

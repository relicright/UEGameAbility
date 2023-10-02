// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class AAuraPlayerState;

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

protected: 

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;


private:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr <UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr <UInputAction> MoveAction;


	UPROPERTY(EditAnywhere, Category = "Input", ReplicatedUsing = "OnRep_PlayerState")
	TObjectPtr <AAuraPlayerState> AuraPlayerState;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
	TObjectPtr <IEnemyInterface> LastActor;
	TObjectPtr <IEnemyInterface> ThisActor;
};

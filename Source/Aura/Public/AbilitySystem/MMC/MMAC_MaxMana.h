// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMAC_MaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMAC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	
	UMMAC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};

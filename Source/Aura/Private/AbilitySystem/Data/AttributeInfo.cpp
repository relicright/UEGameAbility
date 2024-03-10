// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

const FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound /*= false*/)
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant't find INFO for attribute tag [%s) on AttributeInfo [%s]. "), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}

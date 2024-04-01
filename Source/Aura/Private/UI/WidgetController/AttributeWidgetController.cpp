// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeWidgetController.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"


void UAttributeWidgetController::BindCallbackToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	/** Loop through each of the TagsToAttributes inside of the ASC and bind a lambda to GetGameplayAttributeValueChangeDelegate
	*   for each of the attributes in that list.  The lambda will broadcast whenever any changes occur in those
	*   attributes.
	*/
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	// AttributeInfo is set on the BP_AttributeWidgetController and the class is stored on the character's HUD blueprint
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

/**
 * Finds the attribute value needing broadcast using the passed in AttributeTag, store it in the FAuraAttributeInfo struct
 * and then broadcast the FAuraAttributeInfo to the UI binding to that delegate in the editor.
 * @param AttributeTag Tag used to find the associated attribute
 * @param Attribute The attribute we are trying to pull the value from
 */
void UAttributeWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}



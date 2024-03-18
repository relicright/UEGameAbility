 // Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParam(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

 /**
  * Broadcast the initialized values from the character after all of the controller params have been set and
  * stored on the widget.
  */
 void UAuraWidgetController::BroadcastInitialValues()
{

}

void UAuraWidgetController::BindCallbackToDependencies()
{

}

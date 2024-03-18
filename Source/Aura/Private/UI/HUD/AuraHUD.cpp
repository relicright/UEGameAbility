// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeWidgetController.h"

/**
 * Returns an OverlayWidgetController or creates one if none exists
 * @param WCParams Parameters to set the WidgetController with
 * @return 
 */
UOverlayWidgetController* AAuraHUD::GetoverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParam(WCParams);
		OverlayWidgetController->BindCallbackToDependencies();
	}
	return OverlayWidgetController;
}

/**
 * Returns an AttributeWidgetController or creates one using the passed in FWidgetControllerParams
 * @param WCParams WidgetController Params to Use for the AttributeMenuWidgetController
 * @return 
 */
UAttributeWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParam(WCParams);
		AttributeMenuWidgetController->BindCallbackToDependencies();
	}
	return AttributeMenuWidgetController;
}

/**
 * Creates the OverlayWidget, WidgetController, Sets all of the parameters and displays to the viewport.
 * @param PC PlayerController who owns this Overlay
 * @param PS PlayerState of the owner of this Overlay
 * @param ASC AbilitySystemComponent that belongs to The Overlay owner
 * @param AS AttributeSet that belongs to the Overlay owner
 */
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class uninitialized.  Please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget controller class uninitialized.  Please fill out BP_AuraHUD"));

	// Create the widget and set OverlayWidget to the Widget cast to AAuraUserWidget
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	// Store WidgetControllerParams and Create an OverlayWidgetController using them
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetoverlayWidgetController(WidgetControllerParams);

	// Set the OverlayWidget WidgetController to the newly created controller and broadcast initial values
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	// Display the widget to the viewport
	Widget->AddToViewport();
}

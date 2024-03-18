// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AuraUserWidget.h"

/**
 * Sets the WidgetController for the designated widget then calls the WidgetControllerSet()
 * function (BlueprintImplementable) which fires off an event inside of the editor to help store references to the
 * new WidgetController for the parent Widget and Children Widgets and bind in-editor callbacks.
 * This is used on health bars, mana bars, attribute menus and more.
 * @param InWidgetController 
 */
void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

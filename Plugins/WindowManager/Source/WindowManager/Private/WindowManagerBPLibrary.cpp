// Copyright Epic Games, Inc. All Rights Reserved.

#include "WindowManagerBPLibrary.h"
#include "WindowManager.h"

#include "Components/PanelWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"

/**
	* Simplifies window management.
	* 
	* @param ArrayWidgetsRef stores active widgets and their names
	* @param bWidgetsOpened stores information about at least one open widget with WM
	* @param bActionOpen specifies the action of adding the widget (true) or closing it (false)
	* @param bActionCloseAll closes all created widgets
	* @param WidgetClass target widget
	* @param OwningController owning player controller
	* @param bIsReplaced whether the last open widget will be replaced by a new one
	* @param bShowCursor show cursor
	* @param bFocusViewport keeps focus on the viewport
	* @param ZOrder adjusts the display priority of the widget layer
	* @param ChildContainer target container where the widget will be embedded (can be empty)
	* @param ReturnArrayWidgetsRef returns an updated list of open widgets
	* @param bReturnWidgetsOpened returns information about at least one open widget with WM
	*/
void UWindowManagerBPLibrary::WindowManagerCpp(
	TArray<UUserWidget*> ArrayWidgetsRef,
	bool bWidgetsOpened,
	bool bActionOpen,
	bool bActionCloseAll,
	TSubclassOf<UUserWidget> WidgetClass,
	APlayerController* OwningController,
	bool bIsReplaced,
	bool bShowCursor,
	bool bFocusViewport,
	int32 ZOrder,
	UPanelWidget* ChildContainer,
	TArray<UUserWidget*> &ReturnArrayWidgetsRef,
	bool &bReturnWidgetsOpened)
{
	if (WidgetClass && bActionOpen == true && bActionCloseAll == false)
	{
		bool IsDuplicate = CheckDuplicateWidgets(ArrayWidgetsRef, WidgetClass, ChildContainer);

		if (IsDuplicate == false)
		{
			// Remove the Last Window to Replace It
			if (bIsReplaced == true && !ArrayWidgetsRef.IsEmpty())
			{
				RemoveWidget(OwningController, ArrayWidgetsRef, WidgetClass, bShowCursor, bFocusViewport, bActionOpen);
			}

			if (!ChildContainer)
			{
				AddWidgetToViewport(OwningController, ArrayWidgetsRef, WidgetClass, bShowCursor, bFocusViewport, ZOrder);

				// Changing the Visibility of the Cursor
				OwningController->bShowMouseCursor = bShowCursor;
			}
			else
			{
				// Create a Widget and Adding it to an Array
				AddChildWidget(OwningController, WidgetClass, ChildContainer);
			}
		}

		// Report if at Least One Window is Open
		if (!ArrayWidgetsRef.IsEmpty())
		{
			bWidgetsOpened = true;
		}
	}
	else if (!ChildContainer && bActionOpen == false && bActionCloseAll == false)
	{
		// Removing a Target Widget
		// Remove the Last Opening Widget from the Viewport and Widgets Array
		if (!ArrayWidgetsRef.IsEmpty())
		{

			RemoveWidget(OwningController, ArrayWidgetsRef, WidgetClass, bShowCursor, bFocusViewport, bActionOpen);

			// If there are No Open Widgets Left
			if (ArrayWidgetsRef.IsEmpty())
			{
				bWidgetsOpened = false;
			}
		}

		if (!ArrayWidgetsRef.IsValidIndex(ArrayWidgetsRef.Num() - 1))
		{
			// Changing the Visibility of the Cursor
			OwningController->bShowMouseCursor = bShowCursor;
		}
	}
	else if (!ChildContainer && bActionCloseAll == true)
	{
		// Remove All Widgets
		if (UGameViewportClient* ViewportClient = GEngine->GameViewport)
		{
			UWidgetLayoutLibrary::RemoveAllWidgets(ViewportClient->GetWorld());
		}

		// Checking Open Widgets
		if (!ArrayWidgetsRef.IsEmpty())
		{
			// Clearing the Widget Storage
			ArrayWidgetsRef.Empty();
			bWidgetsOpened = false;
		}

		// Focus to Game Viewport
		if (bFocusViewport == true)
		{
			UWidgetBlueprintLibrary::SetFocusToGameViewport();
		}

		// Changing the Visibility of the Cursor
		OwningController->bShowMouseCursor = bShowCursor;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function \"Window Manager\" in \"WindowManager.cpp\" was unable to process the request."));
	}

	// Corrects the Invisibility of the Cursor if it has Not Been Offset
	if (!ChildContainer && bActionCloseAll == true)
	{
		FixShowingMouseCursor(OwningController);
	}

	// Output Data (Return Function Data)
	ReturnArrayWidgetsRef = ArrayWidgetsRef;
	bReturnWidgetsOpened = bWidgetsOpened;
}

// Creating and Adding Widget to Viewport
void UWindowManagerBPLibrary::AddWidgetToViewport(APlayerController* OwningController, TArray<UUserWidget*>& ArrayWidgetsRef, TSubclassOf<UUserWidget> TargetWidgetClass, bool bShowCursor, bool bFocusViewport, int32 TargetZOrder)
{
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(OwningController, TargetWidgetClass, *TargetWidgetClass->GetName());

	// Adding Widget
	if (NewWidget)
	{
		NewWidget->AddToViewport(TargetZOrder);

		// Storing a List of Parent Widgets
		AddParentWidgetInArray(NewWidget, ArrayWidgetsRef);
	}

	// Set Focus to Widget
	if (bFocusViewport == true)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(OwningController, NewWidget);
	}

	// Corrects the Invisibility of the Cursor if it has Not Been Offset
	FixShowingMouseCursor(OwningController);
}

// Adding Target Parent Widget to Array
void UWindowManagerBPLibrary::AddParentWidgetInArray(UUserWidget* NewWidget, TArray<UUserWidget*>& ArrayWidgetsRef)
{
	if (NewWidget)
	{
		ArrayWidgetsRef.Add(NewWidget);
	}
}

// Adding a Child Widget to the Container
void UWindowManagerBPLibrary::AddChildWidget(APlayerController* OwningController, TSubclassOf<UUserWidget> TargetWidgetClass, UPanelWidget* ChildContainer)
{
	UUserWidget* ChildWidget = CreateWidget<UUserWidget>(OwningController, TargetWidgetClass, *TargetWidgetClass->GetName());

	if (ChildWidget)
	{
		ChildContainer->AddChild(ChildWidget);
	}
}

// Remove the Last Opening Widget
void UWindowManagerBPLibrary::RemoveWidget(APlayerController* OwningController, TArray<UUserWidget*>& ArrayWidgetsRef, TSubclassOf<UUserWidget> TargetWidgetClass, bool bShowCursor, bool bFocusViewport, bool ActionOpen)
{
	if (TargetWidgetClass && ActionOpen == false)
	{
		int32 CountWidgets = 0;
		
		for (UUserWidget* Widget : ArrayWidgetsRef)
		{
			CountWidgets++;

			if (Widget->GetName() == TargetWidgetClass->GetName())
			{
				ArrayWidgetsRef.RemoveAt(CountWidgets - 1);
				Widget->RemoveFromParent();
				break;
			}
		}
	}
	else
	{
		UUserWidget* LastWidget = ArrayWidgetsRef.Last();

		if (LastWidget)
		{
			LastWidget->RemoveFromParent();

			// Removing the Last Element in an Array
			ArrayWidgetsRef.RemoveAt(ArrayWidgetsRef.Num() - 1);
		}
	}

	// Focus to Game Viewport
	if (bFocusViewport == true)
	{
		UWidgetBlueprintLibrary::SetFocusToGameViewport();
	}
}

// Checking for Duplicate Widgets
bool UWindowManagerBPLibrary::CheckDuplicateWidgets(TArray<UUserWidget*>& ArrayWidgetsRef, TSubclassOf<UUserWidget> TargetWidgetClass, UPanelWidget* ChildContainer)
{
	if (!ChildContainer)
	{
		for (UUserWidget* Item : ArrayWidgetsRef)
		{
			if (Item->GetName() == TargetWidgetClass->GetName())
			{
				return true;
			}
		}
	}

	return false;
}

// Corrects the Invisibility of the Cursor if it has Not Been Offset
void UWindowManagerBPLibrary::FixShowingMouseCursor(APlayerController* OwningController)
{
	if (OwningController)
	{
		double PosX, PosY;
		OwningController->GetMousePosition(PosX, PosY);
		OwningController->SetMouseLocation(PosX + 1 - 1, PosY);
	}
}

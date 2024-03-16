// UE5.3. 2024. Pavel Gornostaev <pavreally@gmail.com>

#include "WindowManager.h"

#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

/**
	* Simplifies window management.
	* 
	* @param ArrayWindowsActive stores active widgets and their names
	* @param bWindowsOpened stores information about at least one open widget with WM
	* @param bActionOpen specifies the action of adding the widget (true) or closing it (false)
	* @param bActionCloseAll closes all created widgets
	* @param WidgetClass target widget
	* @param OwningController owning player controller
	* @param bIsReplaced whether the last open widget will be replaced by a new one
	* @param bShowCursor show cursor
	* @param bFocusViewport keeps focus on the viewport
	* @param ZOrder adjusts the display priority of the widget layer
	* @param ChildContainer target container where the widget will be embedded (can be empty)
	* @param ReturnWindowsActive returns an updated list of open widgets
	* @param bReturnWindowsOpened returns information about at least one open widget with WM
	*/
void UWindowManager::WindowManagerCpp(
	TArray<FWidgetsParent> ArrayWindowsActive,
	bool bWindowsOpened,
	bool bActionOpen,
	bool bActionCloseAll,
	TSubclassOf<UUserWidget> WidgetClass,
	APlayerController* OwningController,
	bool bIsReplaced,
	bool bShowCursor,
	bool bFocusViewport,
	int32 ZOrder,
	UPanelWidget* ChildContainer,
	TArray<FWidgetsParent> &ReturnWindowsActive,
	bool &bReturnWindowsOpened)
{
	if (WidgetClass && bActionOpen == true && bActionCloseAll == false)
	{
		// Add Widget

		bool IsDuplicate = CheckDuplicateWidgets(ArrayWindowsActive, WidgetClass, ChildContainer);

		if (IsDuplicate == false)
		{
			// Remove the Last Window to Replace It
			if (bIsReplaced == true)
			{
				if (!ArrayWindowsActive.IsEmpty())
				{
					RemoveWidgetLast(OwningController, ArrayWindowsActive, bShowCursor, bFocusViewport);
				}
			}

			if (!ChildContainer)
			{
				// Create a Widget and Adding it to an Array
				AddWidgetToViewport(OwningController, ArrayWindowsActive, WidgetClass, bShowCursor, bFocusViewport, ZOrder);

				// Changing the Visibility of the Cursor
				OwningController->bShowMouseCursor = bShowCursor;
			}
			else
			{
				AddChildWidget(OwningController, WidgetClass, ChildContainer);
			}
		}

		// Report if at Least One Window is Open
		if (!ArrayWindowsActive.IsEmpty())
		{
			bWindowsOpened = true;
		}
	}
	else if (!ChildContainer && bActionOpen == false && bActionCloseAll == false)
	{
		// Remove Widget

		// Remove the Last Opening Widget from the Viewport and Widgets Array
		if (!ArrayWindowsActive.IsEmpty())
		{
			RemoveWidgetLast(OwningController, ArrayWindowsActive, bShowCursor, bFocusViewport);

			// If there are No Open Windows Left
			if (ArrayWindowsActive.IsEmpty())
			{
				bWindowsOpened = false;
			}
		}

		if (!ArrayWindowsActive.IsValidIndex(ArrayWindowsActive.Num() - 1))
		{
			// Changing the Visibility of the Cursor
			OwningController->bShowMouseCursor = bShowCursor;
		}
	}
	else if (!ChildContainer && bActionCloseAll == true)
	{
		// Remove All Widgets

		// Checking Open Widgets
		if (!ArrayWindowsActive.IsEmpty())
		{
			UWidgetLayoutLibrary::RemoveAllWidgets(GEngine->GameViewport->GetWorld());

			// Clearing the Widget Storage
			ArrayWindowsActive.Empty();
			bWindowsOpened = false;

			// Focus to Game Viewport
			if (bFocusViewport == true)
			{
				UWidgetBlueprintLibrary::SetFocusToGameViewport();
			}
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
	ReturnWindowsActive = ArrayWindowsActive;
	bReturnWindowsOpened = bWindowsOpened;
}

// Creating and Adding Widget to Viewport
void UWindowManager::AddWidgetToViewport(APlayerController* OwningController, TArray<FWidgetsParent>& ArrayWindowsActive, TSubclassOf<UUserWidget> TargetWidgetClass, bool bShowCursor, bool bFocusViewport, int32 TargetZOrder)
{
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(OwningController, TargetWidgetClass, *TargetWidgetClass->GetName());

	// Adding Widget
	if (NewWidget)
	{
		NewWidget->AddToViewport(TargetZOrder);

		// Storing a List of Parent Widgets
		AddParentWidgetInArray(NewWidget, ArrayWindowsActive, TargetWidgetClass);
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
void UWindowManager::AddParentWidgetInArray(UUserWidget* NewWidget, TArray<FWidgetsParent>& ArrayWindowsActive, TSubclassOf<UUserWidget> TargetWidgetClass)
{
	FString NewWidgetName = TargetWidgetClass->GetDisplayNameText().ToString();

	// Adding Stucture in Widgets Array
	FWidgetsParent StructParentWidgets;
	ArrayWindowsActive.Add(
		FWidgetsParent {
			StructParentWidgets.WidgetObjectRef = NewWidget,
			StructParentWidgets.WidgetName = NewWidgetName
		}
	);
}

// Adding a Child Widget to the Container
void UWindowManager::AddChildWidget(APlayerController* OwningController, TSubclassOf<UUserWidget> TargetWidgetClass, UPanelWidget* ChildContainer)
{
	UUserWidget* ChildWidget = CreateWidget<UUserWidget>(OwningController, TargetWidgetClass, *TargetWidgetClass->GetName());

	if (ChildWidget)
	{
		ChildContainer->AddChild(ChildWidget);
	}
}

// Remove the Last Opening Widget
void UWindowManager::RemoveWidgetLast(APlayerController* OwningController, TArray<FWidgetsParent>& ArrayWindowsActive, bool bShowCursor, bool bFocusViewport)
{
	UUserWidget* LastWidget = ArrayWindowsActive.Last().WidgetObjectRef;

	if (LastWidget)
	{
		LastWidget->RemoveFromParent();
	
		// Removing the Last Element in an Array
		ArrayWindowsActive.RemoveAt(ArrayWindowsActive.Num() - 1);

		// Focus to Game Viewport
		if (bFocusViewport == true)
		{
			UWidgetBlueprintLibrary::SetFocusToGameViewport();
		}
	}
}

// Checking for Duplicate Widgets
bool UWindowManager::CheckDuplicateWidgets(TArray<FWidgetsParent> ArrayWindowsActive, TSubclassOf<UUserWidget> WidgetClass, UPanelWidget* ChildContainer)
{
	if (!ChildContainer)
	{
		for (FWidgetsParent Item : ArrayWindowsActive)
		{
			if (Item.WidgetName == WidgetClass->GetDisplayNameText().ToString())
			{
				return true;
			}
		}
	}

	return false;
}

// Corrects the Invisibility of the Cursor if it has Not Been Offset
void UWindowManager::FixShowingMouseCursor(APlayerController* OwningController)
{
	if (OwningController)
	{
		double PosX, PosY;
		OwningController->GetMousePosition(PosX, PosY);
		OwningController->SetMouseLocation(PosX + 1 - 1, PosY);
	}
}

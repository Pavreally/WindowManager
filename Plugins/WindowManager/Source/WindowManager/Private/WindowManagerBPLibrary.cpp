// Pavel Gornostaev <https://github.com/Pavreally>

#include "WindowManagerBPLibrary.h"
#include "WindowManager.h"
#include "Components/PanelWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"

#pragma region PUBLIC_FUNCTIONS
void UWindowManagerBPLibrary::CreateWidgetWM(
		TArray<FWidgetStorageWM>& WidgetStorage,
		APlayerController* OwningController,
		TSubclassOf<UUserWidget> WidgetClass,
		FGameplayTag Tag,
		int32 ZOrder,
		FOptionsWM Options,
		FExpOptionsWM ExperimentalOptions)
{
	if (!OwningController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Window Manager (CreateWidgetWM): Controller is undefined!"));

		return;
	}

	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Window Manager (CreateWidgetWM): Widget is undefined!"));

		return;
	}

	// Checking for duplicates
	bool bIsDuplicateWidget = false;
	if (FindWidgetReference(OwningController, WidgetStorage, WidgetClass, true)) 
		bIsDuplicateWidget = true;

	// Create widget only if included array is empty
	if (ExperimentalOptions.NoOtherWidgetsInArray == true && !WidgetStorage.IsEmpty() && !bIsDuplicateWidget)
		return;

	// Create widget only if there are no other widgets added (globally)
	if (ExperimentalOptions.NoOtherWidgetsGlobal == true && FindWidgetReference(OwningController, WidgetStorage, UUserWidget::StaticClass(), true, true))
		return;

	// Remove the last widget in an array to replace it
	if (ExperimentalOptions.ReplaceLastWidget && !WidgetStorage.IsEmpty())
	{
		TSubclassOf<UUserWidget> LastWidgetClass = WidgetStorage.Last().WidgetsRef->GetClass();
		if (!bIsDuplicateWidget)
			RemoveWidgetWM(WidgetStorage, OwningController, LastWidgetClass, false, Options);
	}

	// Create a Widget and adding it to the viewport
	UUserWidget* TargetWidget = CreateWidget<UUserWidget>(OwningController, WidgetClass, *WidgetClass->GetName());
	if (TargetWidget) TargetWidget->AddToViewport(ZOrder);

	// Update the array with widgets by reference
	if (!bIsDuplicateWidget)
	{
		AddWidgetToArray(WidgetStorage, TargetWidget, Tag);
	}
	// Apply additional options
	ApplyOptions(OwningController, TargetWidget, Options);
}

void UWindowManagerBPLibrary::RemoveWidgetWM(TArray<FWidgetStorageWM>& WidgetStorage, APlayerController* OwningController, TSubclassOf<UUserWidget> WidgetClass, bool bSmartCursor, FOptionsWM Options)
{
	if (!OwningController || !WidgetClass)
		return;

	int32 FoundWidgetIndex = FindWidgetIndex(OwningController, WidgetStorage, WidgetClass);

	if (!WidgetStorage.IsEmpty() && FoundWidgetIndex != -1)
	{
		WidgetStorage[FoundWidgetIndex].WidgetsRef->RemoveFromParent();
		WidgetStorage.RemoveAt(FoundWidgetIndex);
	}
	
	UUserWidget* UserWidgetGlobal = FindWidgetReference(OwningController, WidgetStorage, WidgetClass, false);
	if (WidgetStorage.IsEmpty() && UserWidgetGlobal)
	{
		UserWidgetGlobal->RemoveFromParent();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Window Manager (RemoveWidgetWM): There is nothing to delete. The specified widget \"%s\" does not exist."), *WidgetClass->GetName());

		return;
	}

	// Check if there are any more open widgets
	UUserWidget* AnyWidgetGlobal = FindWidgetReference(OwningController, WidgetStorage, UUserWidget::StaticClass(), false, true);
	if (bSmartCursor && WidgetStorage.IsEmpty() && !AnyWidgetGlobal)
	{
		// Disable the cursor if there are no visible widgets left.
		OwningController->bShowMouseCursor = false;
		if (Options.FocusWidget)
		{
			UWidgetBlueprintLibrary::SetFocusToGameViewport();
		}
	}

	if (!WidgetStorage.IsEmpty() || AnyWidgetGlobal)
	{
		// Apply additional options
		ApplyOptions(OwningController, nullptr, Options);
	}
}

void UWindowManagerBPLibrary::RemoveAllWidgetsWM(TArray<FWidgetStorageWM>& WidgetStorage, APlayerController* OwningController, bool bRemoveByTag, FGameplayTagContainer Tags, bool bSmartCursor, FOptionsWM Options)
{
	if (!OwningController)
		return;

	// Removing widgets by the specified tag in the connected data array
	if (!WidgetStorage.IsEmpty())
	{
		if (bRemoveByTag)
		{
			// Removing widgets with specified tags
			for (int32 i = WidgetStorage.Num() - 1; i >=0; --i)
			{
				if (Tags.HasTag(WidgetStorage[i].Tag))
				{
					if (WidgetStorage[i].WidgetsRef)
						WidgetStorage[i].WidgetsRef->RemoveFromParent();
					
					WidgetStorage.RemoveAt(i);
				}
			}
		}
		else
		{
			// Remove all in storage
			for (FWidgetStorageWM Widget : WidgetStorage)
			{
				if (Widget.WidgetsRef)
					Widget.WidgetsRef->RemoveFromParent();
			}
			WidgetStorage.Empty();
		}
	}
	else
	{
		// Remove all widgets globally
		UWorld* World = OwningController->GetWorld();
		if (!World)
			return;

		UWidgetLayoutLibrary::RemoveAllWidgets(World);
		WidgetStorage.Empty();
	}

	// Check if there are any more open widgets
	UUserWidget* AnyWidgetGlobal = FindWidgetReference(OwningController, WidgetStorage, UUserWidget::StaticClass(), false, true);
	if (bSmartCursor && WidgetStorage.IsEmpty() && !AnyWidgetGlobal)
	{
		// Disable the cursor if there are no visible widgets left.
		OwningController->bShowMouseCursor = false;
		if (Options.FocusWidget)
		{
			UWidgetBlueprintLibrary::SetFocusToGameViewport();
		}
	}

	if (!WidgetStorage.IsEmpty() || AnyWidgetGlobal)
	{
		// Apply additional options
		ApplyOptions(OwningController, nullptr, Options);
	}
}

void UWindowManagerBPLibrary::CreateChildWidgetWM(APlayerController* OwningController, UPanelWidget* ChildContainer, TSubclassOf<UUserWidget> WidgetClass, bool bReplace)
{
	UUserWidget* ChildWidget = CreateWidget<UUserWidget>(OwningController, WidgetClass, *WidgetClass->GetName());
	if (!ChildWidget || !ChildContainer)
		return;

	if (bReplace) ChildContainer->ClearChildren();
	ChildContainer->AddChild(ChildWidget);
}

#pragma endregion PUBLIC_FUNCTIONS

#pragma region PRIVATE_FUNCTIONS
void UWindowManagerBPLibrary::AddWidgetToArray(TArray<FWidgetStorageWM>& WidgetStorage, UUserWidget* WidgetClass, FGameplayTag& Tag)
{
	FWidgetStorageWM NewWidget;
	NewWidget.WidgetsRef = WidgetClass;
	NewWidget.Tag = Tag;
	
	WidgetStorage.Add(NewWidget);
}

void UWindowManagerBPLibrary::ApplyOptions(APlayerController* OwningController, UUserWidget* TargetWidget, FOptionsWM& Options)
{
	if (!OwningController)
		return;

	// Changing the Visibility of the Cursor
	OwningController->bShowMouseCursor = Options.ShowCursor;

	// Set Focus to Widget
	if (Options.FocusWidget == true)
	{
		if (TargetWidget)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(OwningController, TargetWidget);
		}
		else
		{
			UWidgetBlueprintLibrary::SetFocusToGameViewport();
		}
	}

	// Fixes the cursor not being visible if it was not moved after opening the widget.
	if (Options.SimulateCursorDisplacement == true)
	{
		double PosX, PosY;
		OwningController->GetMousePosition(PosX, PosY);
		OwningController->SetMouseLocation(PosX + 1 - 1, PosY);
	}
}

int32 UWindowManagerBPLibrary::FindWidgetIndex(APlayerController* OwningController, TArray<FWidgetStorageWM>& WidgetStorage, TSubclassOf<UUserWidget>& WidgetClass)
{
	if (!OwningController || !WidgetClass)
		return -1;

	// Search for widget in connected storage
	UClass* TargetWidgetClass = WidgetClass.Get();

	for (int32 i = 0; i < WidgetStorage.Num(); i++)
	{
		UUserWidget* WidgetRef = WidgetStorage[i].WidgetsRef;
		if (WidgetRef && WidgetRef->GetClass() == TargetWidgetClass)
		{
			return i;
		}
	}

	return -1;
}

UUserWidget* UWindowManagerBPLibrary::FindWidgetReference(APlayerController* OwningController, TArray<FWidgetStorageWM>& WidgetStorage, TSubclassOf<UUserWidget> WidgetClass, bool bIsUsedWidgetStorageWM, bool bFindAnyWidget)
{
	if (!OwningController || !WidgetClass)
		return nullptr;

	// Finding a widget in a connected data array
	if (bIsUsedWidgetStorageWM && !WidgetStorage.IsEmpty())
	{
		int32 WidgetIndex = FindWidgetIndex(OwningController, WidgetStorage, WidgetClass);

		if (WidgetIndex != -1 && WidgetStorage[WidgetIndex].WidgetsRef != nullptr)
		{
			return WidgetStorage[WidgetIndex].WidgetsRef;
		}
	}

	// Search for widget in global space
	UWorld* World = OwningController->GetWorld();
	if (!World)
		return nullptr;
	
	TArray<UUserWidget*> FoundWidgets;

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(
		OwningController,
		FoundWidgets,
		WidgetClass
	);

	if (bFindAnyWidget == true && FoundWidgets.Num() > 0)
	{
		// If we only need the fact of the presence of any widget, we return the first one
		return FoundWidgets[0];
	}

	for (UUserWidget* Widget : FoundWidgets)
	{
		if (Widget && Widget->GetClass() == WidgetClass.Get())
		{
			return Widget;
		}
	}

	return nullptr;
}

#pragma endregion PRIVATE_FUNCTIONS

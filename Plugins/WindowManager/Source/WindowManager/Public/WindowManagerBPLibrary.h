// Copyright Epic Games, Inc. All Rights Reserved.

/**
 * This feature will help you quickly set up secure logic for UI.
 * 
 * Features:
 * - Control important Widgets so that they are not blocked by anything;
 * - Simplify the logic on how to customize composite Widgets;
 * - Control the focus of Widgets without adding repetitive logic;
 * - Control the cursor display at the same time;
 * - Easily add child Widgets to the parent widget container.
 * 
 * ----------------------------------------------------------------------
 * Warning! Don't forget to add "UMG" in the dependencies for the project file "YouProjectName.Build.cs".
 */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "WindowManagerBPLibrary.generated.h"

class UPanelWidget;
class APlayerController;

/**
 * WindowManager Function Library Class
 */
UCLASS()
class UWindowManagerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Simplifies window management.
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Window Manager", Keywords = "WindowManager, UI, Widget"), Category = "Window Manager")
	static void WindowManagerCpp(
		TArray<UUserWidget*> WidgetsRef,
		bool WidgetsOpened,
		bool ActionOpen,
		bool ActionCloseAll,
		TSubclassOf<UUserWidget> WidgetClass,
		APlayerController* OwningController,
		bool IsReplaced,
		bool ShowCursor,
		bool FocusViewport,
		int32 ZOrder,
		UPanelWidget* ChildContainer,
		TArray<UUserWidget*> &ReturnWidgetsRef,
		bool &ReturnWidgetsOpened);

private:
	UFUNCTION()
	static void AddWidgetToViewport(
		APlayerController* OwningController,
		TArray<UUserWidget*>& WidgetsRef,
		TSubclassOf<UUserWidget> TargetWidgetClass,
		bool ShowCursor,
		bool FocusViewport,
		int32 TargetZOrder
		);

	UFUNCTION()
	static void AddParentWidgetInArray(UUserWidget* NewWidget, TArray<UUserWidget*>& WidgetsRef);

	UFUNCTION()
	static void AddChildWidget(
		APlayerController* OwningController,
		TSubclassOf<UUserWidget> TargetWidgetClass,
		UPanelWidget* ChildContainer
		);

	UFUNCTION()
	static void RemoveWidget(
		APlayerController* OwningController,
		TArray<UUserWidget*>& WidgetsRef,
		TSubclassOf<UUserWidget> TargetWidgetClass,
		bool ShowCursor,
		bool FocusViewport,
		bool ActionOpen
		);

	UFUNCTION()
	static bool CheckDuplicateWidgets(TArray<UUserWidget*>& WidgetsRef, TSubclassOf<UUserWidget> TargetWidgetClass, UPanelWidget* ChildContainer);

	// Corrects the Invisibility of the Cursor if it has Not Been Offset
	UFUNCTION()
	static void FixShowingMouseCursor(APlayerController* OwningController);
};

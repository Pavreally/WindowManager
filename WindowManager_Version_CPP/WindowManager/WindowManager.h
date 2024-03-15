// UE5.3. 2024. Pavel Gornostaev <pavreally@gmail.com>

/**
 * This feature will help you quickly set up secure logic for UI.
 * 
 * Features:
 * - Control important windows so that they are not blocked by anything;
 * - Simplify the logic on how to customize composite windows;
 * - Control the focus of windows without adding repetitive logic;
 * - Control the cursor display at the same time;
 * - Easily add child windows to the parent widget container.
 * 
 * ----------------------------------------------------------------------
 * Don't forget to add "UMG" in the dependencies for the project file "YouProjectName.Build.cs".
 */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WindowManager.generated.h"

class UUserWidget;
class UPanelWidget;
class APlayerController;

// Creating Structures for Storing Widgets
USTRUCT(BlueprintType)
struct FWidgetsParent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* WidgetObjectRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WidgetName;
};

/**
 * WindowManager Function Library Class
 */
UCLASS()
class UWindowManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Simplifies window management.
	UFUNCTION(BlueprintCallable, Category = "Window Manager")
	static void WindowManagerCpp(
		TArray<FWidgetsParent> InputWindowsActive,
		bool InputWindowsOpened,
		bool ActionOpen,
		bool ActionCloseAll,
		TSubclassOf<UUserWidget> WidgetClass,
		APlayerController* OwningController,
		bool IsReplaced,
		bool ShowCursor,
		bool FocusViewport,
		int32 ZOrder,
		UPanelWidget* ChildContainer,
		TArray<FWidgetsParent> &ReturnWindowsActive,
		bool &ReturnWindowsOpened);

private:
	UFUNCTION()
	static void AddWidgetToViewport(
		APlayerController* OwningController,
		TArray<FWidgetsParent>& ArrayWindowsActive,
		TSubclassOf<UUserWidget> TargetWidgetClass,
		bool ShowCursor,
		bool FocusViewport,
		int32 TargetZOrder
		);

	UFUNCTION()
	static void AddParentWidgetInArray(UUserWidget* NewWidget, TArray<FWidgetsParent>& ArrayWindowsActive, TSubclassOf<UUserWidget> TargetWidgetClass);

	UFUNCTION()
	static void AddChildWidget(
		APlayerController* OwningController,
		TSubclassOf<UUserWidget> TargetWidgetClass,
		UPanelWidget* ChildContainer
		);

	UFUNCTION()
	static void RemoveWidgetLast(
		APlayerController* OwningController,
		TArray<FWidgetsParent>& ArrayWindowsActive,
		bool ShowCursor,
		bool FocusViewport
		);

	UFUNCTION()
	static bool CheckDuplicateWidgets(TArray<FWidgetsParent> ArrayWindowsActive, TSubclassOf<UUserWidget> WidgetClass, UPanelWidget* ChildContainer);

	// Corrects the Invisibility of the Cursor if it has Not Been Offset
	UFUNCTION()
	static void FixShowingMouseCursor(APlayerController* OwningController);
};

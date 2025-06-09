// Pavel Gornostaev <https://github.com/Pavreally>

/**
 * This function library helps you quickly and easily set up user interface logic using UMG.
 * 
 * Features:
 * - Versatile widget creation setup.
 * - (Optional) Ability to store references to active widgets in a connected data array, 
 * allowing for more flexible widget management. If different arrays are used, they are 
 * handled separately and automatically updated through reference-based access.
 * - (Optional) Functions can be used without a data array. However, in this case, tag-based 
 * management will not be available, as tags are not stored anywhere.
 * - Built-in options to quickly configure cursor visibility, input focus, or apply a fix for 
 * an issue where the cursor is automatically hidden after opening or closing a widget.
 * - Experimental options that prevent a widget from being created if any other widget already 
 * exists—either globally or within a specific data array.
 * - Tag-based control allows for batch management and removal of widget groups. (Note: This 
 * requires a connected data array to function.)
 * - Single-widget removal function doesn't require a data array. Just pass the target widget 
 * to the function, and it will remove it wherever it is.
 * - Simple child widget addition with optional automatic replacement of container content.
 */

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"

#include "WindowManagerBPLibrary.generated.h"

class UPanelWidget;
class APlayerController;

#pragma region STRUCTS
// Structure for storing open widgets
USTRUCT(BlueprintType)
struct FWidgetStorageWM
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Manager")
	TObjectPtr<UUserWidget> WidgetsRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Manager")
	FGameplayTag Tag;
};

// Structure for additional options
USTRUCT(BlueprintType)
struct FOptionsWM
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Manager", meta = (ToolTip = "Displays the mouse cursor."))
	bool ShowCursor = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Manager", meta = (ToolTip = "Sets focus to the widget. If the widget does not exist, sets focus to the game viewport."))
	bool FocusWidget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Manager", meta = (ToolTip = "Fixes the cursor not being visible if it was not moved after opening the widget."))
	bool SimulateCursorDisplacement = false;
};

// Structure for experimental solutions
USTRUCT(BlueprintType)
struct FExpOptionsWM
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Manager", meta = (ToolTip = "After creating a widget, the last added widget is removed (Only works if the array is connected)."))
	bool ReplaceLastWidget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Manager", meta = (ToolTip = "The widget will be created if no other widgets have been created in the attached array (Only works if the array is connected)."))
	bool NoOtherWidgetsInArray = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Window Manager", meta = (ToolTip = "The widget will be created if no other widgets have been created globally."))
	bool NoOtherWidgetsGlobal = false;
};


#pragma endregion STRUCTS

/**
 * Window Manager (WM) Function Library Class
 */
UCLASS()
class UWindowManagerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Brings a new widget to the screen
	 * @param WidgetStorage Array reference. A connectable data array (connection may be absent) that contains a reference to the widget and its tag.
	 * @param OwningController The player controller.
	 * @param WidgetClass The target widget.
	 * @param Tag The identifying tag of the widget.
	 * @param ZOrder The widget's layer (Z-order).
	 * @param Options Additional options.
	 * @param ExperimentalOptions ExperimentalOptions Experimental options.
	 */
	UFUNCTION(BlueprintCallable, Category = "Window Manager", meta = (Keywords = "WM WindowManager UI Widget", AutoCreateRefTerm = "WidgetStorage"))
	static void CreateWidgetWM(
		UPARAM(ref) TArray<FWidgetStorageWM>& WidgetStorage,
		APlayerController* OwningController,
		TSubclassOf<UUserWidget> WidgetClass,
		FGameplayTag Tag,
		int32 ZOrder,
		FOptionsWM Options,
		FExpOptionsWM ExperimentalOptions);

	/**
	 * Removal of the target widget.
	 * @param WidgetStorage Array reference. A connectable data array (connection may be absent) that contains a reference to the widget and its tag.
	 * @param OwningController The player controller.
	 * @param WidgetClass The target widget.
	 * @param bSmartCursor If all widgets are closed, the cursor is disabled regardless of the settings.
	 * @param Options Additional options.
	 */
	UFUNCTION(BlueprintCallable, Category = "Window Manager", meta = (Keywords = "WM WindowManager UI Widget", AutoCreateRefTerm = "WidgetStorage"))
	static void RemoveWidgetWM(
		UPARAM(ref) TArray<FWidgetStorageWM>& WidgetStorage,
		APlayerController* OwningController,
		TSubclassOf<UUserWidget> WidgetClass,
		bool bSmartCursor,
		FOptionsWM Options
	);

	/**
	 * Multiple widget removal. Warning: Deleting widgets without using tags will delete widgets globally (watch out for connected widget arrays).
	 * @param WidgetStorage Auto overwritten array. A connectable data array (connection may be absent) that contains a reference to the widget and its tag.
	 * @param OwningController The player controller.
	 * @param bRemoveByTag Enables the ability to remove groups of widgets by tag. If set to false, all widgets are removed globally.
	 * @param Tags Widgets with the specified tags will be removed.
	 * @param bSmartCursor If all widgets are closed, the cursor is disabled regardless of the settings.
	 * @param Options Additional options.
	 */
	UFUNCTION(BlueprintCallable, Category = "Window Manager", meta = (Keywords = "WM WindowManager UI Widget", AutoCreateRefTerm = "WidgetStorage"))
	static void RemoveAllWidgetsWM(
		UPARAM(ref) TArray<FWidgetStorageWM>& WidgetStorage,
		APlayerController* OwningController,
		bool bRemoveByTag,
		FGameplayTagContainer Tags,
		bool bSmartCursor,
		FOptionsWM Options
	);

	/**
	 * Adding a child widget to a container.
	 * @param OwningController The player controller.
	 * @param ChildContainer The container into which the child widget will be placed.
	 * @param WidgetClass The widget that will be placed in the container.
	 * @param bReplace Clear the container before adding the widget.
	 */
	UFUNCTION(BlueprintCallable, Category = "Window Manager", meta = (Keywords = "WM WindowManager UI Widget"))
	static void CreateChildWidgetWM(
		APlayerController* OwningController,
		UPanelWidget* ChildContainer,
		TSubclassOf<UUserWidget> WidgetClass,
		bool bReplace
	);

private:
	/**
	 * Save information about the added widget to the storage
	 * @param WidgetStorage A reference to an array of data.
	 * @param WidgetClass The target widget.
	 * @param Tag The identifying tag of the widget.
	 */
	UFUNCTION()
	static void AddWidgetToArray(TArray<FWidgetStorageWM>& WidgetStorage, UUserWidget* WidgetClass, FGameplayTag& Tag);

	/**
	 * Using additional options.
	 * @param OwningController The player controller.
	 * @param TargetWidget The target widget.
	 * @param Options Additional options. Expand the options structure for detailed description.
	 */
	UFUNCTION()
	static void ApplyOptions(APlayerController* OwningController, UUserWidget* TargetWidget, FOptionsWM& Options);

	/**
	 * Find the index of the saved widget in the array by the specified class.
	 * @param OwningController The player controller.
	 * @param WidgetStorage A reference to an array of data.
	 * @param WidgetClass The target widget.
	 */
	UFUNCTION()
	static int32 FindWidgetIndex(APlayerController* OwningController, TArray<FWidgetStorageWM>& WidgetStorage, TSubclassOf<UUserWidget>& WidgetClass);

	/**
	 * Find the reference of the saved widget in the array by the specified class.
	 * @param OwningController The player controller.
	 * @param WidgetStorage A reference to an array of data.
	 * @param WidgetClass The target widget.
	 * @param bIsUsedWidgetStorageWM Use a pluggable data array to include its contents in the search.
	 * @param bFindAnyWidget Enables searching for the first found widget to check if any widget has been created.
	 */
	UFUNCTION()
	static UUserWidget* FindWidgetReference(APlayerController* OwningController, TArray<FWidgetStorageWM>& WidgetStorage, TSubclassOf<UUserWidget> WidgetClass, bool bIsUsedWidgetStorageWM = false, bool bFindAnyWidget = false);

};

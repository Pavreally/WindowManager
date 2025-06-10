![Window Manager](./_Misc/Preview.png)

# Window Manager
`BETA`<br><br>
A plugin that includes a C++ library of universal widget management functions to help you quickly build interface logic using UMG in Unreal Engine 5.

<br>

> [!NOTE]
> The plugin has been pre-packaged only for Win64 and Android.

# Latest Updates
`Global update`
`Version 1.6.1`
- The plugin has been redesigned and updated for Unreal Engine 5.6.
- The main function is now more flexible — it no longer requires memory allocation or data arrays. You can even mix different usage approaches.
- A major C++ refactor was done: the Blueprint-based version was removed, the code was broken down into smaller components, and one universal function was split into four parts for clarity. All optional settings have been grouped into a convenient struct.
- No more need to manually update arrays — the function now automatically manages the linked data array, handling both saving and removal of entries.
- Widget tags are now supported (when using a data array). This allows you to remove groups of widgets by tag all at once.
- Targeted widget removal can now work without a data array — just place the removal function wherever needed and specify the widget class.
- All function options are now optional and can be enabled or disabled at any time.
- Several new experimental features have been added:
1. SmartCursor – automatically hides the cursor when all widgets are closed and none remain on screen.
2. NoOtherWidgetsInArray – prevents a widget from opening if any other active widgets are present in the data array (works only with arrays).
3. NoOtherWidgetsGlobal – prevents a widget from opening if any other widget exists globally (This option will work in any case).
- Improved keyword search logic.
- Major improvements to code documentation.
<br>
Fixes:
<br>
- Fixed a compilation warning in accordance with the updated code maintenance rules in Unreal Engine 5. 
- Fixed log warning about non-existent widget after attempting to delete it.

## What it's for
- Easy and fast setup of the widget management system.
- Simplified and minimized amount of code to add.
- Simultaneous configuration of cursor behavior, focus, and the restriction on creating new widgets.

## Features
- Very fast setup of user interface logic in Unreal Engine!
- Versatile functions for managing widgets can operate in multiple modes — both the standard Unreal Engine mode and with an optional external storage system for greater control.
- Remove specific widgets from almost anywhere in your project.
- Support for multi-layered windows with multiple groups.
- Automatic blocking of new widgets if more important ones have already been added.
- Open windows with automatic replacement of existing ones.
- Quickly add child windows to a container with optional content replacement.
- Mass close widget groups by a specified tag.

## Install

> [!NOTE]
> Starting with Unreal Engine version 5.6, it is recommended to use the new project type based on C++. Otherwise, when creating a new Blueprint project, you may encounter the following error: "Engine modules are out of date, and cannot be compiled while the engine is running. Please build through your IDE." This issue does not occur in a pure C++ project.

1. Make sure the Unreal Engine editor is closed.
2. Move the "Plugins" folder to the root folder of your created project.
3. Run your project to which the "Plugins" folder with WM was added. If a message about restoring the module appears, select "Yes".
4. Done! The Window Manager folders should appear in the Unreal Engine browser and the plugin should be automatically activated. If the plugin folder is not visible, activate visibility through the browser settings: `Settings > Show Plugin Content`.
5. Run the `L_WindowManager_Demo` test level, which is located in the Window Manager Content folder with the test files, and verify that the function works in C++.

## How to use it?
An interactive step-by-step tutorial on how to use WM can be found in the file: `BP_WM_PlayerController_Demo`, which is located at the path `Plugins\Window Manager Content\DemoFiles\BP\`.

![Window Manager](./_Misc/Tutorial/Tutorial_1.jpg)
![Window Manager](./_Misc/Tutorial/Tutorial_2.jpg)
![Window Manager](./_Misc/Tutorial/Tutorial_3.jpg)
![Window Manager](./_Misc/Tutorial/Tutorial_4.jpg)

## (C++) Documentaion
All sources contain self-documenting code.

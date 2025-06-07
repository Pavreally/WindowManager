![Window Manager](./_Misc/Preview.png)

# Window Manager
`BETA`<br><br>
A plugin that includes a C++ library of universal widget management functions to help you quickly build interface logic using UMG in Unreal Engine 5.

<br>

> [!NOTE]
> The plugin has been pre-packaged only for Win64 and Android.

# Latest Updates
`Global update`
`Version 1.6`
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
3. NoOtherWidgetsGlobal – prevents a widget from opening if any other widget exists globally (always active).
- Improved keyword search logic.
- Major improvements to code documentation.

## What it's for
- Control important windows so that they are not blocked by anything.
- Simplify the logic on how to customize composite windows.
- Control the focus of windows without adding repetitive logic.
- Control the cursor display at the same time.
- Easily add child windows to the parent widget container.

## Features
- Extremely fast setup for UI logic in Unreal Engine!
- Ability to use functions without memory allocation or data arrays.
- Support for multi-level (nested) windows.
- Open windows with automatic replacement of existing ones.
- A priority window system that blocks less important UI windows from opening.
- Remove specific widgets from virtually anywhere in your project.
- Support for adding child windows to existing ones.
- Close widgets by tag, allowing grouped widget management.

## Install
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

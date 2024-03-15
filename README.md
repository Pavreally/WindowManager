![Window Manager](./_Misc/Preview.png)

# Window Manager
This is a versatile feature that can help in quickly customizing the interface (UMG) logic in Unreal Engine 5. 
Blueprints and C++ versions will allow you to apply the most appropriate and convenient way of integration into your projects.

# Latest Updates
`Version 1.1`
- Code refactoring. Simplification of logic and data array.
- A function in C++ has been created. A shared variable was created, for interchangeability of BP and C++ version functions.
- Child widgets in containers are now not indexed in memory, for more flexibility.
- Fixed cursor visibility disappearing after closing the widget if multiple windows are open in a row.
- Controller support

## What it's for:
- Control important windows so that they are not blocked by anything
- Simplify the logic on how to customize composite windows
- Control the focus of windows without adding repetitive logic
- Control the cursor display at the same time
- Easily add child windows to the parent widget container

## Features:
- Very fast UI logic customization in Unreal Engine!
- Multi-level window support
- Opening windows with replacement
- System for important windows (blocks opening of insignificant UI windows)
- Support adding child windows without replacement
- Support adding child windows with target content replacement

## Install:
You must first install the C++ version, as it contains the necessary data structure.

### Install C++ Version WM
1. Make sure you have a compiled C++ project in Unreal Engine and close UE Editor.
2. Copy the contents of "WindowManager_Version_CPP" with the WM function library to your C++ folder. Default: "/YourProject/Source/YourProjectName/".
3. Open the file in this folder file "YourProjectName.Build.cs" and add "UMG" to the "PublicDependencyModuleNames" line.
Example: `PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });`
4. Start compiling your project with the UE Editor closed. If errors appear, open the UE Editor and refresh the project files using the item in the top menu: "Tools > Refresh Visual Studio Code Project", then close the editor and repeat the compilation.
5. Done! Now you can install the files with the examples and the demo level. 

(Next: Install Test Level and BP Version)

### Install Test Level and BP Version
1. Move the "FeaturePacks" and "Samples" folders to the root folder with installed UE v.5.3
2. Open the UE Editor and add the WM content files to your UE project
3. In the UE browser, find the file: "BP_PlayerController_WindowManager_demo"
4. If the structure reference breaks, recreate the array. By default, the structure is called "Widgets Parent". If you are going to modify the WM function with Blueprint, you should also fix all references in the "BP_WindowManager_Library" file or just delete it.
5. Now you can open the "L_WM_PreviewMap" level and test the operation of the window manager.

## How to use it?: 
An interactive step-by-step tutorial on how to use WM can be found in the file: `BP_PlayerController_WindowManager_demo`.

![Window Manager](./_Misc/Tutorial/Tutorial_1.jpg)
![Window Manager](./_Misc/Tutorial/Tutorial_2.jpg)

## (C++) Documentaion:
All sources contain self-documenting code. C++ and BP functions are completely identical and interchangeable.

Description of the main parameters of the main function:
`ArrayWindowsActive` - stores active widgets and their names
`bWindowsOpened` - stores information about at least one open widget with WM
`bActionOpen` - specifies the action of adding the widget (true) or closing it (false)
`bActionCloseAll` - closes all created widgets
`WidgetClass` - target widget
`OwningController` - owning player controller
`bIsReplaced` - whether the last open widget will be replaced by a new one
`bShowCursor` - show cursor
`bFocusViewport` - keeps focus on the viewport
`ZOrder` - adjusts the display priority of the widget layer
`ChildContainer` - target container where the widget will be embedded (can be empty)
`ReturnWindowsActive` - returns an updated list of open widgets
`bReturnWindowsOpened` - returns information about at least one open widget with WM

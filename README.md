# game_services_client-UE4
Unreal 4 client for game services umbrella project

## Game Services Project
* https://github.com/Alezrik/game_services_umbrella

## Setup
* right click on .uproject file and build project files
* open .uproject file for UE4 editor or .sln for Visual Studio 2017

## Packaging and Release Example (local - WIn64)
* open .snl project in Visual Studio
* Build 'Development Editor' Configuration of UnrealDemo in Visual Studio
* Build 'Development Server' Configuration of UnrealDemo in Visual Studio
* Execute 'Development Editor' in Visual Studio
* Go to File -> PackageProject and Package for Win64 and Package the project into a new directory
* From the Project/Binaries directory copy the UnrealDemoServer.exe into the new Package directory at Package\WindowsNoEditor\UnrealDemo\Binaries\Win64 directory
* Create a shortcut to the Server.exe in the Package and append the target with -log parameter
* Start Server via Shortcut
* Start Client
* Clients can see each other
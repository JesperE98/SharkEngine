# SharkEngine 3D Platformer

## Building from source

SharkEngine uses CMake. GLFW is downloaded automatically the first time you configure,
so you need an internet connection for the first build.

### Requirements

- Visual Studio 2022 or 2026 with the **Desktop development with C++** workload (includes CMake)
- Or: CMake 3.24+ and a C++20 compiler

### Visual Studio (Open Folder, recommended)

1. Clone: `git clone https://github.com/JesperE98/SharkEngine/`
2. In Visual Studio: **File > Open > Folder...** and pick the `SharkEngine` folder.
3. Choose the `Ninja Debug` or `Ninja Release` configuration in the toolbar.
4. Choose `Editor.exe` as the startup item, then press F5.

### CLion

Works with both the bundled MinGW (GCC) toolchain and the Visual Studio (MSVC) toolchain.

1. Open the `SharkEngine` folder. CLion loads `CMakeLists.txt` automatically.
2. Pick the **Editor** run configuration and press Run/Debug.

Don't use "Run" on single files (like `.cmake` files) - always run the **Editor** configuration.

To build with MSVC instead: add a **Visual Studio** toolchain (architecture `amd64`) under
**Settings > Build, Execution, Deployment > Toolchains**, then enable the `ninja-debug` / `ninja-release`
preset profiles under **Settings > Build, Execution, Deployment > CMake**.

### Generate a Visual Studio solution

```bash
cmake --preset vs2026
```

Then open `build/vs2026/SharkEngine.sln` (Editor is already the startup project). Use the `vs2022` preset for Visual Studio 2022.

### Command line

```bash
cmake --preset vs2026
cmake --build --preset vs2026-release
```

### Output

`Editor.exe` is placed in `Engine/Binaries/x64/<Config>/` (MSVC) or `Engine/Binaries/x64-gnu/<Config>/` (MinGW/GCC). The engine looks for the `Content/` and
`Shaders/` folders by searching upwards from its working directory, so run it from inside the `Engine` folder.

### Third party libraries

| Library | Version | How it's included |
|---|---|---|
| GLFW | 3.4 | Downloaded by CMake (FetchContent). Set `-DSHARK_USE_SYSTEM_GLFW=ON` to use an installed one |
| glad | 0.1.36 (OpenGL 3.3 core) | `Engine/ThirdParty/glad` |
| Dear ImGui (docking) | 1.92.5 | `Engine/ThirdParty/ImGui` |
| glm | 1.0.1 | `Engine/ThirdParty/glm` (header only) |
| nlohmann/json | 3.12.0 | `Engine/ThirdParty/nlohmann` (header only) |
| stb_image | 2.30 | `Engine/ThirdParty/stb` |

## Or run the prebuilt binary

1. Download the latest release ZIP from [https://github.com/JesperE98/SharkEngine].
2. Extract anywhere.
3. Double-click `Editor.exe` located at "..\SharkEngine\Engine\Binaries\x64\Debug\Editor.exe".

## How to play

- WASD: move
- Space: Jump
- Walk into the door to complete a level
- Falling off the world respawns you

## Game flow

1. Launch -> main menu appears.
2. Click Level 1 to start.
3. Reach the door to complete the level.
4. Best times are saved automatically.

## What's in the game

- 3 levels with platforming challenges
- 1 level with an enemy or enemies
- 1 level with terrain
- Persistent record times

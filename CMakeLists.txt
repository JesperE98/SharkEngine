cmake_minimum_required(VERSION 3.24)

project(SharkEngine
    VERSION 0.1.0
    DESCRIPTION "SharkEngine - a small OpenGL game engine with editor"
    LANGUAGES C CXX
)

# --------------------------------------------------------------------------
# Options
# --------------------------------------------------------------------------
option(SHARK_USE_SYSTEM_GLFW "Use an installed GLFW (find_package) instead of downloading it" OFF)
option(SHARK_ENABLE_IPO      "Enable link-time optimization (LTO/LTCG) for Release builds" ON)

# --------------------------------------------------------------------------
# Global settings
# --------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Show folders (Engine / Editor / ThirdParty) in IDEs like Visual Studio.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Paths used throughout the build.
set(SHARK_ENGINE_DIR     "${CMAKE_SOURCE_DIR}/Engine")
set(SHARK_SOURCE_DIR     "${SHARK_ENGINE_DIR}/Source")
set(SHARK_THIRDPARTY_DIR "${SHARK_ENGINE_DIR}/ThirdParty")

# Executables go to Engine/Binaries/<arch>/<Config>. The engine finds its
# Content/ and Shaders/ folders by searching upwards from the working
# directory, so the exe must live inside Engine/.
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SHARK_ARCH x64)
else()
    set(SHARK_ARCH x86)
endif()
# Keep MSVC at x64/<Config>; other compilers get their own folder (e.g. x64-gnu)
# so builds from different toolchains don't overwrite each other.
if(NOT MSVC)
    string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" _compiler_id)
    string(APPEND SHARK_ARCH "-${_compiler_id}")
endif()
set(SHARK_BINARIES_DIR "${SHARK_ENGINE_DIR}/Binaries/${SHARK_ARCH}/$<CONFIG>")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(SharkCompileOptions)

find_package(OpenGL REQUIRED)

# --------------------------------------------------------------------------
# Projects
# --------------------------------------------------------------------------
add_subdirectory(Engine/ThirdParty)
add_subdirectory(Engine/Source/Runtime)
add_subdirectory(Engine/Source/Editor)

# Make Editor the startup project when generating a Visual Studio solution.
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)

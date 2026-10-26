# SharkEngine runtime (static library).

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR
        "This folder is part of SharkEngine and can't be built on its own. "
        "Open the repository root (the folder with the top-level CMakeLists.txt) instead.")
endif()

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)
shark_exclude_build_dirs(ENGINE_SOURCES)

# Shaders are not compiled, but listed so they show up in the IDE.
file(GLOB ENGINE_SHADERS CONFIGURE_DEPENDS "${SHARK_ENGINE_DIR}/Shaders/*.glsl")
set_source_files_properties(${ENGINE_SHADERS} PROPERTIES HEADER_FILE_ONLY ON)

add_library(Engine STATIC ${ENGINE_SOURCES} ${ENGINE_SHADERS})
add_library(Shark::Engine ALIAS Engine)

# Engine headers are included relative to this folder, e.g. <Core/GameObject.h>.
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(Engine
    PUBLIC
        glad
        glfw
        imgui
        glm::glm
        nlohmann_json::nlohmann_json
        OpenGL::GL
    PRIVATE
        stb
)

if(WIN32)
    # FileDialog.cpp uses the Win32 common dialogs.
    target_link_libraries(Engine PRIVATE comdlg32)
endif()

shark_target_options(Engine)

set_target_properties(Engine PROPERTIES FOLDER "Engine")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ENGINE_SOURCES})
source_group("Shaders" FILES ${ENGINE_SHADERS})

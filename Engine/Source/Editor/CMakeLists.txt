# SharkEngine editor (executable).

file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(Editor ${EDITOR_SOURCES})

target_include_directories(Editor PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(Editor PRIVATE Shark::Engine)

shark_target_options(Editor)

set_target_properties(Editor PROPERTIES
    FOLDER "Engine"
    # Put Editor.exe in Engine/Binaries/<arch>/<Config> (see root CMakeLists.txt).
    RUNTIME_OUTPUT_DIRECTORY "${SHARK_BINARIES_DIR}"
    # Run from the Engine folder when debugging in Visual Studio, so
    # Content/, Shaders/ and imgui.ini are found right away.
    VS_DEBUGGER_WORKING_DIRECTORY "${SHARK_ENGINE_DIR}"
)

if(MINGW)
    # Link the GCC runtime statically so Editor.exe starts without
    # libstdc++-6.dll / libgcc_s_seh-1.dll on PATH. (A fully static "-static"
    # link fails with some MinGW builds, e.g. the one bundled with CLion.)
    target_link_options(Editor PRIVATE -static-libgcc -static-libstdc++)

    # libwinpthread can't be linked statically that way, so copy its DLL next to the exe.
    get_filename_component(_mingw_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
    if(EXISTS "${_mingw_bin}/libwinpthread-1.dll")
        add_custom_command(TARGET Editor POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${_mingw_bin}/libwinpthread-1.dll" "$<TARGET_FILE_DIR:Editor>"
        )
    endif()
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_SOURCES})

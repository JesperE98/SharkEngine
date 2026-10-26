# Shared compiler settings for SharkEngine targets.

include(CheckIPOSupported)

if(SHARK_ENABLE_IPO)
    check_ipo_supported(RESULT SHARK_IPO_SUPPORTED OUTPUT _ipo_output LANGUAGES C CXX)
    if(NOT SHARK_IPO_SUPPORTED)
        message(STATUS "SharkEngine: link-time optimization not supported by this compiler, skipping")
    endif()
endif()

# Removes files inside IDE/CMake build folders (cmake-build-*, CMakeFiles, ...)
# from a globbed source list. An IDE that once opened a subfolder as its own
# project can leave such folders behind, and their generated .cpp files
# (e.g. CMakeCXXCompilerId.cpp with its own main()) would otherwise be compiled.
function(shark_exclude_build_dirs list_var)
    set(_kept "")
    foreach(_file IN LISTS ${list_var})
        # Only look at the part below the current folder, so the location of
        # the checkout itself (e.g. C:/build/SharkEngine) doesn't matter.
        file(RELATIVE_PATH _rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_file}")
        if(NOT _rel MATCHES "(^|/)(cmake-build-[^/]*|CMakeFiles|build|out)/")
            list(APPEND _kept "${_file}")
        endif()
    endforeach()
    set(${list_var} ${_kept} PARENT_SCOPE)
endfunction()

# Settings that apply to every target we build (our own code and third party).
function(shark_common_options target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /utf-8   # Source files are UTF-8
            /MP      # Parallel compilation inside a project (Visual Studio generator)
        )
    endif()

    if(SHARK_ENABLE_IPO AND SHARK_IPO_SUPPORTED)
        set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    endif()
endfunction()

# Settings for SharkEngine's own targets (Engine, Editor).
function(shark_target_options target)
    shark_common_options(${target})

    target_compile_features(${target} PUBLIC cxx_std_20)

    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W3
            /permissive-   # Conformance mode
            /sdl           # Additional security checks
        )
    else()
        target_compile_options(${target} PRIVATE -Wall)
    endif()

    if(WIN32)
        # Matches the old "Use Unicode Character Set" project setting.
        target_compile_definitions(${target} PRIVATE UNICODE _UNICODE)
    endif()
endfunction()

# Settings for vendored third party libraries.
function(shark_third_party_options target)
    shark_common_options(${target})
    set_target_properties(${target} PROPERTIES FOLDER "ThirdParty")
endfunction()

# Third party libraries used by SharkEngine.
#
# Every library is exposed as a CMake target so Engine/Editor only need to
# link against it. Headers are included as <Lib/header.h> relative to this
# folder (e.g. <ImGui/imgui.h>, <stb/stb_image.h>), so this folder is the
# include root for the vendored libraries.
#
#   Target                          Version   Source
#   glad                            0.1.36    vendored (OpenGL 3.3 core loader)
#   glfw                            3.4       downloaded with FetchContent
#   imgui                           1.92.5    vendored (docking branch, GLFW + OpenGL3 backends)
#   glm::glm                        1.0.1     vendored, header only
#   nlohmann_json::nlohmann_json    3.12.0    vendored, header only
#   stb                             2.30      vendored (stb_image)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# --------------------------------------------------------------------------
# GLFW (window, input, OpenGL context)
# --------------------------------------------------------------------------
if(SHARK_USE_SYSTEM_GLFW)
    find_package(glfw3 3.4 REQUIRED)
else()
    include(FetchContent)

    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(glfw
        URL      https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
        URL_HASH SHA256=b5ec004b2712fd08e8861dc271428f048775200a2df719ccf575143ba749a3e9
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    FetchContent_MakeAvailable(glfw)

    set_target_properties(glfw PROPERTIES FOLDER "ThirdParty/GLFW")
    if(TARGET update_mappings)
        set_target_properties(update_mappings PROPERTIES FOLDER "ThirdParty/GLFW")
    endif()
endif()

# --------------------------------------------------------------------------
# glad (OpenGL function loader)
# --------------------------------------------------------------------------
add_library(glad STATIC
    glad/glad.c
    glad/glad/glad.h
    glad/KHR/khrplatform.h
)
target_include_directories(glad SYSTEM PUBLIC "${THIRDPARTY_DIR}/glad")
target_link_libraries(glad PUBLIC OpenGL::GL ${CMAKE_DL_LIBS})
shark_third_party_options(glad)

# --------------------------------------------------------------------------
# Dear ImGui (+ GLFW and OpenGL3 backends)
# --------------------------------------------------------------------------
add_library(imgui STATIC
    ImGui/imgui.cpp
    ImGui/imgui_demo.cpp
    ImGui/imgui_draw.cpp
    ImGui/imgui_tables.cpp
    ImGui/imgui_widgets.cpp
    ImGui/imgui_impl_glfw.cpp
    ImGui/imgui_impl_opengl3.cpp

    ImGui/imconfig.h
    ImGui/imgui.h
    ImGui/imgui_internal.h
    ImGui/imgui_impl_glfw.h
    ImGui/imgui_impl_opengl3.h
    ImGui/imgui_impl_opengl3_loader.h
    ImGui/imstb_rectpack.h
    ImGui/imstb_textedit.h
    ImGui/imstb_truetype.h
)
target_include_directories(imgui SYSTEM PUBLIC
    "${THIRDPARTY_DIR}"          # <ImGui/imgui.h>
    "${THIRDPARTY_DIR}/ImGui"    # "imgui.h" from inside the backends
)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
target_compile_features(imgui PUBLIC cxx_std_11)
shark_third_party_options(imgui)

# --------------------------------------------------------------------------
# stb_image (image loading). The implementation is compiled once here.
# --------------------------------------------------------------------------
add_library(stb STATIC
    stb/stb_image.c
    stb/stb_image.h
)
target_include_directories(stb SYSTEM PUBLIC "${THIRDPARTY_DIR}")   # <stb/stb_image.h>
shark_third_party_options(stb)

# --------------------------------------------------------------------------
# glm (math, header only)
# --------------------------------------------------------------------------
add_library(glm INTERFACE)
add_library(glm::glm ALIAS glm)
target_include_directories(glm SYSTEM INTERFACE "${THIRDPARTY_DIR}")   # <glm/glm.hpp>

# --------------------------------------------------------------------------
# nlohmann/json (JSON, header only)
# --------------------------------------------------------------------------
add_library(nlohmann_json INTERFACE)
add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
target_include_directories(nlohmann_json SYSTEM INTERFACE "${THIRDPARTY_DIR}")   # <nlohmann/json.hpp>

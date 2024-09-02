
cmake_minimum_required(VERSION 3.16)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# vpkg fix
#set(VCPKG_TARGET_TRIPLET x64-mingw-static CACHE STRING "")
#set(VCPKG_FEATURE_FLAGS -binarycaching CACHE STRING "")

# if you haven't included it already
# definitions of CMAKE_INSTALL_LIBDIR, CMAKE_INSTALL_INCLUDEDIR and others
include(GNUInstallDirs)

set(texgen_VERSION 1.0.1)

# set the project name and version
project(TexGen  
    VERSION ${texgen_VERSION}
    DESCRIPTION "Texture Map Generator using vector graphics."
    HOMEPAGE_URL "https://github.com/manisonik/texture-map-gen"
    LANGUAGES CXX
)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
add_compile_definitions(IL_STATIC_LIB)

add_executable(TexGen WIN32
    main.cpp 
    stdafx.cpp
    msk-glm-ray.cpp
    msk-glm-mousepicker.cpp
    msk-glm-camera.cpp
    msk-gl-textureregion.cpp
    msk-gl-texturedata.cpp
    msk-gl-texture.cpp
    msk-gl-text.cpp
    msk-gl-text-renderer.cpp
    msk-gl-stencil-renderer.cpp
    msk-gl-spritebatch.cpp
    msk-gl-shaderprogram.cpp
    msk-gl-shader.cpp
    msk-gl-scene.cpp
    msk-gl-renderer.cpp
    msk-gl-multisampleframerenderer.cpp
    msk-gl-mesh.cpp
    msk-gl-mesh-builder.cpp
    msk-gl-material.cpp
    msk-gl-main-scene.cpp
    msk-gl-line-renderer.cpp
    msk-gl-fragment.cpp
    msk-gl-font.cpp
    msk-core.cpp
    msk-app.cpp
    msk-object.cpp
    msk-glfw-window.cpp
    msk-win32-window.cpp
)

target_precompile_headers(TexGen PRIVATE stdafx.h)
#target_link_options(TexGen PRIVATE "LINKER:--verbose")

# Opengl
find_package(OpenGL REQUIRED)
target_link_libraries(TexGen PRIVATE ${OPENGL_LIBRARIES})
target_include_directories(TexGen PRIVATE ${OPENGL_INCLUDE_DIR})

# Cairo
find_package(PkgConfig REQUIRED)
pkg_check_modules(cairo REQUIRED IMPORTED_TARGET cairo)
target_link_libraries(TexGen PRIVATE PkgConfig::cairo)

# DevIL
find_package(PkgConfig REQUIRED)
pkg_check_modules(DevIL REQUIRED IMPORTED_TARGET DevIL)
target_link_libraries(TexGen PRIVATE PkgConfig::DevIL)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ILU REQUIRED IMPORTED_TARGET ILU)
target_link_libraries(TexGen PRIVATE PkgConfig::ILU)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ILUT REQUIRED IMPORTED_TARGET ILUT)
target_link_libraries(TexGen PRIVATE PkgConfig::ILUT)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libtiff-4 REQUIRED IMPORTED_TARGET libtiff-4)
target_link_libraries(TexGen PRIVATE PkgConfig::libtiff-4)

# GLM
find_package(glm CONFIG REQUIRED)
target_link_libraries(TexGen PRIVATE glm::glm)

# GLEW
find_package(GLEW REQUIRED)
target_link_libraries(TexGen PRIVATE GLEW::GLEW)

# FreeType
find_package(Freetype REQUIRED)
target_link_libraries(TexGen PRIVATE Freetype::Freetype)

# PNG
find_package(PNG REQUIRED)
target_link_libraries(TexGen PRIVATE PNG::PNG)

# Eigen3
find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(TexGen PRIVATE Eigen3::Eigen)

# DCEL
find_package(dcel CONFIG REQUIRED)
target_link_libraries(TexGen PRIVATE manisonik::dcel)

# Spdlog
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(TexGen PRIVATE spdlog::spdlog)

# GLFW3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(TexGen PRIVATE glfw)

# Move resources
add_custom_command(
        TARGET TexGen POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/resources/ui_icons.png
                ${CMAKE_CURRENT_BINARY_DIR}/ui_icons.png)
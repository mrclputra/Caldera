cmake_minimum_required(VERSION 3.19)
project(Caldera LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGLWidgets)

qt_standard_project_setup()

if (MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif ()

set(SPDLOG_BUILD_COMPILED ON CACHE BOOL "" FORCE)
add_subdirectory(external/spdlog)
add_subdirectory(external/glm)

set(BUILD_SHARED_LIBS                       OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT   OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_PLY_IMPORTER               ON  CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER              ON  CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS                      OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS               OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL                          OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX             OFF CACHE BOOL "" FORCE)
add_subdirectory(external/assimp)

add_library(CalderaCore
        STATIC
        src/core/engine/Renderer.h
        src/core/engine/Renderer.cpp
        src/core/engine/components/Shader.h
        src/core/engine/components/Shader.cpp
        src/core/engine/components/Object.cpp
        src/core/engine/components/Object.h
        src/core/engine/Scene.cpp
        src/core/engine/Scene.h
        src/core/engine/Camera.cpp
        src/core/engine/Camera.h
        src/core/engine/Gizmos.cpp
        src/core/engine/Gizmos.h
        src/core/engine/components/BoundingBox.h
        src/core/engine/components/PointCloud.cpp
        src/core/engine/components/PointCloud.h
        src/core/utilities/ModelLoader.cpp
        src/core/utilities/ModelLoader.h
)

target_link_libraries(CalderaCore
        PUBLIC
        spdlog::spdlog
        Qt::OpenGL
        glm::glm
        assimp
)
target_include_directories(CalderaCore
        PUBLIC
        src/core
        external/stb
        external/assimp/include
)
target_compile_definitions(CalderaCore
        PRIVATE
        SHADER_DIR="${PROJECT_SOURCE_DIR}/src/core/engine/shaders/"
)

add_library(CalderaGUI
        STATIC
        src/gui/App.h
        src/gui/App.cpp
        src/gui/MainWindow.h
        src/gui/MainWindow.cpp
        src/gui/MainWindow.ui
        src/gui/ViewportWidget.h
        src/gui/ViewportWidget.cpp
)

target_link_libraries(CalderaGUI
        PUBLIC
        CalderaCore
        Qt::Core
        Qt::Widgets
        Qt::OpenGLWidgets
)
target_include_directories(CalderaGUI
        PUBLIC
        src/gui
)

qt_add_executable(${PROJECT_NAME}
        MACOSX_BUNDLE
        src/main.cpp
)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug>>
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        CalderaGUI
)

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
        TARGET ${PROJECT_NAME}
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

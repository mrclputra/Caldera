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
        src/core/engine/components/PointCloud.cpp
        src/core/engine/components/PointCloud.h
)

target_link_libraries(CalderaCore
        PUBLIC
        spdlog::spdlog
        Qt::OpenGL
        glm::glm
)
target_include_directories(CalderaCore
        PUBLIC
        src/core
        external/stb
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
        src/core/engine/components/Transform.h
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

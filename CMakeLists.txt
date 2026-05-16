cmake_minimum_required(VERSION 3.19)
project(Caldera LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGLWidgets)

qt_standard_project_setup()

if(MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif()

set(SPDLOG_BUILD_COMPILED ON CACHE BOOL "" FORCE)
add_subdirectory(external/spdlog)

# core
file(GLOB_RECURSE CORE_SRCS CONFIGURE_DEPENDS src/core/*.cpp src/core/*.h)
add_library(CalderaCore STATIC ${CORE_SRCS})
target_link_libraries(CalderaCore PUBLIC spdlog::spdlog Qt::OpenGL)
target_include_directories(CalderaCore PUBLIC src/core)

# gui
file(GLOB_RECURSE GUI_SRCS CONFIGURE_DEPENDS src/gui/*.cpp src/gui/*.h src/gui/*.ui)
add_library(CalderaGUI STATIC ${GUI_SRCS})
target_link_libraries(CalderaGUI PUBLIC CalderaCore Qt::Core Qt::Widgets Qt::OpenGLWidgets)
target_include_directories(CalderaGUI PUBLIC src/gui)

# entry point
qt_add_executable(${PROJECT_NAME}
    WIN32 MACOSX_BUNDLE
    src/main.cpp
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        CalderaGUI
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

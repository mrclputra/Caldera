cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 17)

project(Caldera)

set(SPDLOG_BUILD_COMPILED ON CACHE BOOL "" FORCE)
add_subdirectory(external/spdlog)

file(GLOB SRCS src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
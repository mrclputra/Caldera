cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 17)

project(Caldera)

file(GLOB SRCS src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SRCS})
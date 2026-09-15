cmake_minimum_required(VERSION 3.20)

project(ShiftPlanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Svg)

option(BUILD_TESTS "Build unit tests" OFF)

add_subdirectory(src)
add_subdirectory(app)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
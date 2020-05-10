cmake_minimum_required(VERSION 3.9)

project(tcpping VERSION 1.0.1 LANGUAGES CXX)
message("configure ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES main.cpp tcpping.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
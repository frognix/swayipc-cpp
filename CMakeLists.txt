cmake_minimum_required(VERSION 3.5)

project(term-runner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CPP src/*)
file(GLOB HPP include/*)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(${PROJECT_NAME} ${CPP})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
include_directories(${PROJECT_NAME} include)
target_link_libraries(term-runner PRIVATE nlohmann_json::nlohmann_json)

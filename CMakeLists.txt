cmake_minimum_required(VERSION 3.16)
set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "" FORCE)

project(physics_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SFML
    URL https://github.com/SFML/SFML/archive/refs/tags/3.0.0.zip
)

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(physics_engine ${PROJECT_SOURCES})

target_include_directories(physics_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(physics_engine PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

cmake_minimum_required(VERSION 3.16)
set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "" FORCE)

project(particle_collision LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(particle_collision ${PROJECT_SOURCES})

target_include_directories(particle_collision PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(particle_collision PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)
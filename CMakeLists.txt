cmake_minimum_required(VERSION 3.15)

project(MAIN_PROJ LANGUAGES CXX)

add_executable(MAIN src/main.cpp)

add_subdirectory(src/GLGE_Core)

if(MSVC)
    target_link_options(MAIN PRIVATE "/WHOLEARCHIVE:GLGE_CORE")
else()
    target_link_options(MAIN PRIVATE "-Wl,--whole-archive" "-lGLGE_CORE" "-Wl,--no-whole-archive")
endif()

# Apply to all targets in the project
if(MSVC)
    # Use dynamic runtime: /MD (Release), /MDd (Debug)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

set_target_properties(MAIN PROPERTIES 
                        CXX_STANDARD 23 
                        CXX_STANDARD_REQUIRED ON)
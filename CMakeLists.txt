cmake_minimum_required(VERSION 3.15)

project(MAIN_PROJ LANGUAGES CXX)

add_executable(MAIN src/main.cpp)

set_target_properties(MAIN PROPERTIES 
                        CXX_STANDARD 23 
                        CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/GLGEGraphic)
target_link_libraries(MAIN PUBLIC GLGE_GRAPHIC)

# Apply to all targets in the project
if(MSVC)
   # Use dynamic runtime: /MD (Release), /MDd (Debug)
   set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()
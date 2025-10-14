cmake_minimum_required(VERSION 3.10)

set(SRC 
    src/main.cpp
)

project(MAIN_PROJ LANGUAGES CXX)

add_executable(MAIN ${SRC})

add_subdirectory(src/GLGEGraphic)
target_link_libraries(MAIN GLGE_GRAPHIC)
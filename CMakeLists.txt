cmake_minimum_required(VERSION 3.10)

set(SRC 
    src/main.cpp
)

project(MAIN_PROJ LANGUAGES CXX)

add_executable(MAIN ${SRC})
set_property(TARGET MAIN PROPERTY CXX_STANDARD 20)

add_subdirectory(src/GLGEGraphic)
target_link_libraries(MAIN GLGE_GRAPHIC)
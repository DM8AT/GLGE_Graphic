cmake_minimum_required(VERSION 3.10)

project(GLGE_GRAPHIC_PROJ LANGUAGES CXX C)

set(SRC 
    Backend/Instance.cpp
    Backend/API_Implementations/OpenGL/OGL_Instance.cpp
    Backend/API_Implementations/OpenGL/OGL_Window.cpp
    Backend/API_Implementations/OpenGL/OGL_CommandBuffer.cpp
    Backend/API_Implementations/OpenGL/OGL_Commands.cpp
    Backend/API_Implementations/OpenGL/OGL_RenderPipeline.cpp

    Frontend/Window/Window.cpp
    Frontend/Common.cpp
    Frontend/RenderPipeline.cpp
)

## add GLAD
set(SRC
    ${SRC}

    external/glad/src/glad.c
)

add_library(GLGE_GRAPHIC ${SRC})
target_include_directories(GLGE_GRAPHIC PRIVATE ${PROJECT_SOURCE_DIR}/external/glad/include)

## BG submodule
# add_subdirectory(${PROJECT_SOURCE_DIR}/../GLGE_BG ${PROJECT_BINARY_DIR}/../GLGE_BG)
# target_link_libraries(GLGE_GRAPHIC PUBLIC GLGE_BG)
# target_include_directories(GLGE_GRAPHIC PRIVATE ${PROJECT_SOURCE_DIR}/../GLGE_BG)
## Math submodule
# add_subdirectory(${PROJECT_SOURCE_DIR}/../GLGE_Math ${PROJECT_BINARY_DIR}/../GLGE_Math)
# target_link_libraries(GLGE_GRAPHIC PUBLIC GLGE_Math)
# target_include_directories(GLGE_GRAPHIC PRIVATE ${PROJECT_SOURCE_DIR}/../GLGE_Math)
## Core submodule
add_subdirectory(${PROJECT_SOURCE_DIR}/../GLGE_Core ${PROJECT_BINARY_DIR}/../GLGE_Core)
target_link_libraries(GLGE_GRAPHIC PUBLIC GLGE_CORE)
target_include_directories(GLGE_GRAPHIC PRIVATE ${PROJECT_SOURCE_DIR}/../GLGE_Core)

## SDL3
add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL ${PROJECT_BINARY_DIR}/external/SDL)
target_link_libraries(GLGE_GRAPHIC PRIVATE SDL3::SDL3)
target_include_directories(GLGE_GRAPHIC PRIVATE ${PROJECT_SOURCE_DIR}/external/SDL/include)
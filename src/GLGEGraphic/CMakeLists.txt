cmake_minimum_required(VERSION 3.10)

project(GLGE_GRAPHIC_PROJ LANGUAGES CXX C)

set(GLGE_GRAPHIC_VERSION 0.1.0)

set(SRC 
    Backend/Instance.cpp
    
    Backend/Objects/RenderObjectSystem.cpp
    
    Backend/API_Implementations/API_Instance.cpp
    Backend/API_Implementations/API_Shader.cpp
    Backend/API_Implementations/API_Buffer.cpp
    Backend/API_Implementations/API_MemoryArena.cpp
    Backend/API_Implementations/API_RenderMesh.cpp
    Backend/API_Implementations/API_CycleBuffer.cpp

    Backend/API_Implementations/OpenGL/OGL_Instance.cpp
    Backend/API_Implementations/OpenGL/OGL_Window.cpp
    Backend/API_Implementations/OpenGL/OGL_CommandBuffer.cpp
    Backend/API_Implementations/OpenGL/OGL_Commands.cpp
    Backend/API_Implementations/OpenGL/OGL_RenderPipeline.cpp
    Backend/API_Implementations/OpenGL/OGL_Texture.cpp
    Backend/API_Implementations/OpenGL/OGL_Shader.cpp
    Backend/API_Implementations/OpenGL/OGL_Material.cpp
    Backend/API_Implementations/OpenGL/OGL_Buffer.cpp
    Backend/API_Implementations/OpenGL/OGL_CycleBuffer.cpp
    Backend/API_Implementations/OpenGL/OGL_Framebuffer.cpp

    Frontend/Window/Window.cpp
    Frontend/RenderAPI/RenderPipeline.cpp
    Frontend/RenderAPI/RenderMesh.cpp
    Frontend/RenderAPI/RenderMeshRegistry.cpp
    Frontend/RenderAPI/Renderer.cpp
    Frontend/Common.cpp
    Frontend/Texture.cpp
    Frontend/TextureAsset.cpp
    Frontend/Shader.cpp
    Frontend/Material.cpp
    Frontend/Buffer.cpp
    Frontend/Compute.cpp
    Frontend/Camera.cpp
    Frontend/Keys.cpp
    Frontend/Mouse.cpp
    Frontend/Framebuffer.cpp
)

## add GLAD
set(SRC
    ${SRC}

    external/glad/src/glad.c
)

add_library(GLGE_GRAPHIC ${SRC})
# C++23 and versioning
set_target_properties(GLGE_GRAPHIC PROPERTIES 
    CXX_STANDARD 23 
    CXX_STANDARD_REQUIRED ON 
    VERSION ${GLGE_GRAPHIC_VERSION} 
    SOVERSION 1
)
target_include_directories(GLGE_GRAPHIC PUBLIC ${PROJECT_SOURCE_DIR}/external/glad/include)

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
target_include_directories(GLGE_GRAPHIC PUBLIC ${PROJECT_SOURCE_DIR}/../GLGE_Core)

## SDL3
set(SDL_WAYLAND_LIBDECOR OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/SDL ${PROJECT_BINARY_DIR}/external/SDL)
target_link_libraries(GLGE_GRAPHIC PUBLIC SDL3::SDL3)
target_include_directories(GLGE_GRAPHIC PUBLIC ${PROJECT_SOURCE_DIR}/external/SDL/include)

## STB image
target_include_directories(GLGE_GRAPHIC PUBLIC ${PROJECT_SOURCE_DIR}/external/stb)
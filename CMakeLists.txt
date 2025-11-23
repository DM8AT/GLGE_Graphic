cmake_minimum_required(VERSION 3.15)

project(MAIN_PROJ LANGUAGES CXX)

set(IMGUI_SRC src/external/imgui)

add_library(IMGUI 
    ${IMGUI_SRC}/imgui.cpp
    ${IMGUI_SRC}/imgui_draw.cpp
    ${IMGUI_SRC}/imgui_tables.cpp
    ${IMGUI_SRC}/imgui_widgets.cpp
    ${IMGUI_SRC}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_SRC}/backends/imgui_impl_opengl3.cpp
)

add_library(GLGE_IMGUI src/GLGE_ImGuiImpl.cpp)
target_link_libraries(GLGE_IMGUI PUBLIC GLGE_GRAPHIC IMGUI)

add_executable(MAIN src/main.cpp)

set_target_properties(MAIN PROPERTIES 
                        CXX_STANDARD 23 
                        CXX_STANDARD_REQUIRED ON)

set_target_properties(GLGE_IMGUI PROPERTIES 
                        CXX_STANDARD 23 
                        CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/Example/FirstPersonExample)
target_link_libraries(MAIN FirstPersonExample GLGE_IMGUI)

target_include_directories(IMGUI PUBLIC ${IMGUI_SRC})

# Apply to all targets in the project
if(MSVC)
   # Use dynamic runtime: /MD (Release), /MDd (Debug)
   set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()
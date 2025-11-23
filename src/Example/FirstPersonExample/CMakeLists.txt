cmake_minimum_required(VERSION 3.10)
project(FirstPersonExample_PROJ VERSION 0.1 LANGUAGES CXX)

# bring in the GLGEGraphic project located two levels up
add_subdirectory("../../GLGEGraphic" ${CMAKE_CURRENT_BINARY_DIR}/GLGEGraphic_build)

# build the example executable from the single source file in this directory
add_library(FirstPersonExample FirstPersonExample.cpp)

# use C++23
set_target_properties(FirstPersonExample PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# link against the GLGE_GRAPHIC target provided by the added subdirectory
target_link_libraries(FirstPersonExample PUBLIC GLGE_GRAPHIC)
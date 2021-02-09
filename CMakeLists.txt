cmake_minimum_required(VERSION 3.16.3)

project(execute) #executable

set(CMAKE_CXX_STANDARD)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-g -static -Wall -Wextra -std=c++17)


add_executable(${PROJECT_NAME} trianglesrc.cpp)

add_subdirectory(src) #if u add add_subdirectory,then u must create CMakeLists,txt for linking
add_subdirectory(abstraction)
add_subdirectory(vendor/stb_image)
add_subdirectory(${CMAKE_SOURCE_DIR}/assimp-5.0.1)
link_directories(vendor/stb_image)

target_include_directories(${PROJECT_NAME} PUBLIC abstraction/ include/ vendor/stb_image/ )
    #target_link_directories(${PROJECT_NAME} Public abstraction/)

target_link_libraries(glaD -ldl -lglfw -lpthread -lGL -lGLU -lglut -lXrandr -lX11 -lrt assimp) # linking

target_link_libraries(${PROJECT_NAME} glaD code stb_github ) # linking
    #target_link_libraries(${PROJECT_NAME} LINK_PUBLIC src_lib)
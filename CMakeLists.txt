cmake_minimum_required(VERSION 3.16)
project( oglapp LANGUAGES "CXX" )

add_executable( oglapp
src/camera.cpp
src/oglwindow.cpp
src/shader.cpp
src/mesh.cpp
src/texture.cpp 
src/light.cpp 
src/material.cpp 
src/directionallight.cpp
src/pointlight.cpp
src/spotlight.cpp
src/model.cpp
src/main.cpp )



find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp CONFIG REQUIRED)

if(${ASSIMP_IMPORTED_LOCATION})
    set_target_properties(assimp::assimp PROPERTIES IMPORTED_LOCATION "${ASSIMP_IMPORTED_LOCATION}")
endif()

target_link_libraries(oglapp PRIVATE
    glfw
    glm::glm
    GLEW::GLEW
    assimp::assimp
)

target_include_directories( oglapp PRIVATE include )

target_compile_features( oglapp PRIVATE cxx_std_20)

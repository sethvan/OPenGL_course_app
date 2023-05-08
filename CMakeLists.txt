cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE "/home/sethvan/vcpkg/scripts/buildsystems/vcpkg.cmake" )
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

set(CMAKE_POLICY_DEFAULT_CMP0012 NEW) # for error with assimp
find_package(assimp CONFIG REQUIRED)
set_target_properties(assimp::assimp PROPERTIES IMPORTED_LOCATION "/home/sethvan/vcpkg/packages/assimp_x64-linux/lib/libassimp.a")


target_link_libraries(oglapp PRIVATE
    glfw
    glm::glm
    GLEW::GLEW
    assimp::assimp
)

target_include_directories( oglapp PRIVATE include )

target_compile_features( oglapp PRIVATE cxx_std_20)
target_link_options( oglapp PRIVATE -ldl -fsanitize=address )
target_compile_options( oglapp PRIVATE 
	-Wall -Wextra -Wl,-z,defs -ldl -fwrapv -Og
	$<$<CONFIG:Debug>:
		-D_GLIBCXX_ASSERTIONS=1 -ggdb3 -fno-omit-frame-pointer -faas -fasynchronous-unwind-tables -fsanitize=address -fstack-protector-all 
	>
)
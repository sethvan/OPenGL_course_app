cmake_minimum_required(VERSION 3.16)

project( oglapp LANGUAGES "CXX" )

add_executable( oglapp
src/camera.cpp
src/oglwindow.cpp
src/shader.cpp
src/mesh.cpp
src/texture.cpp 
src/light.cpp 
src/light.cpp 
src/light.cpp 
src/material.cpp 
src/main.cpp )

target_include_directories( oglapp PRIVATE include )
target_compile_features( oglapp PRIVATE cxx_std_17)
target_link_options( oglapp PRIVATE -ldl -lglfw -lGLEW -lGL -fsanitize=address )
target_compile_options( oglapp PRIVATE 
	-Wall -Wextra -Wl,-z,defs -ldl -lglfw -lGLEW -lGL -fwrapv -Og
	$<$<CONFIG:Debug>:
		-D_GLIBCXX_ASSERTIONS=1 -ggdb3 -fno-omit-frame-pointer -faas -fasynchronous-unwind-tables -fsanitize=address -fstack-protector-all 
	>
)

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
src/main.cpp )



find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(oglapp PRIVATE
    glfw
    glm::glm
    GLEW::GLEW
)

target_include_directories(oglapp PRIVATE
    include
    ${GLFW3_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
	 ${GLEW_INCLUDE_DIRS}
)

target_compile_features( oglapp PRIVATE cxx_std_17)
target_link_options( oglapp PRIVATE -ldl -fsanitize=address )
target_compile_options( oglapp PRIVATE 
	-Wall -Wextra -Wl,-z,defs -ldl -fwrapv -Og
	$<$<CONFIG:Debug>:
		-D_GLIBCXX_ASSERTIONS=1 -ggdb3 -fno-omit-frame-pointer -faas -fasynchronous-unwind-tables -fsanitize=address -fstack-protector-all 
	>
)
cmake_minimum_required(VERSION 3.29)
project(image_test)
set(CMAKE_CXX_STANDARD 20)

add_executable(
	i 
	src/glad.c 
	src/quadMesh.cpp 
	src/loadShader.cpp 
	src/loadString.cpp 
	src/generateTexture.cpp 
	src/main.cpp
	src/player.cpp
	src/algebra.cpp
	src/walls.cpp
	src/solids.cpp
	)

target_include_directories(i PRIVATE dependencies)

target_link_libraries(i PRIVATE C:/lib/libglfw3.a)

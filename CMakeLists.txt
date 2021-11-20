cmake_minimum_required(VERSION 3.19)
project(pixelboard)

set(CMAKE_CXX_STANDARD 20)

# include third party projects
include(3rdparty/3rdparty.cmake)

add_executable(pixelboard
		src/main.cpp
		src/BoardRegion.cpp
		src/PixelBoard.cpp
)

target_include_directories(pixelboard PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(pixelboard PRIVATE fmt::fmt spdlog::spdlog)
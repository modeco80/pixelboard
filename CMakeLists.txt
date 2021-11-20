cmake_minimum_required(VERSION 3.19)
project(pixelboard)

set(CMAKE_CXX_STANDARD 20)

# include third party projects
#include(3rdparty/3rdparty.cmake)

find_package(Boost REQUIRED COMPONENTS system)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)


# include module subdirectories.
add_subdirectory(src/pixelboard)
add_subdirectory(src/websocket)
add_subdirectory(src/server)

add_executable(pixelboard
		src/main.cpp
)

target_link_libraries(pixelboard PRIVATE pixelboard::server spdlog::spdlog)
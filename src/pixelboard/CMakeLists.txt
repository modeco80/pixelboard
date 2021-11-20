# PixelBoard core

add_library(pixelboard_core
		BoardRegion.cpp
		PixelBoard.cpp
)

target_include_directories(pixelboard_core PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(pixelboard_core PRIVATE fmt::fmt spdlog::spdlog)

add_library(pixelboard::core ALIAS pixelboard_core)
add_library(pixelboard_server
		BoardServer.cpp
)

target_include_directories(pixelboard_server PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(pixelboard_server PUBLIC pixelboard::core pixelboard::websocket)
target_link_libraries(pixelboard_server PRIVATE fmt::fmt spdlog::spdlog)

add_library(pixelboard::server ALIAS pixelboard_server)
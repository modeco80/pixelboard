add_library(pixelboard_ws
		Server.cpp
		Listener.cpp netts.h HttpSession.cpp)

target_include_directories(pixelboard_ws PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(pixelboard_ws PRIVATE fmt::fmt spdlog::spdlog Boost::system)

add_library(pixelboard::websocket ALIAS pixelboard_ws)
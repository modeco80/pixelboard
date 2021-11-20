#include <iostream>
#include <filesystem>

#include <spdlog/spdlog.h>

#include <PixelBoard.h>

int main() {

	// Make the board directory if it doesn't exist
	if(!std::filesystem::exists(std::filesystem::current_path() / "board")) {
		spdlog::info("Board directory did not exist before; creating it");
		std::filesystem::create_directory(std::filesystem::current_path() / "board");
	}

	spdlog::info("Initializing PixelBoard server..");

	pixelboard::PixelBoard board;

	for(std::int32_t i = 10; i < 100; ++i)
		board.DrawPixel({i, i}, {.r = 255, .g = 128, .b = 100, .a = 255});

	for(std::int32_t i = 10; i < 100; ++i)
		board.DrawPixel({i - 2048, i - 100}, {.r = 255, .g = 128, .b = 100, .a = 255});


	return 0;
}

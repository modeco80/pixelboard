#include <pixelboard/PixelBoard.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <iostream>

int main() {
	pixelboard::PixelBoard board;

	// Make the board directory if it doesn't exist
	if(!std::filesystem::exists(std::filesystem::current_path() / "board")) {
		spdlog::info("Board directory did not exist before; creating it");
		std::filesystem::create_directory(std::filesystem::current_path() / "board");
	}

	spdlog::info("Initializing PixelBoard server..");


	board.LoadRegions();

	for(std::int32_t i = 15; i < 100; ++i)
		board.DrawPixel({i + 3, i}, {.r = 255, .g = 200, .b = 75, .a = 255});

	for(std::int32_t i = 20; i < 100; ++i)
		board.DrawPixel({i - 4096, i - 100}, {.r = 128, .g = 128, .b = 100, .a = 255});


	// save stuff.
	board.SaveRegions();
	return 0;
}

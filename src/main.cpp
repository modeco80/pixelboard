
#include <websocket/Server.h>

#include <spdlog/spdlog.h>

#include <filesystem>
#include <iostream>



int main() {
	net::io_context ioc;

	// Make the board directory if it doesn't exist
	if(!std::filesystem::exists(std::filesystem::current_path() / "board")) {
		spdlog::info("Board directory did not exist before; creating it");
		std::filesystem::create_directory(std::filesystem::current_path() / "board");
	}

	spdlog::info("Initializing PixelBoard server...");

	std::make_shared<pixelboard::websocket::Server>(ioc)->Run("0.0.0.0", 9090);

	ioc.run();
	return 0;
}


#include <spdlog/spdlog.h>
#include <websocket/Server.h>

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

	auto sp = std::make_shared<pixelboard::websocket::Server>(ioc);

	// Set some test callbacks

	struct MyData {
		int n{};
	};

	sp->SetOpen([](std::weak_ptr<pixelboard::websocket::Client> client) {
		if(auto sp = client.lock()) {
			sp->SetUserData<MyData>();
			spdlog::info("Connection opened from {}", sp->GetRemoteAddress().to_string());
		}
	});

	sp->SetMessage([](std::weak_ptr<pixelboard::websocket::Client> client, std::shared_ptr<const pixelboard::websocket::Message> message) {
		if(auto sp = client.lock()) {
			if(message->GetType() == pixelboard::websocket::Message::Type::Text) {
				spdlog::info("Message from {}: {}", sp->GetRemoteAddress().to_string(), message->GetString());
				sp->GetUserData<MyData>().n++;
				sp->Send(message);
			}
		}
	});

	sp->SetClose([](std::weak_ptr<pixelboard::websocket::Client> client) {
		if(auto sp = client.lock()) {
			spdlog::info("Connection closed from {}", sp->GetRemoteAddress().to_string());
			spdlog::info("MyData::n for this connection: {}", sp->GetUserData<MyData>().n);
		}
	});

	sp->Run("0.0.0.0", 9090);

	ioc.run();
	return 0;
}

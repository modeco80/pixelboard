//
// Created by lily on 11/20/2021.
//

#ifndef PIXELBOARD_SERVER_H
#define PIXELBOARD_SERVER_H

#include <boost/asio/io_context.hpp>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

// to pull in some stuff we don't wanna
#include <websocket/forward_decls.h>

// (insert loyal pledge to kick the person who made windows
// headers as garbage as they are here)
#ifdef _WIN32
	#ifdef SendMessage
		#undef SendMessage
	#endif
#endif

namespace pixelboard::websocket {

	// forward declare further internals
	namespace detail {
		struct Listener;
	}

	struct Server : public std::enable_shared_from_this<Server> {
		friend struct detail::Listener;
		friend struct Client;

		Server(boost::asio::io_context& ioc);

		~Server();

		// types for handlers.
		using Validate_t = std::function<void(std::shared_ptr<Client>)>;
		using Open_t = std::function<void(std::weak_ptr<Client>)>;
		using Close_t = std::function<void(std::weak_ptr<Client>)>;
		using Message_t = std::function<void(std::weak_ptr<Client>, std::shared_ptr<const Message>)>;
		// TODO: Http_t ?
		//  that'd imply needing to export some http session stuff though..

		void Run(const std::string& host, std::uint16_t port);
		void Stop();

		void SetValidate(Validate_t&&);
		void SetOpen(Open_t&&);
		void SetClose(Close_t&&);
		void SetMessage(Message_t&&);

		bool SendMessage(std::shared_ptr<Client> client, std::shared_ptr<const Message> message);


	   protected:
		Validate_t validate_handler;
		Open_t open_handler;
		Close_t close_handler;
		Message_t message_handler;

	   private:
		boost::asio::io_context& ioc;
		std::shared_ptr<detail::Listener> listener;
	};

} // namespace pixelboard::websocket

#endif //PIXELBOARD_SERVER_H

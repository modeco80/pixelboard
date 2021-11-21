//
// Created by lily on 11/20/2021.
//

#include <websocket/Server.h>
#include <boost/asio/ip/tcp.hpp>

#include "netts.h"

namespace pixelboard::websocket {

	std::shared_ptr<detail::Listener> RunListener(net::io_context& ioc, tcp::endpoint&& ep, const std::shared_ptr<Server>& server);

	Server::Server(net::io_context& ioc)
		: ioc(ioc) {

	}

	Server::~Server() {
		Stop();
	}

	void Server::Run(const std::string& host, std::uint16_t port) {
		listener = RunListener(ioc, tcp::endpoint { net::ip::make_address(host), port }, shared_from_this());
	}

	void Server::Stop() {
		if(listener)
			listener.reset();
	}
}
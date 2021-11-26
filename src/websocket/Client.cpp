//
// Created by lily on 11/21/2021.
//

#include <websocket/Client.h>
#include <websocket/Server.h>

#include "HttpUtils.h"

#include <iostream>

namespace pixelboard::websocket {

	struct Client::MessageQueue {
		/**
		 * The max amount of WebSocket messages we buffer
		 * inside of this send queue.
		 */
		constexpr static auto MAX_MESSAGES = 512;

		Client& self;
		std::vector<std::shared_ptr<const Message>> messages;

		explicit MessageQueue(Client& self)
			: self(self) {
			messages.reserve(MAX_MESSAGES);
		}

		[[nodiscard]] bool IsFull() const {
			return messages.size() >= MAX_MESSAGES;
		}

		std::shared_ptr<const Message>& GetFront() {
			return messages.front();
		}

		void OnWrite() {
			if(self.state.load() == State::Closing)
				return;

			messages.erase(messages.begin());

			// If there are more messages,
			// keep writing them
			if(!messages.empty())
				self.WriteMessage(messages.front());
		}

		void Push(std::shared_ptr<const Message> message) {
			if(IsFull())
				return;

			if(self.state.load() == State::Closing)
				return;

			messages.push_back(message);

			// Begin writing if we can
			if(messages.size() == 1)
				self.WriteMessage(messages.front());
		}
	};

	Client::Client(tcp::socket&& socket, const std::shared_ptr<Server>& server)
		: server(server),
		  wsStream(std::move(socket)) {
		messageQueue = std::make_unique<MessageQueue>(*this);
	}

	// Generate the trivial-dtor here
	// where Client::MessageQueue is fully defined.
	Client::~Client() = default;

	void Client::SelectSubprotocol(const std::string& subprotocol) {
		selectedSubprotocol = subprotocol;
	}

	net::ip::address Client::GetRemoteAddress() {
		return cachedAddress;
	}

	void Client::Run(http::request<http::string_body>&& request) {
		// plop in the upgrade request
		upgradeRequest = std::move(request);

		net::post(wsStream.get_executor(), [self = shared_from_this()] {
			self->cachedAddress = self->wsStream.next_layer().socket().remote_endpoint().address();

			// TODO: proxy load balancer support

			self->wsStream.set_option(beast::websocket::stream_base::decorator([self](beast::websocket::response_type& res) {
				SetCommonResponseFields(res);

				// If a subprotocol has been negotiated,
				// we decorate the response with it.
				// Fixes Chrome, which expects this header to exist in the response..
				// (which Chrome ironically enough is in the right and follows the spec properly)

				if(self->selectedSubprotocol.has_value())
					res.set(http::field::sec_websocket_protocol, self->selectedSubprotocol.value());
			}));

			// TODO: permessage-deflate?

			if(self->server->validate_handler) {
				// If we *have* a validate handler,
				// call it.
				if(!self->server->validate_handler(self->weak_from_this()))
					// Validate handler failed, close without
					// calling the close handler.
					return self->HardClose();
			}

			// Otherwise, let's just skip onto accept
			self->wsStream.async_accept(self->upgradeRequest, beast::bind_front_handler(&Client::OnAccept, self->shared_from_this()));
		});
	}

	void Client::OnAccept(const boost::system::error_code& ec) {
		if(ec)
			return;

		// Well, we got here, so we're open now!
		state.store(State::Open);

		// Call the server-defined Open handler.
		if(server->open_handler)
			server->open_handler(weak_from_this());

		// Start asynchronous read loop
		ReadMessage();
	}

	void Client::ReadMessage() {
		wsStream.async_read(messageBuffer, beast::bind_front_handler(&Client::OnRead, shared_from_this()));
	}

	void Client::OnRead(const boost::system::error_code& ec, std::size_t bytes_transferred) {
		boost::ignore_unused(bytes_transferred);

		if(ec == beast::websocket::error::closed)
			return Close();

		// this is a little grody :(

		if(wsStream.got_binary()) {
			if(server->message_handler)
				server->message_handler(weak_from_this(), std::make_shared<Message>(std::span<std::uint8_t>{reinterpret_cast<std::uint8_t*>(messageBuffer.data().data()), messageBuffer.size()}));
		} else if(wsStream.got_text()) {
			if(server->message_handler)
				server->message_handler(weak_from_this(), std::make_shared<Message>(std::string_view{ reinterpret_cast<char*>(messageBuffer.data().data()), messageBuffer.size()}));
		}

		// The Message constructor internally copies data, so we can clear the buffer
		messageBuffer.clear();

		// Read another WebSocket message
		ReadMessage();
	}

	void Client::WriteMessage(std::shared_ptr<const Message> message) {
		if(!message)
			return;

		if(message->GetType() == Message::Type::Text)
			wsStream.text(true);
		else if(message->GetType() == Message::Type::Binary)
			wsStream.binary(true);

		wsStream.async_write(net::buffer(message->data_buffer), beast::bind_front_handler(&Client::OnWrite, shared_from_this()));
	}

	void Client::OnWrite(const boost::system::error_code& ec, std::size_t bytes_transferred) {
		if(ec)
			return Close();

		// notify the message queue that we've written
		messageQueue->OnWrite();
	}

	void Client::Close() {
		state.store(State::Closing);
		wsStream.async_close(beast::websocket::close_reason(beast::websocket::close_code::normal), [self = shared_from_this()](const boost::system::error_code& ec) {
			self->state.store(State::Closed);

			if(self->server->close_handler)
				self->server->close_handler(self->weak_from_this());

			// After this all strong client handles should be relinquished
			// so this object dies
		});
	}

	void Client::Send(std::shared_ptr<const Message> message) {
		if(!messageQueue->IsFull())
			messageQueue->Push(message);
	}

	void Client::HardClose() {
		boost::system::error_code ec;
		state.store(State::Closing);
		wsStream.next_layer().socket().close(ec);
	}

} // namespace pixelboard::websocket
//
// Created by lily on 11/20/2021.
//

#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <websocket/forward_decls.h>

#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/bind_handler.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/read.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/beast/websocket/rfc6455.hpp>

#include "HttpUtils.h"
#include <websocket/NetworkingTSCompatibility.h>

namespace pixelboard::websocket {

	// TODO: prototype for running websocket session :)

	namespace detail {

		struct HttpSession : public std::enable_shared_from_this<HttpSession> {
			explicit HttpSession(tcp::socket&& socket, const std::shared_ptr<Server>& server)
				: stream(std::move(socket)),
				  queue(*this),
				  server(server) {
			}

			void Run() {
				net::dispatch(stream.get_executor(), beast::bind_front_handler(&HttpSession::DoRead, shared_from_this()));
			}

			void DoRead() {
				// Reset the HTTP request.
				req = {};

				stream.expires_after(std::chrono::seconds(2));

				// Do the read.
				boost::beast::http::async_read(stream, message_buffer, req, beast::bind_front_handler(&HttpSession::OnRead, shared_from_this()));
			}

			void OnRead(const boost::system::error_code& ec, std::size_t bytes_transferred) {
				boost::ignore_unused(bytes_transferred);

				// If the connection closed during read
				// then close
				if(ec == http::error::end_of_stream)
					return Close();

				if(beast::websocket::is_upgrade(req)) {
					if(req.target() == "/") {
						// Make websocket session
					}

					// Send a proper error out if the above condition isn't met.

					http::response<http::string_body> res{http::status::bad_request, req.version()};
					SetCommonResponseFields(res);
					res.body() = "WebSocket connection needs to be at /.";
					queue.Push(std::move(res));
				} else {
					spdlog::info("[IP {}] HTTP {} {}", stream.socket().remote_endpoint().address().to_string(), http::to_string(req.method()), req.target());

					http::response<http::string_body> res { http::status::ok, req.version() };
					SetCommonResponseFields(res);

					// nice little page
					res.body() = fmt::format(
					"<!DOCTYPE html>\r\n"
					"<html>\r\n"
					"<head>\r\n"
					"   <title>Pixelboard backend</title>\r\n"
					"</head>\r\n"
					"<body>\r\n"
					"   <h1>PixelBoard Backend</h1>\r\n"
					"   <p>There are no static pages here. Not even {}.</p>\r\n"
					"</body>\r\n"
					"</html>\r\n",
					req.target());

					res.set(http::field::content_type, "text/html");

					// Put it into the response queue.
					// The server will eventually get to it.
					queue.Push(std::move(res));
				}
			}

			void OnWrite(bool shouldClose, const boost::system::error_code& ec, std::size_t bytes_transferred) {
				boost::ignore_unused(bytes_transferred);

				// dunno
				if(ec)
					return Close();

				if(shouldClose)
					return Close();

				// Notify the queue that a write operation succeeded.
				if(queue.OnWrite())
					// If we can, read another request.
					return DoRead();
			}

			/**
			 * Close the socket. Only valid if we hold the socket.
			 * Otherwise undefined.
			 */
			void Close() {
				stream.close();
			}

		   private:
			/**
			 * Response queue for outgoing HTTP messages.
			 */
			struct ResponseQueue {
				explicit ResponseQueue(HttpSession& session)
					: self(session) {
					queue.reserve(MAX_ITEMS);
				}

				[[nodiscard]] bool IsFull() const {
					return queue.size() >= MAX_ITEMS;
				}

				/**
				 * Called when a HTTP response has been written to this session's stream.
				 */
				[[nodiscard]] bool OnWrite() {
					const auto was_full = IsFull();

					// Remove the work we just completed.
					queue.erase(queue.begin());

					// If there's more work, then execute that work.
					if(!queue.empty())
						(*queue.front())();

					return was_full;
				}

				template <class Body, class Fields>
				void operator()(http::response<Body, Fields>&& message) {
					// work implementation
					struct MessageWorkImpl : public MessageWork {
						HttpSession& self;
						http::response<Body, Fields> msg;

						MessageWorkImpl(HttpSession& session, http::response<Body, Fields>&& msg)
							: self(session),
							  msg(std::move(msg)) {
						}

						void operator()() override {
							http::async_write(self.stream, msg, beast::bind_front_handler(&HttpSession::OnWrite, self.shared_from_this(), msg.need_eof()));
						}
					};

					queue.push_back(std::make_unique<MessageWorkImpl>(self, std::move(message)));

					// If the queue has no previous work,
					// start it.
					if(queue.size() == 1)
						(*queue.front())();
				}

				template <class Body, class Fields>
				void Push(http::response<Body, Fields>&& message) {
					return this->operator()(std::move(message));
				}

			   private:
				/**
				 * The max amount of responses we will try to buffer.
				 */
				constexpr static auto MAX_ITEMS = 8;

				/**
				 * Type-erased base class for HTTP response work.
				 * Holds the response until destroyed.
				 */
				struct MessageWork {
					virtual ~MessageWork() = default;

					/**
					 * Writes the message.
					 */
					virtual void operator()() = 0;
				};

				HttpSession& self;
				std::vector<std::unique_ptr<MessageWork>> queue;
			};

			beast::tcp_stream stream;
			beast::flat_buffer message_buffer;
			http::request<http::string_body> req;

			ResponseQueue queue;

			std::shared_ptr<Server> server;
		};

	} // namespace detail

	std::shared_ptr<detail::HttpSession> RunHttpSession(tcp::socket&& socket, const std::shared_ptr<Server>& server) {
		auto sp = std::make_shared<detail::HttpSession>(std::move(socket), server);
		sp->Run();
		return sp;
	}

} // namespace pixelboard::websocket
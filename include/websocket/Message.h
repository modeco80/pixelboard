//
// Created by lily on 11/21/2021.
//

#ifndef PIXELBOARD_MESSAGE_H
#define PIXELBOARD_MESSAGE_H

#include <cstdint>
#include <vector>
#include <memory>

namespace pixelboard::websocket {

	/**
	 * Neat wrapper class for WebSocket messages.
	 */
	struct Message {

		enum class Type {
			Text,
			Binary
		};

		Message() = default;

		/**
		 * Move constructor.
		 * Invalidates moving message
		 * and makes this one own the buffer.
		 */
		Message(Message&& move);

		/**
		 * Copy constructor.
		 * Internally triggers shared_ptr<> copy.
		 */
		Message(const Message& copy);


		Message& operator=(const Message& other);


	   private:
		Type type{};
		std::shared_ptr<std::vector<std::uint8_t>> data_buffer;
	};

}

#endif // PIXELBOARD_MESSAGE_H

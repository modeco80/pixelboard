//
// This file provides forward declarations for
// the websocket layer's types, so that a shared_ptr<websocket::*>
// doesn't end up pulling in Beast/ASIO headers the millisecond it's used.
//
// Created by lily on 11/19/2021.
//

#ifndef PIXELBOARD_FORWARD_DECLS_H
#define PIXELBOARD_FORWARD_DECLS_H

namespace pixelboard::websocket {
	struct Server;
	struct Client;
	struct Message;
} // namespace pixelboard::websocket

#endif //PIXELBOARD_FORWARD_DECLS_H

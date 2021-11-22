//
// Created by lily on 11/21/2021.
//

#ifndef PIXELBOARD_HTTPUTILS_H
#define PIXELBOARD_HTTPUTILS_H

#include <boost/beast/http/message.hpp>

#include "NetworkingTSCompatibility.h"

namespace pixelboard::websocket {

	/**
	 * Set any common HTTP response fields.
	 */
	template <class Body, class Fields>
	constexpr void SetCommonResponseFields(http::response<Body, Fields>& res) {
		res.set(http::field::server, "Pixelboard/0.1");
	}


}

#endif // PIXELBOARD_HTTPUTILS_H

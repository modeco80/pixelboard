//
// Created by lily on 11/18/2021.
//

#ifndef PIXELBOARD_BOARDTYPES_H
#define PIXELBOARD_BOARDTYPES_H

#include <cstdint>

namespace pixelboard {

	/**
	 * Basic coordinate template type.
	 */
	template <class T>
	struct BasicCoordinate {
		using coordinate_type = T;

		coordinate_type x;
		coordinate_type y;

		friend constexpr bool operator==(const BasicCoordinate& l, const BasicCoordinate& r) {
			return (l.x == r.x) && (l.y == r.y);
		}

		friend constexpr bool operator!=(const BasicCoordinate& l, const BasicCoordinate& r) {
			return !(l == r); // NOLINT (we need this to be !(l==r))
		}
	};

	// COORDINATE SPACES

	// This type in theory means that there can only ever be 4096
	// regions (1024 per direction.)

	/**
	 * A board coordinate.
	 * This is relative to the whole board.
	 */
	using BoardCoordinate = BasicCoordinate<std::int32_t>;

	/**
	 * A coordinate in region space.
	 */
	using RegionCoordinate = BasicCoordinate<std::int16_t>;

	/**
	 * Pixel coordinate, used inside of a region
	 * once we know what region it's in
	 */
	using PixelCoordinate = BasicCoordinate<std::uint16_t>;

	/**
	 * Region width.
	 */
	constexpr auto REGION_WIDTH = 512;

	/**
	 * Region height.
	 */
	constexpr auto REGION_HEIGHT = 512;

	/**
	 * RGBA8888 color
	 */
	union Color {
		std::uint32_t total;

		// accessor for colors.
		struct {
			std::uint8_t r;
			std::uint8_t g;
			std::uint8_t b;
			std::uint8_t a;
		};
	};

} // namespace pixelboard

#endif //PIXELBOARD_BOARDTYPES_H

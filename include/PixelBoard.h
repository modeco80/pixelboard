//
// Created by lily on 11/18/2021.
//

#ifndef PIXELBOARD_PIXELBOARD_H
#define PIXELBOARD_PIXELBOARD_H

#include <BoardRegion.h>

#include <memory>
#include <optional>
#include <vector>

namespace pixelboard {

	/**
	 * The pixel board.
	 * Does pixel board things, as you'd expect it to.
	 */
	struct PixelBoard {
		/**
		 * Load all regions from the boards directory into memory.
		 */
		void LoadRegions();

		/**
		 * Draw a pixel on the board. Will create a new board region if the board
		 * coordinate is outside of any registered region.
		 */
		void DrawPixel(const BoardCoordinate& where, const Color& color);

	   private:
		/**
		 * Make a region
		 */
		std::shared_ptr<BoardRegion> MakeRegion(const RegionCoordinate& region);

		std::optional<std::shared_ptr<BoardRegion>> GetRegion(const RegionCoordinate&);

		/**
		 * Regions.
		 */
		std::vector<std::shared_ptr<BoardRegion>> regions;
	};

} // namespace pixelboard

#endif //PIXELBOARD_PIXELBOARD_H

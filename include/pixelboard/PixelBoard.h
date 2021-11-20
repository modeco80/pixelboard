//
// Created by lily on 11/18/2021.
//

#ifndef PIXELBOARD_PIXELBOARD_H
#define PIXELBOARD_PIXELBOARD_H

#include <pixelboard/BoardRegion.h>
#include <pixelboard/BoardTypes.h>

#include <memory>
#include <optional>
#include <vector>

namespace pixelboard {

	// Some potentional FIXME's:
	//
	// - Don't keep regions in memory if they aren't being used or written to?
	//   (This would increase complexity a bit but allow me to tune the board types
	//      to possibly 64-bit? It'd also GREATLY pull down the memory footprint)
	//
	// - Allow destroying region data? Sort of overlaps with the first point
	//      but it'd probably not be the WORST idea I've ever had, right?

	/**
	 * The infinite pixel board.
	 */
	struct PixelBoard {
		/**
		 * Load all of the regions from the boards directory into memory,
		 * registering them.
		 */
		void LoadRegions();

		/**
		 * Save all registered regions to disk.
		 */
		void SaveRegions();

		/**
		 * Draw a pixel on the board.
		 *
		 * Will create and register a new board region if
		 * the board coordinate is outside of any registered region.
		 *
		 * \param[in] where Where to draw, in the board coordinate space.
		 * \param[in] color The color of pixel to draw.
		 */
		void DrawPixel(const BoardCoordinate& where, const Color& color);

	   private:
		std::shared_ptr<BoardRegion> MakeRegion(const RegionCoordinate& region);

		std::optional<std::shared_ptr<BoardRegion>> GetRegion(const RegionCoordinate& coord);

		/**
		 * All tracked board regions.
		 */
		std::vector<std::shared_ptr<BoardRegion>> regions;
	};

} // namespace pixelboard

#endif //PIXELBOARD_PIXELBOARD_H

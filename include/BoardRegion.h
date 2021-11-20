//
// Created by lily on 11/18/2021.
//

#ifndef PIXELBOARD_PIXELBOARDREGION_H
#define PIXELBOARD_PIXELBOARDREGION_H

#include <BoardTypes.h>

#include <array>

namespace pixelboard {

	/**
	 * A region on the board.
	 */
	struct BoardRegion {
		/**
		 * Region constructor.
		 *
		 * \param[in] region Coordinate in region-space.
		 */
		explicit BoardRegion(const RegionCoordinate& region);

		/**
		 * Load data from region file.
		 */
		void LoadFile();

		/**
		 * Save data to a region file.
		 */
		void SaveFile();

		/**
		 * Plot a single pixel onto this region
		 *
		 * \param[in] where Where on the region this pixel should be plotted
		 * \param[in] color the color to plot
		 */
		void PlotPixel(const PixelCoordinate& where, const Color& color);

		const RegionCoordinate& GetCoordinate() const;

	   private:
		RegionCoordinate regionPos;

		/**
		 * Pixel data, in RGBA8888 format.
		 */
		std::array<Color, REGION_WIDTH * REGION_HEIGHT> pixelData {};
	};

} // namespace pixelboard

#endif //PIXELBOARD_PIXELBOARDREGION_H

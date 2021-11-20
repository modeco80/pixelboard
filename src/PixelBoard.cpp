//
// Created by lily on 11/19/2021.
//

#include <PixelBoard.h>

namespace pixelboard {

	namespace {
		// Convert a board coordinate to region coordinate.
		RegionCoordinate BoardToRegion(const BoardCoordinate& coord) {
			return {
				.x = static_cast<std::int16_t>(abs(coord.x / REGION_WIDTH)),
				.y = static_cast<std::int16_t>(abs(coord.y / REGION_HEIGHT))
			};
		}

		// Convert a board coordinate to pixel coordinate.
		// Region needs to be known beforehand.
		PixelCoordinate BoardToPixel(const BoardCoordinate& coord) {
			return {
				.x = static_cast<std::uint16_t>(abs(coord.x % REGION_WIDTH)),
				.y = static_cast<std::uint16_t>(abs(coord.y % REGION_HEIGHT))
			};
		}
	} // namespace

	std::optional<std::shared_ptr<BoardRegion>> PixelBoard::GetRegion(const RegionCoordinate& coord) {
		// Simple b-search.
		// Shouldn't be too slow since theres about 4096 max possible regions.

		for(auto& reg : regions)
			if(reg->GetCoordinate() == coord)
				return reg;

		return std::nullopt;
	}

	std::shared_ptr<BoardRegion> PixelBoard::MakeRegion(const RegionCoordinate& region) {
		auto sp = std::make_shared<BoardRegion>(region);
		regions.push_back(sp);
		return sp;
	}

	void PixelBoard::DrawPixel(const BoardCoordinate& where, const Color& color) {
		auto region_space = BoardToRegion(where);
		auto region = GetRegion(region_space);

		// If a region couldn't be found, make a new one.
		if(!region.has_value())
			region = MakeRegion(region_space);

		(*region)->PlotPixel(BoardToPixel(where), color);

		// TODO: I'm probably going to put this on a timer with the WS server.
		(*region)->SaveFile();
	}

} // namespace pixelboard

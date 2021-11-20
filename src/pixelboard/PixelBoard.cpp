//
// Created by lily on 11/19/2021.
//

#include <pixelboard/PixelBoard.h>
#include <spdlog/spdlog.h>

#include <filesystem>

namespace pixelboard {

	namespace {
		// Convert a board coordinate to region coordinate.
		RegionCoordinate BoardToRegion(const BoardCoordinate& coord) {
			return {
				.x = static_cast<std::int16_t>(coord.x / REGION_WIDTH),
				.y = static_cast<std::int16_t>(coord.y / REGION_HEIGHT)
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

	void PixelBoard::LoadRegions() {
		for(auto& dir : std::filesystem::directory_iterator(std::filesystem::current_path() / "board")) {
			auto path = dir.path().filename().string();
			RegionCoordinate rc{};

			// This is ugly and bad and I hate it because it furthers the hardcoded
			// board region format but NO ONE will ever see this, so whatever.
			sscanf(path.c_str(), "board_%hi_%hi.rgn", &rc.x, &rc.y);

			if(!GetRegion(rc)) {
				spdlog::info("Creating region for region {}x{}", rc.x, rc.y);
				MakeRegion(rc)->LoadFile();
			}
		}
	}

	void PixelBoard::SaveRegions() {
		for(auto& reg : regions) {
			spdlog::info("Saving region {}x{}", reg->GetCoordinate().x, reg->GetCoordinate().y);
			reg->SaveFile();
		}
	}

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

		// If a region couldn't be found, make and register a region
		// and clear it.
		if(!region) {
			region = MakeRegion(region_space);
			(*region)->Clear();
		}

		// Finally, plot the pixel inside of the region.
		(*region)->PlotPixel(BoardToPixel(where), color);
	}

} // namespace pixelboard

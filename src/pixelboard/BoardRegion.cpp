//
// Created by lily on 11/18/2021.
//

#include <fmt/core.h>
#include <pixelboard/BoardRegion.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>

namespace pixelboard {

	namespace {
		std::string MakeRegionFilename(const RegionCoordinate& region_coord) {
			return fmt::format("board_{}_{}.rgn", region_coord.x, region_coord.y);
		}

		// lazy "serialization". I should probably replace this later,
		// but for now this *should* work.
		// KEY WORD: should.

		template <class T>
		inline void LazyRead(std::istream& is, T& thing) {
			is.read(reinterpret_cast<char*>(&thing), sizeof(T));
		}

		template <class T>
		inline void LazyWrite(std::ostream& os, const T& thing) {
			os.write(reinterpret_cast<const char*>(&thing), sizeof(T));
		}

		// This is a simple data header we serialize along with
		// our bitmap data, mostly for verification purposes.
		// However, it also provides a magic and stuff.

		struct RegionDataHeader {
			constexpr static auto VALID_MAGIC = UINT32_C(0x50425230); // 'PBR0'
			std::uint32_t magic;									  // should be equal to VALID_MAGIC
			std::uint16_t width;
			std::uint16_t height;
			// RGBA8888 data follows.
		};

	} // namespace

	BoardRegion::BoardRegion(const RegionCoordinate& region)
		: regionPos(region) {

	}

	void BoardRegion::LoadFile() {
		auto path = std::filesystem::current_path() / "board" / MakeRegionFilename(regionPos);
		RegionDataHeader rdh {};
		std::ifstream ifs(path.string(), std::ifstream::binary);

		if(!ifs) {
			spdlog::error("Couldn't open board region file {} for reading.", path.string());
			return;
		}

		LazyRead(ifs, rdh);

		// your usual "this is a binary file and people can screw with it"
		// sanity checks apply.

		if(rdh.magic != RegionDataHeader::VALID_MAGIC) {
			spdlog::error("Region file {} is not in valid format", path.string());
			return;
		}

		// If the region isn't the right width or height,
		// it won't make a proper grid.

		if(rdh.width != REGION_WIDTH || rdh.height != REGION_HEIGHT) {
			spdlog::error("Region file {} is not {}x{}", path.string(), REGION_WIDTH, REGION_HEIGHT);
			return;
		}

		LazyRead(ifs, pixelData);
	}

	void BoardRegion::SaveFile() {
		auto path = std::filesystem::current_path() / "board" / MakeRegionFilename(regionPos);
		RegionDataHeader rdh {
			.magic = RegionDataHeader::VALID_MAGIC,

			.width = REGION_WIDTH,
			.height = REGION_HEIGHT
		};

		std::ofstream ofs(path.string(), std::ofstream::binary);

		if(!ofs) {
			spdlog::error("Could not open region file {} for writing", path.string());
			return;
		}

		LazyWrite(ofs, rdh);
		LazyWrite(ofs, pixelData);

		ofs.close();

		spdlog::info("Wrote region data to region file {}.", path.string());
	}

	void BoardRegion::Clear() {
		memset(&pixelData[0], 0xFF, sizeof(pixelData));
	}

	void BoardRegion::PlotPixel(const PixelCoordinate& where, const Color& color) {
		// some simple sanity checks always compiled in

		if(where.x > REGION_WIDTH)
			return;

		if(where.y > REGION_HEIGHT)
			return;

		pixelData[(REGION_WIDTH * where.y) + where.x] = color;
	}

	const RegionCoordinate& BoardRegion::GetCoordinate() const {
		return regionPos;
	}

} // namespace pixelboard
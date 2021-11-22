//
// Created by lily on 11/19/2021.
//

#ifndef PIXELBOARD_BOARDSERVER_H
#define PIXELBOARD_BOARDSERVER_H

#include <pixelboard/PixelBoard.h>
#include <server/WorkerThread.h>
#include <websocket/Server.h>

namespace pixelboard {

	/**
	 * Board server.
	 */
	struct BoardServer {

		void Run(const std::string& host, std::uint16_t port);

	   private:
		/**
  		 * Base class for worker thread data.
  		 */
		struct BaseBoardThreadData {
			enum class Kind {
				SaveBoard,
				DestroyBoard,
				PlotPixel,
				SendChunks
			} kind;

			constexpr explicit BaseBoardThreadData(Kind kind)
				: kind(kind) {
			}

			virtual ~BaseBoardThreadData() = default;
		};

		/**
		 * Trait template to avoid writing the same constructor for the same thing.
		 */
		template <BaseBoardThreadData::Kind kind>
		struct BoardThreadData : public BaseBoardThreadData {
			constexpr BoardThreadData()
				: BaseBoardThreadData(kind) {
			}
		};

		/**
		 * Thread message to save the board.
		 */
		struct ThreadSave : BoardThreadData<BaseBoardThreadData::Kind::SaveBoard> {};

		/**
		 * Thread message to destroy the board
		 */
		struct ThreadDestroy : BoardThreadData<BaseBoardThreadData::Kind::DestroyBoard> {};

		/**
		 * Thread message to plot a pixel on the board.
		 */
		struct ThreadPlotPixel : BoardThreadData<BaseBoardThreadData::Kind::PlotPixel> {
			BoardCoordinate coord {};
			Color color {};
		};

		/**
		 * Thread message to send a region as chunks to a user.
		 */
		struct ThreadSendChunks : BoardThreadData<BaseBoardThreadData::Kind::SendChunks> {
			RegionCoordinate region{};
			// TODO: websocket::Client handle probably
		};


		void BoardThreadProcessor(std::shared_ptr<BaseBoardThreadData> data);

		/**
		 * The pixelboard.
		 */
		PixelBoard pixelBoard;

		/**
		 * Board worker thread.
		 * All interaction with the pixelboard must happen here.
		 */
		WorkerThread<BaseBoardThreadData> boardThread;

		std::shared_ptr<websocket::Server> server;
	};

} // namespace pixelboard

#endif //PIXELBOARD_BOARDSERVER_H

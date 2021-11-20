//
// Created by lily on 11/19/2021.
//

#ifndef PIXELBOARD_BOARDSERVER_H
#define PIXELBOARD_BOARDSERVER_H

#include <pixelboard/PixelBoard.h>
#include <server/WorkerThread.h>

namespace pixelboard {

	/**
	 * Board server.
	 */
	struct BoardServer {
	   private:
		/**
  		 * Base class for worker thread data.
  		 */
		struct BaseBoardThreadData {
			enum class Kind {
				SaveBoard,
				DestroyBoard,
				PlotPixel
			} kind;

			constexpr explicit BaseBoardThreadData(Kind kind)
				: kind(kind) {
			}

			virtual ~BaseBoardThreadData() = default;
		};

		/**
		 * Trait template to avoid writing the same code a lot.
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
		struct BoardSave : BoardThreadData<BaseBoardThreadData::Kind::SaveBoard> {};

		/**
		 * Thread message to destroy the board
		 */
		struct BoardDestroy : BoardThreadData<BaseBoardThreadData::Kind::DestroyBoard> {};

		/**
		 * Thread message to plot a pixel on the board.
		 */
		struct BoardPlotPixel : BoardThreadData<BaseBoardThreadData::Kind::PlotPixel> {
			BoardCoordinate coord {};
			Color color {};
		};

		/**
		 * The pixelboard.
		 */
		PixelBoard pixelBoard;

		/**
		 * Board worker thread.
		 * All interaction with the pixelboard must happen here.
		 */
		WorkerThread<BaseBoardThreadData> boardThread;
	};

} // namespace pixelboard

#endif //PIXELBOARD_BOARDSERVER_H

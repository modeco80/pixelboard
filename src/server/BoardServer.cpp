//
// Created by lily on 11/20/2021.
//

#include <server/BoardServer.h>

namespace pixelboard {

	void BoardServer::BoardThreadProcessor(std::shared_ptr<BaseBoardThreadData> data) {
		switch(data->kind) {
			case BaseBoardThreadData::Kind::SaveBoard:
				pixelBoard.SaveRegions();
				break;

			case BaseBoardThreadData::Kind::PlotPixel: {
				auto sp = std::static_pointer_cast<ThreadPlotPixel>(data);
				pixelBoard.DrawPixel(sp->coord, sp->color);
			} break;

			case BaseBoardThreadData::Kind::SendChunks: {
				auto sp = std::static_pointer_cast<ThreadSendChunks>(data);
			} break;

			default:
				break;
		}
	}

}
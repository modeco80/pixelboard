//
// Created by lily on 11/20/2021.
//

#ifndef PIXELBOARD_WORKERTHREAD_H
#define PIXELBOARD_WORKERTHREAD_H

#include <condition_variable>
#include <thread>
#include <deque>
#include <functional>

namespace pixelboard {

	/**
	 * Worker thread thing.
	 *
	 * \tparam BaseData User-defined base data type.
	 */
	template<typename BaseData>
	struct WorkerThread {

		template<class Processor>
		explicit inline WorkerThread(Processor&& processor) {

		}


		template<class T>
		inline void PushData(std::shared_ptr<T>& ptr) {

		}

	   private:

		void ThreadEntry() {

		}

		std::thread thread;

		std::mutex mutex;
		std::condition_variable cv;

		std::deque<std::shared_ptr<BaseData>> queue;
	};

}

#endif //PIXELBOARD_WORKERTHREAD_H

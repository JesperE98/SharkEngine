#ifndef ENGINE_CORE_MESSAGING_MESSAGEQUEUE_H
#define ENGINE_CORE_MESSAGING_MESSAGEQUEUE_H

#include "EngineMessage.h"

#include <queue>
#include <mutex>

namespace Shark::Core {

	class MessageQueue {
	public:

		// Add a message to the back of the line
		void Push(const EngineMessage& msg) {
			std::lock_guard<std::mutex> lock(queueMutex);
			messages.push(msg);
		}

		// Checks if there is anything to process
		bool HasMessage() const {
			return !messages.empty();
		}

		// Get the next message and remove it from the queue
		bool Pop(EngineMessage& outMsg) {
			std::lock_guard<std::mutex> lock(queueMutex);
			if (messages.empty()) return false;

			outMsg = messages.front();
			messages.pop();
			return true;
		}

	private:
		std::queue<EngineMessage> messages;
		std::mutex queueMutex;
	};
}

#endif // ENGINE_CORE_MESSAGING_MESSAGEQUEUE_H

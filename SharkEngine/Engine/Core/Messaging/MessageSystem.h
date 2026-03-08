#ifndef ENGINE_CORE_MESSAGING_MESSAGEQUEUE_H
#define ENGINE_CORE_MESSAGING_MESSAGEQUEUE_H

#include <queue>
#include <mutex>

namespace Shark::Core {

	enum class EventType {
		None = 0,				EditorWindowOpen,
		EditorWindowClose,		EditorWindowResize,
		KeyPressed,				KeyReleased,
		MouseButtonPressed,		MouseButtonReleased,
		MouseMoved,				MouseScrolled,
		GameObjectCreated,		GameObjectDestroyed,
		SceneLoaded,			SceneUnloaded,
		LoadModel,				ModelLoaded,
		LoadShader,				ShaderLoaded,
		LoadTexture,			TextureLoaded,
		LoadPrimitiveType,		PrimitiveTypeLoaded,
		ReloadMesh,				UnloadMesh,
		LogAdded,
		ErrorMessage
	};

	struct Message {
		EventType type = EventType::None;
		std::string payload = "";
		void* data{ nullptr };
	};

	class MessageSystem {
	public:

		// Add a message to the back of the line
		void Push(const Message& msg) {
			std::lock_guard<std::mutex> lock(queueMutex);
			messages.push(msg);
		}

		// Checks if there is anything to process
		bool HasMessage() const {
			return !messages.empty();
		}

		// Get the next message and remove it from the queue
		bool Pop(Message& outMsg) {
			std::lock_guard<std::mutex> lock(queueMutex);
			if (messages.empty()) return false;

			outMsg = messages.front();
			messages.pop();
			return true;
		}

	private:
		std::queue<Message> messages;
		std::mutex queueMutex;
	};
}

#endif // ENGINE_CORE_MESSAGING_MESSAGEQUEUE_H

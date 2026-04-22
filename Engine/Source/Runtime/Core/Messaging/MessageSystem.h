#ifndef ENGINE_CORE_MESSAGING_MESSAGEQUEUE_H
#define ENGINE_CORE_MESSAGING_MESSAGEQUEUE_H

#include <queue>
#include <mutex>

namespace Shark::Core {

	enum class EventType : uint8_t {
		None = 0,				EditorWindowOpen,
		EditorWindowClose,		EditorWindowResize,
		KeyPressed,				KeyReleased,
		MouseButtonPressed,		MouseButtonReleased,
		MouseMoved,				MouseScrolled,
		GameObjectCreated,		GameObjectDestroyed,
		LoadScene,				UnloadScene,
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

#pragma region HELPER FUNCTIONS

	/**
	 * @brief Constructs and returns a Message initialized with the specified event type, optional payload, and optional data pointer.
	 * @param type The event type for the message.
	 * @param payload Optional payload string for the message. Defaults to an empty string.
	 * @param data Optional user-defined pointer associated with the message. Defaults to nullptr.
	 * @return A Message object with its type, payload, and data fields set to the provided values.
	 */
	inline Message MakeMessage(EventType type, const std::string& payload = "", void* data = nullptr) {
		Message msg;
		msg.type = type;
		msg.payload = payload;
		msg.data = data;

		return msg;
	}

	/**
	 * @brief Creates an event message and delivers it to the target's inbox by calling Push.
	 * @tparam T The recipient type; must have an inbox with a Push method compatible with MakeMessage(EventType, const std::string&, void*).
	 * @param target Reference to the recipient object. The type T must provide an inbox member with a Push method that accepts the message produced by MakeMessage.
	 * @param type The EventType value describing the event to send.
	 * @param payload Optional string payload to include in the message (defaults to an empty string).
	 * @param data Optional pointer to additional data to attach to the message (defaults to nullptr).
	 */
	template<typename T>
	inline void SendTo(T& target, EventType type, const std::string& payload = "", void* data = nullptr) {
		target.inbox.Push(MakeMessage(type, payload, data));
	}

#pragma endregion
}

#endif // ENGINE_CORE_MESSAGING_MESSAGEQUEUE_H

#ifndef ENGINE_CORE_MESSAGING_ENGINEMESSAGE_H
#define ENGINE_CORE_MESSAGING_ENGINEMESSAGE_H

#include <string>

namespace Shark::Core {
	enum class MessageType {
		None = 0,
		WindowClose,
		WindowResize,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled,
		GameObjectCreated,
		GameObjectDestroyed,
		SceneLoaded,
		SceneUnloaded,
		LoadModel,
		ModelLoaded,
		LoadShader,
		ShaderLoaded,
		LoadTexture,
		TextureLoaded,
		LoadPrimitiveType,
		PrimitiveTypeLoaded,
		ReloadMesh,
		UnloadMesh,
		ErrorMessage,
	};

	struct EngineMessage {
		MessageType type = MessageType::None;
		std::string payload = "";
		void* data{ nullptr };
	};
}

#endif // ENGINE_CORE_MESSAGING_ENGINEMESSAGE_H
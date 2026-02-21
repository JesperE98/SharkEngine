#ifndef LEVEL_EDITOR_MANAGER_H
#define LEVEL_EDITOR_MANAGER_H

#include <Core/Messaging/MessageQueue.h>
#include <string>
#include <vector>

namespace Shark::Graphics { class Mesh; enum class PrimitiveType; }

namespace Shark::Editor {
	class LevelEditorManager
	{
	public:
		// The mailbox for other systems to talk to the EditorApp
		Shark::Core::MessageQueue inbox;

		static LevelEditorManager& Get() {
			static LevelEditorManager instance;
			return instance;
		}

		void Init();
		void Update(float DeltaTime);
		void Shutdown();

		// The UI call that triggers the request
		void RequestModelLoad(const std::string& path);
		void RequestPrimitiveLoad(Shark::Graphics::PrimitiveType type);
		void RequestTextureLoad(const std::string& path);

		void ReceiveMessage(const Shark::Core::EngineMessage& msg);

	private:
		LevelEditorManager() = default;

		// Delete copy constructor and assignment operator
		LevelEditorManager(const LevelEditorManager&) = delete;
		LevelEditorManager& operator=(const LevelEditorManager&) = delete;

		void LoadModel(const Shark::Core::EngineMessage& msg);
		void LoadPrimitive(const Shark::Core::EngineMessage& msg);
	};
}
#endif
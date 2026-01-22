#ifndef LEVEL_EDITOR_MANAGER_H
#define LEVEL_EDITOR_MANAGER_H

#include <Core/Messaging/MessageQueue.h>
#include <string>
#include <vector>

namespace Shark::Graphics { class Mesh; }

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

	private:
		LevelEditorManager() = default;

		std::vector<Shark::Graphics::Mesh*> m_ActiveMeshes;
	};
}
#endif
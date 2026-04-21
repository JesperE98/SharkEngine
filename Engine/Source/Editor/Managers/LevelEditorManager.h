#ifndef LEVEL_EDITOR_MANAGER_H
#define LEVEL_EDITOR_MANAGER_H

#include <Core/Messaging/MessageSystem.h>
#include <string>

namespace Shark::Graphics { 
	class Mesh; 
	enum class PrimitiveType : uint8_t; 
}

namespace Shark::Editor {

	class InspectorWindow;

	class LevelEditorManager
	{
	public:
		// The mailbox for other systems to talk to the EditorApp
		Core::MessageSystem inbox;

		static LevelEditorManager& Get() {
			static LevelEditorManager instance;
			return instance;
		}

		void Initialize();
		void Update(float DeltaTime);
		void Shutdown();

		// The UI call that triggers the request
		void RequestModelLoad(const std::string& path);
		void RequestPrimitiveLoad(Graphics::PrimitiveType type);
		void RequestDiffTextureLoad(const std::string& path);
		void RequestSpecTextureLoad(const std::string& path);

		void ReceiveMessage(const Core::Message& msg);
		void SetInspectorWindow(InspectorWindow& inspector) { m_InspectorWindow = &inspector; }

	private:
		InspectorWindow* m_InspectorWindow{ nullptr };

		LevelEditorManager() = default;
		~LevelEditorManager() {
			Shutdown();
		}

		// Delete copy constructor and assignment operator
		LevelEditorManager(const LevelEditorManager&) = delete;
		LevelEditorManager& operator=(const LevelEditorManager&) = delete;

		void LoadModel(const Core::Message& msg);
		void LoadPrimitive(const Core::Message& msg);
	};
}
#endif
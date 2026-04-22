#ifndef SHARKEDITOR_H
#define SHARKEDITOR_H

#include <Core/App/Application.h>

namespace Shark::Editor { class WindowManager; class EditorMenuBar; }

namespace Shark {
	class EditorApp : public Core::Application {
	public:
		EditorApp() = default;
		~EditorApp() override final = default;

		void OnInitialize() override final;
		void OnUpdate(float deltaTime) override final;
		void OnRender() override final;
		void OnShutdown() override final;

	private:
		Editor::EditorMenuBar* m_MenuBar{ nullptr };

		void InitImGui();
		void BeginImGuiFrame();
		void EndImGuiFrame();
		void RenderPlayBar();
	};
}


#endif // SHARKEDITOR_H
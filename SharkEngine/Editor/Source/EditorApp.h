#ifndef SHARKEDITOR_H
#define SHARKEDITOR_H

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>

namespace Shark::Editor {
	class HierarchyWindow;
	class InspectorWindow;
	class SceneViewport;
	class ConsoleWindow;
	class WindowBase;
}

namespace Shark {
	class EditorApp
	{
	public:
		EditorApp();
		~EditorApp();

		void Run();

	private:
		std::vector<std::unique_ptr<Shark::Editor::WindowBase>> m_Windows;
		std::unique_ptr<Shark::Editor::SceneViewport> m_SceneViewport;
		Shark::Editor::HierarchyWindow* m_HierarchyWindow{ nullptr };
		Shark::Editor::InspectorWindow* m_InspectorWindow{ nullptr };
		Shark::Editor::ConsoleWindow* m_ConsoleWindow{ nullptr };

		// Helper function to check for errors
		static void CheckGLErrors(const std::string& context);

		void CreateEditorWindow();
		void InitializeViewport();
		void BeginFrame();
		void RenderPanels(float deltaTime);
		void RenderSceneViewport();
		void Render();
		void RenderStatsWindow();
	};
}


#endif // SHARKEDITOR_H
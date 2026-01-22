#ifndef SHARKEDITOR_H
#define SHARKEDITOR_H

#include <stdio.h>
#include <vector>
#include <string>
#include <memory>

namespace Shark::Editor {
	class HierarchyPanel;
	class InspectorPanel;
	class SceneViewport;
}

namespace Shark::Interfaces { class IPanel; }

namespace Shark {
	class EditorApp
	{
	public:
		EditorApp();
		~EditorApp();

		void Run();

	private:
		std::vector<Shark::Interfaces::IPanel*> m_Panels;
		Shark::Editor::HierarchyPanel* m_HierarchyPanel{ nullptr };
		Shark::Editor::InspectorPanel* m_InspectorPanel{ nullptr };
		std::unique_ptr<Shark::Editor::SceneViewport> m_SceneViewport;

		// Helper function to check for errors
		static void CheckGLErrors(const std::string& context);

		void CreateEditorWindow();
		void InitPanels();
		void BeginFrame();
		void RenderPanels(float deltaTime);
		void RenderSceneViewport();
		void Render();
	};
}


#endif // SHARKEDITOR_H
#ifndef HIERARCHYPANEL_H
#define HIERARCHYPANEL_H

#include "EditorWindow.h"

namespace Shark::Core { class GameObject; class MessageSystem; }
namespace Shark { class Scene; }

namespace Shark::Editor {

	class InspectorWindow;

	class HierarchyWindow : virtual public EditorWindow
	{
	public:
		HierarchyWindow() : EditorWindow("Hierarchy", true) {}
		~HierarchyWindow() override;

#pragma region EditorWindow override functions
		void OnInitialize() override;
		void OnUpdateWindow(float deltaTime) override;
		void OnShutdown() override;
		
#pragma endregion

		void SetInspector(InspectorWindow* inspector) { m_Inspector = inspector; }

	private:
		
		Core::GameObject* m_SelectedObject{ nullptr };
		InspectorWindow* m_Inspector{ nullptr };


		void DrawObjectNode(Core::GameObject* obj, const char* filter);
		bool NameMatchesFilter(const std::string& name, const char* filter);
		void DrawRootDropZone(Scene* scene);
	};
}


#endif
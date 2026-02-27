#ifndef HIERARCHYPANEL_H
#define HIERARCHYPANEL_H

#include "WindowBase.h"

namespace Shark::Core { class GameObject; }

namespace Shark::Editor {

	class InspectorWindow;

	class HierarchyWindow : virtual public WindowBase
	{
	public:
		HierarchyWindow() : WindowBase("Hierarchy", true) {}
		~HierarchyWindow() override;

#pragma region WindowBase override functions
		void OnInitialize() override;
		void OnRenderPanel(float deltaTime) override;
		void OnShutdown() override;
		
#pragma endregion

		void SetInspector(InspectorWindow* inspector) { m_Inspector = inspector; }

	private:
		
		Shark::Core::GameObject* m_SelectedObject{ nullptr };
		InspectorWindow* m_Inspector{ nullptr };


		void DrawObjectNode(Shark::Core::GameObject* obj, const char* filter);
		bool NameMatchesFilter(const std::string& name, const char* filter);
	};
}


#endif
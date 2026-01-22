#ifndef INSPECTORPANEL_H
#define INSPECTORPANEL_H

#include "Source/Interfaces/IPanel.h"

namespace Shark::Entities { class GameObject; }

namespace Shark::Editor {
	class InspectorPanel : public Shark::Interfaces::IPanel
	{
	public:
		InspectorPanel() : m_Name("Inspector"), m_IsVisible(true), m_SelectedObject(nullptr) {}
		~InspectorPanel() override;

#pragma region IPanel functions
		void OnInit() override;
		void OnRenderPanel(float deltaTime) override;
		void OnShutdown() override;
		const std::string& GetName() const override { return m_Name; }
		bool IsVisible() const override { return m_IsVisible; }
		void SetVisible(bool value) override;
#pragma endregion

		void SetSelectedObject(Shark::Entities::GameObject* obj) { m_SelectedObject = obj; }

	private:
		std::string m_Name;
		bool m_IsVisible;
		Shark::Entities::GameObject* m_SelectedObject;
	};
}


#endif // INSPECTORPANEL_H
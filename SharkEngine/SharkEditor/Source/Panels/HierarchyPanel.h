#ifndef HIERARCHYPANEL_H
#define HIERARCHYPANEL_H

#include "Source/Interfaces/IPanel.h"
#include <Core/Entities/GameObject.h>

class InspectorPanel;

class HierarchyPanel : public IPanel
{
public:
	HierarchyPanel();
	~HierarchyPanel() override;

#pragma region IPanel override functions
	void OnInit() override;
	void OnRenderPanel(float deltaTime) override;
	void OnShutdown() override;
	const std::string& GetName() const override;
	bool IsVisible() const override;
	void SetVisible(bool visible) override;
#pragma endregion

	void SetInspector(InspectorPanel* inspector) { m_Inspector = inspector; }

private:

	std::string m_Name = "Hierarchy";
	bool m_Visible = true;
	GameObject* m_SelectedObject{ nullptr };
	InspectorPanel* m_Inspector{ nullptr };

	void DrawObjectNode(GameObject* obj, const char* filter);
	bool NameMatchesFilter(const std::string& name, const char* filter);
};

#endif
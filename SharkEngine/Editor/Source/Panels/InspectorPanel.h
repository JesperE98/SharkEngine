#ifndef INSPECTORPANEL_H
#define INSPECTORPANEL_H

#include "Source/Interfaces/IPanel.h"

#include <Core/GameObject.h>

namespace Shark::Editor {
	class InspectorPanel : public Shark::Interfaces::IPanel
	{
	public:
		static InspectorPanel& Get() {
			static InspectorPanel instance;
			return instance;
		}

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

		void SetSelectedObject(Shark::Core::GameObject* obj) { m_SelectedObject = obj; }
		Shark::Core::GameObject* GetSelectedObject() { return m_SelectedObject; }
	private:
		std::string m_Name;
		bool m_IsVisible;
		Shark::Core::GameObject* m_SelectedObject;

		// Delete Copy Constructor and Assignment Operator
		InspectorPanel(const InspectorPanel&) = delete;
		InspectorPanel& operator=(const InspectorPanel&) = delete;

		void DrawObjectName(Shark::Core::GameObject* obj);
		void DrawTransform(Shark::Math::Transform& transform);
		void DrawAddComponentButton(Shark::Core::GameObject* obj);

		template<typename T>
		void OnComponentUI(T* component);

		template<typename T>
		void DrawComponentUI(const std::string& name, Shark::Core::GameObject* obj) {
			auto* component = obj->GetComponent<T>();
			if (component) {
				// Set a consistent ID for ImGui to avoid conflicts
				ImGui::PushID(typeid(T).name()); // Use component type as part of ID

				bool open = ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);

				if (ImGui::BeginPopupContextItem("ComponentSettings")) {
					if (ImGui::MenuItem("Remove Component")) {
						obj->RemoveComponent<T>();
					}
					ImGui::EndPopup();
				}

				if (open) {
					OnComponentUI(component);
				}

				ImGui::PopID();
				ImGui::Spacing();
			}
		}
	};
}


#endif // INSPECTORPANEL_H
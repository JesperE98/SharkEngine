#ifndef INSPECTORPANEL_H
#define INSPECTORPANEL_H

#include "WindowBase.h"

#include <Core/GameObject.h>

namespace Shark::Math { struct Transform; }

namespace Shark::Editor {
	class InspectorWindow : virtual public WindowBase
	{
	public:

		InspectorWindow() : WindowBase("Inspector", true), m_SelectedObject(nullptr) {}
		~InspectorWindow() override;

#pragma region WindowBase functions
		void OnInitialize() override;
		void OnRenderPanel(float deltaTime) override;
		void OnShutdown() override;
#pragma endregion

		void SetSelectedObject(Core::GameObject* obj) { m_SelectedObject = obj; }
		Shark::Core::GameObject* GetSelectedObject() { return m_SelectedObject; }
	private:
		Shark::Core::GameObject* m_SelectedObject;


		void DrawObjectName(Shark::Core::GameObject* obj);
		void DrawTransform(Shark::Math::Transform& transform);
		void DrawAddComponentButton(Shark::Core::GameObject* obj);

		template<typename T>
		void OnComponentUI(T* component);

		template<typename T>
		void DrawComponentUI(const std::string& name, Core::GameObject* obj) {
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
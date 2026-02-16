#include "HierarchyPanel.h"
#include "InspectorPanel.h"

#pragma region Engine Includes
#include <Core/GameObject.h>
#include <Core/Utilities/Debug.h>
#include <Scene/Scene.h>
#include <Managers/SceneManager.h>
#pragma endregion

namespace Shark::Editor {

    using Shark::Editor::InspectorPanel;
	using Shark::Core::GameObject;
    using Shark::Managers::SceneManager;

    HierarchyPanel::HierarchyPanel()
    {
        SE_LOG(Editor, "HierarchyPanel::HierarchyPanel() - Creating Hierarchy panel.");
    }

    HierarchyPanel::~HierarchyPanel()
    {
        if (m_SelectedObject) {
            delete m_SelectedObject;
            m_SelectedObject = nullptr;
        }
    }

    void HierarchyPanel::OnInit()
    {
    }

    void HierarchyPanel::OnRenderPanel(float deltaTime)
    {
        if (!m_Visible) return;

        ImGui::Begin(m_Name.c_str(), &m_Visible);

        // Search Bar
        static char searchBuffer[128] = "";
        ImGui::InputTextWithHint("##Search", "Search...", searchBuffer, IM_ARRAYSIZE(searchBuffer));

        // Iterate over root-level objects
        Scene& scene = *SceneManager::Get().GetActiveScene();
        for (auto& obj : scene.GetGameObjects()) {
            DrawObjectNode(obj, searchBuffer);
        }


        ImGui::End();
    }

    void HierarchyPanel::OnShutdown()
    {
    }

    const std::string& HierarchyPanel::GetName() const
    {
        return m_Name;
    }

    bool HierarchyPanel::IsVisible() const
    {
        return m_Visible;
    }

    void HierarchyPanel::SetVisible(bool visible)
    {
        m_Visible = visible;
    }

    void HierarchyPanel::DrawObjectNode(GameObject* obj, const char* filter)
    {
        if (!obj || (filter[0] != '\n' && !NameMatchesFilter(obj->GetName(), filter))) {
            return;
        }

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

        if (obj == m_SelectedObject) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool open = ImGui::TreeNodeEx((void*)obj, flags, "%s", obj->GetName().c_str());

        // Selection
        if (ImGui::IsItemClicked()) {
            m_SelectedObject = obj;
            if (m_Inspector) {
                m_Inspector->SetSelectedObject(m_SelectedObject);
            }
        }

        // Context Menu
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Rename")) {
                // TODO - Trigger rename logic
            }
            if (ImGui::MenuItem("Delete")) {
                // TODO - Mark for deletion
            }
            ImGui::EndPopup();
        }

        // Drag Source
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("HIERARCHY_DRAG", &obj, sizeof(Object*));
            ImGui::Text("Move %s", obj->GetName().c_str());
            ImGui::EndDragDropSource();
        }

        // Drop Target
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_DRAG")) {
                Object* dragged = *(Object**)payload->Data;

            }
            ImGui::EndDragDropTarget();
        }

        // Recurisvely draw children
        if (open) {
            for (GameObject* child : obj->GetChildren()) {
                DrawObjectNode(child, filter);
            }
            ImGui::TreePop();
        }
    }

    bool HierarchyPanel::NameMatchesFilter(const std::string& name, const char* filter)
    {
        return std::string(name).find(filter) != std::string::npos;
    }

}
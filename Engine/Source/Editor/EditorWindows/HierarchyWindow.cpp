#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "Managers/LevelEditorManager.h"

#pragma region Engine Includes
#include <Core/GameObject.h>
#include <Core/Utilities/Debug.h>
#include <Scene/Scene.h>
#include <Scene/SceneManager.h>
#include <Graphics/Resources/PrimitiveMesh.h>
#pragma endregion

namespace Shark::Editor {

    using Editor::InspectorWindow;
    using Editor::LevelEditorManager;
    using Core::GameObject;
    using Core::SceneManager;
    using Graphics::PrimitiveType;

    HierarchyWindow::~HierarchyWindow()
    {
        if (m_SelectedObject) {
            m_SelectedObject = nullptr;
        }
    }

    void HierarchyWindow::OnInitialize()
    {
    }

    void HierarchyWindow::OnUpdateWindow(float deltaTime)
    {
        // Search Bar
        static char searchBuffer[128] = "";
        ImGui::InputTextWithHint("##Search", "Search...", searchBuffer, IM_ARRAYSIZE(searchBuffer));

        // Iterate over root-level objects
        Scene& scene = *SceneManager::Get().GetActiveScene();
        for (auto& obj : scene.GetGameObjects()) {
            DrawObjectNode(obj, searchBuffer);
        }

        // Clear Selection if clicked on empty space
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            m_SelectedObject = nullptr;
            if (m_Inspector) {
                m_Inspector->SetSelectedObject(nullptr);
            }
        }

        // GLOBAL CONTEXT MENU (Right Cick Empty Space)
        // Will ONLY trigger if not right-clicking on a actual item node
        if (ImGui::BeginPopupContextWindow("HierarchyContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)){
            if (ImGui::MenuItem("Create Empty")) {
                GameObject* obj = new GameObject("Empty GameObject");
                scene.AddGameObject(obj);
            }

            if (ImGui::BeginMenu("3D Object")) {
                if (ImGui::MenuItem("Cube")) {
                    LevelEditorManager::Get().RequestPrimitiveLoad(PrimitiveType::Cube);
                }
                if (ImGui::MenuItem("Sphere")) {
                    //LevelEditorManager::Get().RequestPrimitiveLoad(PrimitiveType::Sphere);
                }
                if (ImGui::MenuItem("Plane")) {
                    //LevelEditorManager::Get().RequestPrimitiveLoad(PrimitiveType::Plane);
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Camera")) {
                if (ImGui::MenuItem("Create Camera")) {
                    scene.CreateCamera();
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }
    }

    void HierarchyWindow::OnShutdown()
    {
    }

    void HierarchyWindow::DrawObjectNode(GameObject* obj, const char* filter)
    {
        Scene* scene = SceneManager::Get().GetActiveScene();
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
                scene->DestroyGameObject(obj);
                if (m_SelectedObject) {
                    m_SelectedObject = nullptr;
                    m_Inspector->SetSelectedObject(nullptr);
                }
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

    bool HierarchyWindow::NameMatchesFilter(const std::string& name, const char* filter)
    {
        return std::string(name).find(filter) != std::string::npos;
    }

}
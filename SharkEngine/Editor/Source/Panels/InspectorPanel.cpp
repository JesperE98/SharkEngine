#include "InspectorPanel.h"
#include "Source/Managers/LevelEditorManager.h"

#pragma region Engine Includes
#include <Components/Rendering/MeshRendererComponent.h>
#include <Components/Logic/CameraComponent.h>
#include <Graphics/Resources/PrimitiveMesh.h>
#include <ImGui/imgui.h>
#include <Managers/MeshManager.h>
#include <Math/MathUtils.h>
#pragma endregion

namespace Shark::Editor {

	using Shark::Core::GameObject;
	using Shark::Math::Transform;
	using Shark::Math::Vector3;
	using Shark::Components::MeshRendererComponent;
	using Shark::Components::CameraComponent;
	using Shark::Graphics::Mesh;
	using Shark::Graphics::PrimitiveType;
	using Shark::Managers::MeshManager;
	using Shark::Editor::LevelEditorManager;

	InspectorPanel::~InspectorPanel()
	{
	}

	void InspectorPanel::OnInit() {

	}

	void InspectorPanel::OnRenderPanel(float deltaTime)
	{
		if (!m_IsVisible) return;

		ImGui::Begin(m_Name.c_str(), &m_IsVisible);

		if (!m_SelectedObject) {
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Select an object.");
			ImGui::End();
			return;
		}

		// --- 1. Draw Global Object Info (Name)
		DrawObjectName(m_SelectedObject);

		// --- 2. Draw Components (Delegated)
		DrawTransform(m_SelectedObject->GetTransform());

		ImGui::Separator();

		// --- 3. Draw Components via Delegation
		// Here is where to add Components in the future to the UI.
		DrawComponentUI<MeshRendererComponent>("Mesh Renderer", m_SelectedObject);
		DrawComponentUI<CameraComponent>("Camera", m_SelectedObject);

		// --- 4. Footer
		DrawAddComponentButton(m_SelectedObject);

		ImGui::End();
	}

	void InspectorPanel::OnShutdown()
	{
		m_SelectedObject = nullptr;
	}

	void InspectorPanel::SetVisible(bool value)
	{
		m_IsVisible = value;
	}
#pragma region Helper Logic
	void InspectorPanel::DrawObjectName(GameObject* obj)
	{
		if (obj) {
			static char nameBuffer[128];
			strncpy_s(nameBuffer, obj->GetName().c_str(), sizeof(nameBuffer));
			nameBuffer[sizeof(nameBuffer - 1)] = '\0'; // Ensures null-termination
			ImGui::Text("Name");
			ImGui::SameLine();

			if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
				obj->SetName(std::string(nameBuffer));
				SE_LOG(Editor, "InspectorPanel::OnRenderPanel() - Object renamed to {}", nameBuffer);
			}
		}
	}

	void InspectorPanel::DrawTransform(Transform& transform)
	{
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::BeginTable("TransformTable", 2, ImGuiTableFlags_SizingStretchProp)) {

				// Position
				float pos[3] = { transform.position.x, transform.position.y, transform.position.z };
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Position");
				ImGui::TableSetColumnIndex(1);
				if (ImGui::DragFloat3("##Position", pos, 0.01f)) {
					transform.position = { pos[0], pos[1], pos[2] };
				}


				// Rotation
				Vector3 euler = Math::ToEulerDegrees(transform.rotation);
				float rot[3] = { euler.x, euler.y, euler.z };
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Rotation");
				ImGui::TableSetColumnIndex(1);
				if (ImGui::DragFloat3("##Rotation", rot, 0.01f)) {
					Vector3 updatedEuler = { rot[0], rot[1], rot[2] };
					transform.rotation = Math::FromEulerDegrees(updatedEuler);
				}

				// Scale
				float scale[3] = { transform.scale.x, transform.scale.y, transform.scale.z };
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Scale");
				ImGui::TableSetColumnIndex(1);
				if (ImGui::DragFloat3("##Scale", scale, 0.01f)) {
					transform.scale = { scale[0], scale[1], scale[2] };
				}

				ImGui::EndTable();
			}
		}
	}

	void InspectorPanel::DrawAddComponentButton(GameObject* obj)
	{
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
			ImGui::OpenPopup("AddComponentPopup");
		}

		if (ImGui::BeginPopup("AddComponentPopup")) {
			if (ImGui::MenuItem("Camera")) {
				if (!obj->GetComponent<CameraComponent>()) {
					obj->AddComponent<CameraComponent>(45.0f, 1.77f, 0.1f, 1000.0f);
				}
			}
			if (ImGui::MenuItem("Mesh Renderer")) {
				if (!obj->GetComponent<MeshRendererComponent>()) {

					auto* mesh = MeshManager::Get().LoadMesh(PrimitiveType::Cube);
					obj->AddComponent<MeshRendererComponent>(mesh, nullptr);
				}
			}
			ImGui::EndPopup();
		}
	}

	template<>
	void InspectorPanel::OnComponentUI<MeshRendererComponent>(MeshRendererComponent* renderer) {
		// Primitive Enum dropdown
		const char* primitiveNames[] = { "Select Primitive", "Cube", "Sphere", "Plane", "Cylinder", "Cone" };
		static int selectedPrim = 0;

		if (ImGui::Combo("Primitive", &selectedPrim, primitiveNames, IM_ARRAYSIZE(primitiveNames))) {
			if (selectedPrim > 0) {
				PrimitiveType type = static_cast<PrimitiveType>(selectedPrim);
				
				LevelEditorManager::Get().RequestPrimitiveLoad(type);
			}
		}

		ImGui::Spacing();

		// String Path Input
		static char pathBuffer[256] = "";
		ImGui::Text("Mesh Path (.obj)");
		if (ImGui::InputText("##MeshPath", pathBuffer, sizeof(pathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
			
			// MeshManager handles the cache lookup automatically
			LevelEditorManager::Get().RequestModelLoad(std::string(pathBuffer));
			SE_LOG(Editor, "InspectorPanel - Mesh path swapped to: {}", pathBuffer);

			memset(pathBuffer, 0, sizeof(pathBuffer)); // Clear on success
		}
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Enter path and press ENTER to load.");
		}

		// Texture path
		ImGui::Spacing();
		static char texPathBuffer[256] = "";
		ImGui::Text("Texture Path (.png/.jpg)");

		if (ImGui::InputText("##TexPath", texPathBuffer, sizeof(texPathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
			// We send a request to LevelEditorManager
			LevelEditorManager::Get().RequestTextureLoad(std::string(texPathBuffer));
			SE_LOG(Editor, "InspectorPanel - Requesting texture swap: {}", texPathBuffer);
		}
	}

	template<>
	void InspectorPanel::OnComponentUI<CameraComponent>(CameraComponent* cam) {
		bool changed = false;

		if (ImGui::BeginTable("CameraTable", 2, ImGuiTableFlags_SizingStretchProp)) {
			// Field of View
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Field Of View");
			ImGui::TableSetColumnIndex(1);
			if (ImGui::SliderFloat("##FOV", &cam->FOV, 1.0f, 120.0f)) {
				changed = true;
			}

			// Near Clip
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Near Clip");
			ImGui::TableSetColumnIndex(1);
			if (ImGui::DragFloat("##Near", &cam->NearClip, 0.01f, 0.001f, 10.0f)) {
				changed = true;
			}

			// Far Clip
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Far Clip");
			ImGui::TableSetColumnIndex(1);
			if (ImGui::DragFloat("##Far", &cam->FarClip, 1.0f, 0.1f, 1000.0f)) {
				changed = true;
			}

			// Aspect Ratio
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Aspect Ratio");
			ImGui::TableSetColumnIndex(1);
			if (ImGui::DragFloat("##Aspect", &cam->AspectRatio, 0.01f, 0.1f, 5.0f)) {
				changed = true;
			}

			ImGui::EndTable();
		}
		if (changed) {
			cam->UpdateProjectionMatrix();
		}
	}
#pragma endregion
}

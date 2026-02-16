#include "InspectorPanel.h"

#pragma region Engine Includes
#include <Components/Rendering/MeshRendererComponent.h>
#include <Components/Logic/CameraComponent.h>
#include <Core/Utilities/Debug.h>
#include <Core/GameObject.h>
#include <Graphics/Resources/PrimitiveMesh.h>
#include <ImGui/imgui.h>
#include <Managers/MeshManager.h>
#include <Math/MathUtils.h>
#include <Math/Transform.h>
#include <Math/Vector3.h>
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

	InspectorPanel::~InspectorPanel()
	{
	}

	void InspectorPanel::OnInit() {

	}

	void InspectorPanel::OnRenderPanel(float deltaTime)
	{
		if (!m_IsVisible) return;

		ImGui::Begin(m_Name.c_str(), &m_IsVisible);

		if (m_SelectedObject) {

			static char nameBuffer[128];
			strncpy_s(nameBuffer, m_SelectedObject->GetName().c_str(), sizeof(nameBuffer));
			nameBuffer[sizeof(nameBuffer - 1)] = '\0'; // Ensures null-termination
			ImGui::Text("Name");
			ImGui::SameLine();

			if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
				m_SelectedObject->SetName(std::string(nameBuffer));
				SE_LOG(Editor, "InspectorPanel::OnRenderPanel() - Object renamed to {}", nameBuffer);
			}

			Transform& transform = m_SelectedObject->GetTransform();

			// Position
			float pos[3] = { transform.position.x, transform.position.y, transform.position.z };

			// Rotation
			Vector3 euler = Math::ToEulerDegrees(transform.rotation);
			float rot[3] = { euler.x, euler.y, euler.z };

			// Scale
			float scale[3] = { transform.scale.x, transform.scale.y, transform.scale.z };

			if (ImGui::BeginTable("InspectorTable", 2, ImGuiTableFlags_SizingStretchProp)) {

				// Position
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Position");
				ImGui::TableSetColumnIndex(1);
				ImGui::DragFloat3("##Position", pos, 0.01f);

				// Rotation
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Rotation");
				ImGui::TableSetColumnIndex(1);
				ImGui::DragFloat3("##Rotation", rot, 0.01f);

				// Scale
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Scale");
				ImGui::TableSetColumnIndex(1);
				ImGui::DragFloat3("##Scale", scale, 0.01f);

				ImGui::EndTable();
			}

			transform.position = { pos[0], pos[1], pos[2] };
			Vector3 updatedEuler = { rot[0], rot[1], rot[2] };
			transform.rotation = Math::FromEulerDegrees(updatedEuler);
			transform.scale = { scale[0], scale[1], scale[2] };

			ImGui::Separator();

			// --------- MESH RENDERER COMPONENT SECTION ----------

			auto* renderer = m_SelectedObject->GetComponent<MeshRendererComponent>();
			if (renderer) {
				if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {

					// Primitive Enum dropdown
					const char* primitiveNames[] = { "Select Primitive", "Cube", "Sphere", "Plane", "Cylinder", "Cone" };
					static int selectedPrim = 0;

					if (ImGui::Combo("Primitive", &selectedPrim, primitiveNames, IM_ARRAYSIZE(primitiveNames))) {
						if (selectedPrim > 0) {
							PrimitiveType type = static_cast<PrimitiveType>(selectedPrim);
							Mesh* newMesh = MeshManager::Get().LoadMesh(type);

							if (newMesh) renderer->SetMesh(newMesh);
						}
					}

					ImGui::Spacing();

					// String Path Input
					static char pathBuffer[256] = "";
					ImGui::Text("Mesh Path (.obj)");
					if (ImGui::InputText("##MeshPath", pathBuffer, sizeof(pathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
						// MeshManager handles the cache lookup automatically
						Mesh* newMesh = MeshManager::Get().LoadMesh(pathBuffer);

						if (newMesh) {
							renderer->SetMesh(newMesh);
							SE_LOG(Editor, "InspectorPanel - Mesh path swapped to: {}", pathBuffer);
						}
						memset(pathBuffer, 0, sizeof(pathBuffer)); // Clear on success
					}
					ImGui::SameLine();
					ImGui::TextDisabled("(?)");
					if (ImGui::IsItemHovered()) {
						ImGui::SetTooltip("Enter path and press ENTER to load.");
					}
				}
			}

			// --------- CAMERA COMPONENT SECTION ----------

			auto* camera = m_SelectedObject->GetComponent<CameraComponent>();
			if (camera) {
				if (ImGui::CollapsingHeader("Camera", ImGuiTableFlags_SizingStretchProp)) {

					bool changed = false;

					if (ImGui::BeginTable("CameraTable", 2, ImGuiTableFlags_SizingStretchProp)) {
						// Field of View
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Field Of View");
						ImGui::TableSetColumnIndex(1);
						if (ImGui::SliderFloat("##FOV", &camera->FOV, 1.0f, 120.0f)) {
							changed = true;
						}

						// Near Clip
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Near Clip");
						ImGui::TableSetColumnIndex(1);
						if (ImGui::DragFloat("##Near", &camera->NearClip, 0.01f, 0.001f, 10.0f)) {
							changed = true;
						}

						// Far Clip
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Far Clip");
						ImGui::TableSetColumnIndex(1);
						if (ImGui::DragFloat("##Far", &camera->FarClip, 1.0f, 0.1f, 1000.0f)) {
							changed = true;
						}

						// Aspect Ratio
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("Aspect Ratio");
						ImGui::TableSetColumnIndex(1);
						if (ImGui::DragFloat("##Aspect", &camera->AspectRatio, 0.01f, 0.1f, 5.0f)) {
							changed = true;
						}

						ImGui::EndTable();
					}
					if (changed) {
						camera->UpdateProjectionMatrix();
					}
				}
			}
		}
		else {
			ImGui::Text("No Object Selected");
		}

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

}

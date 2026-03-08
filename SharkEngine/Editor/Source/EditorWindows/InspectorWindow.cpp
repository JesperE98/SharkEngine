#include "InspectorWindow.h"
#include "Source/Managers/LevelEditorManager.h"

#pragma region Engine Includes
#include <Components/Rendering/MeshRendererComponent.h>
#include <Components/Logic/CameraComponent.h>
#include <Graphics/Resources/PrimitiveMesh.h>
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

	InspectorWindow::~InspectorWindow()
	{
		OnShutdown();
	}

	void InspectorWindow::OnInitialize() {
		m_Name = "Inspector";
		m_bIsVisible = true;
	}

	void InspectorWindow::OnUpdateWindow(float deltaTime)
	{
		if (!m_SelectedObject) {
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Select an object.");
			return;
		}

		// --- 1. Draw Global Object Info (Name)
		DrawObjectName(m_SelectedObject);

		// --- 2. Draw Components (Delegated)
		DrawTransform(m_SelectedObject->GetTransform());

		ImGui::Separator();

		// --- 3. Draw Components via Delegation
		// Here is where to add Components in the future to the UI.
		DrawComponentUI<MeshRendererComponent>("MeshRenderer", m_SelectedObject);
		DrawComponentUI<CameraComponent>("Camera", m_SelectedObject);

		// --- 4. Footer
		DrawAddComponentButton(m_SelectedObject);
	}

	void InspectorWindow::OnShutdown()
	{
		if(m_SelectedObject) m_SelectedObject = nullptr;
	}

#pragma region Helper Logic
	void InspectorWindow::DrawObjectName(GameObject* obj)
	{
		if (obj) {
			static char nameBuffer[128];
			strncpy_s(nameBuffer, obj->GetName().c_str(), sizeof(nameBuffer));
			nameBuffer[sizeof(nameBuffer - 1)] = '\0'; // Ensures null-termination
			ImGui::Text("Name");
			ImGui::SameLine();

			if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
				obj->SetName(std::string(nameBuffer));
				SE_LOG(Editor, "InspectorWindow::OnRenderPanel() - Object renamed to {}", nameBuffer);
			}
		}
	}

	void InspectorWindow::DrawTransform(Transform& transform)
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

	void InspectorWindow::DrawAddComponentButton(GameObject* obj)
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
			if (ImGui::MenuItem("MeshRenderer")) {
				if (!obj->GetComponent<MeshRendererComponent>()) {

					auto* mesh = MeshManager::Get().LoadMesh(PrimitiveType::Cube);
					obj->AddComponent<MeshRendererComponent>(mesh, nullptr);
				}
			}
			ImGui::EndPopup();
		}
	}

	template<>
	void InspectorWindow::OnComponentUI<MeshRendererComponent>(MeshRendererComponent* renderer) {
		// Mesh Path Input
		static char pathBuffer[256] = "";
		ImGui::Text("Mesh Path (.obj)");
		if (ImGui::InputText("##MeshPath", pathBuffer, sizeof(pathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
			// MeshManager handles the cache lookup automatically
			LevelEditorManager::Get().RequestModelLoad(std::string(pathBuffer));
			SE_LOG(Editor, "InspectorWindow - Mesh path swapped to: {}", pathBuffer);

			memset(pathBuffer, 0, sizeof(pathBuffer)); // Clear on success
		}

		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Enter path and press ENTER to load.");
		}

		ImGui::Spacing();

		const auto& material = renderer->GetMaterial();
		if (material) {
			ImGui::Text("Material Properties");

			// 1. Diffuse Map (Albedo)
			static char diffuseBuffer[256] = "";
			ImGui::Text("Diffuse Map");
			if (ImGui::InputText("##DiffusePath", diffuseBuffer, sizeof(diffuseBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
				LevelEditorManager::Get().RequestDiffTextureLoad(static_cast<std::string>(diffuseBuffer));
				SE_LOG(Editor, "InspectorWindow - Requesting texture swap: {}", diffuseBuffer);
				memset(diffuseBuffer, 0, sizeof(diffuseBuffer)); // Clear on success
			}
			ImGui::TextDisabled("Current: %s", material->GetTexturePath().c_str());

			ImGui::Spacing();

			// 2. Specular Map
			static char specularBuffer[256] = "";
			ImGui::Text("Specular Map");
			if (ImGui::InputText("##SpecularPath", specularBuffer, sizeof(specularBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
				material->SetSpecularTexture(static_cast<std::string>(specularBuffer));
				SE_LOG(Editor, "Specular texture updated to: {}", specularBuffer);
				memset(specularBuffer, 0, sizeof(specularBuffer)); // Clear on success
			}
			ImGui::TextDisabled("Current: %s", material->GetSpecularTexturePath().c_str());

			ImGui::Spacing();

			// 3. Shininess factor
			float shininess = material->GetShininess();
			ImGui::Text("Shininess Strength");
			if (ImGui::SliderFloat("", &shininess, 1.0f, 128.0f)) {
				material->SetShininess(shininess);
			}
		}
		else {
			ImGui::Text("No material assigned.");
		}
	}

	template<>
	void InspectorWindow::OnComponentUI<CameraComponent>(CameraComponent* cam) {
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

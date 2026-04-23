#include "InspectorWindow.h"
#include "Managers/LevelEditorManager.h"

#pragma region Engine Includes
#include <Components/Rendering/MeshRendererComponent.h>
#include <Components/Rendering/LightComponent.h>
#include <Components/Logic/CameraComponent.h>
#include <Components/Physics/AABBComponent.h>
#include <Components/Physics/RigidbodyComponent.h>
#include <Components/PlayerController.h>
#include <Graphics/Resources/PrimitiveMesh.h>
#include <Graphics/Resources/MeshManager.h>
#include <Math/MathUtils.h>
#pragma endregion

namespace Shark::Editor {

	using Core::GameObject;
	using Math::Transform;
	using Math::Vector2;
	using Math::Vector3;
	using Components::MeshRendererComponent;
	using Components::CameraComponent;
	using Components::LightComponent;
	using Components::LightType;
	using Components::AABBComponent;
	using Components::RigidbodyComponent;
	using Components::PlayerController;
	using Graphics::Mesh;
	using Graphics::PrimitiveType;
	using Resources::MeshManager;
	using Editor::LevelEditorManager;

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
		DrawComponentUI<LightComponent>("Light", m_SelectedObject);
		DrawComponentUI<AABBComponent>("AABB Collider", m_SelectedObject);
		DrawComponentUI<RigidbodyComponent>("Rigidbody", m_SelectedObject);
		DrawComponentUI<PlayerController>("Player Controller", m_SelectedObject);

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
				SE_LOG(Editor, "Object renamed to {}", nameBuffer);
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
				float rot[3] = { transform.eulerAngle.x, transform.eulerAngle.y, transform.eulerAngle.z };
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("Rotation");
				ImGui::TableSetColumnIndex(1);

				if (ImGui::DragFloat3("##Rotation", rot, 0.01f)) {
					transform.eulerAngle = { rot[0], rot[1], rot[2] };
					transform.UpdateQuaternion();
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
			if (ImGui::MenuItem("LightComponent")) {
				if (!obj->GetComponent<LightComponent>()) {
					obj->AddComponent<LightComponent>();
				}
			}
			if (ImGui::MenuItem("AABB Collider")) {
				if (!obj->GetComponent<AABBComponent>())
					obj->AddComponent<AABBComponent>(Vector3(0.5f, 0.5f, 0.5f), true);
			}
			if (ImGui::MenuItem("Rigidbody")) {
				if (!obj->GetComponent<RigidbodyComponent>())
					obj->AddComponent<RigidbodyComponent>();
			}
			if (ImGui::MenuItem("PlayerController")) {
				if (!obj->GetComponent<PlayerController>())
					obj->AddComponent<PlayerController>();
			}
			ImGui::EndPopup();
		}
	}

#pragma endregion

#pragma region Template Functions

	template<>
	void InspectorWindow::OnComponentUI<MeshRendererComponent>(MeshRendererComponent* renderer) {
		// Mesh Path Input
		static char pathBuffer[256] = "";
		ImGui::Text("Mesh Path (.obj)");
		if (ImGui::InputText("##MeshPath", pathBuffer, sizeof(pathBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
			// MeshManager handles the cache lookup automatically
			LevelEditorManager::Get().RequestModelLoad(std::string(pathBuffer));
			SE_LOG(Editor, "Mesh path swapped to: {}", pathBuffer);

			memset(pathBuffer, 0, sizeof(pathBuffer)); // Clear on success
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_MODEL")) {
				const char* path = static_cast<const char*>( payload->Data );
				LevelEditorManager::Get().RequestModelLoad(std::string(path));
				SE_LOG(Editor, "Dropped model: {}", path);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::SameLine();
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Enter path and press ENTER, or drag a model from the Content Browser.");
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
				SE_LOG(Editor, "Requesting texture swap: {}", diffuseBuffer);
				memset(diffuseBuffer, 0, sizeof(diffuseBuffer)); // Clear on success
			}

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_TEXTURE")) {
					const char* path = static_cast<const char*>( payload->Data );
					LevelEditorManager::Get().RequestDiffTextureLoad(std::string(path));
					SE_LOG(Editor, "Dropped diffuse texture: {}", path);
				}
				ImGui::EndDragDropTarget();
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

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_TEXTURE")) {
					const char* path = static_cast<const char*>( payload->Data );
					material->SetSpecularTexture(std::string(path));
					SE_LOG(Editor, "Dropped model: {}", path);
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::TextDisabled("Current: %s", material->GetSpecularTexturePath().c_str());
			ImGui::Spacing();

			// 3. Shininess factor
			float shininess = material->GetShininess();
			ImGui::Text("Shininess Strength");
			if (ImGui::SliderFloat("", &shininess, 1.0f, 128.0f)) {
				material->SetShininess(shininess);
			}

			bool bUseMipMaps = material->GetUseMipMaps();
			if (ImGui::Checkbox("Use Mip Maps", &bUseMipMaps)) {
				material->SetUseMipMaps(bUseMipMaps);
			}

			Vector2 tiling = material->GetTiling();
			ImGui::Text("Texture Tiling");
			if (ImGui::DragFloat2("##Tiling", &tiling.x, 0.1f, 0.1f, 100.0f)) {
				material->SetTiling(tiling);
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

	template<>
	void InspectorWindow::OnComponentUI<LightComponent>(LightComponent* light) {
		const char* types[] = { "Directional Light", "Point Light" };
		int currentType = static_cast<int>(light->Type);

		if (ImGui::Combo("Light Type", &currentType, types, IM_ARRAYSIZE(types))) {
			light->Type = static_cast<LightType>(currentType);
		}

		ImGui::ColorEdit3("Color", &light->Color.x);
		ImGui::DragFloat("Intensity", &light->Intensity, 0.1f, 0.1f, 100.0f);

		if (light->Type == LightType::Point) {
			ImGui::DragFloat("Range", &light->Range, 0.1f, 0.1f, 500.0f);
		}
	}

	template<>
	void InspectorWindow::OnComponentUI<AABBComponent>(AABBComponent* aabb) {
		float ext[3] = { aabb->m_Extents.x, aabb->m_Extents.y, aabb->m_Extents.z };

		if (ImGui::DragFloat3("Extents", ext, 0.01f, 0.01f, 100.0f)) {
			aabb->m_Extents = { ext[0], ext[1], ext[2] };
		}

		float off[3] = { aabb->m_Offset.x, aabb->m_Offset.y, aabb->m_Offset.z };
	
		if (ImGui::DragFloat3("Offset", off, 0.01f)) {
			aabb->m_Offset = { off[0], off[1], off[2] };
		}

		ImGui::Checkbox("Is Static", &aabb->bIsStatic);
	}

	template<>
	void InspectorWindow::OnComponentUI<RigidbodyComponent>(RigidbodyComponent* physics) {
		ImGui::DragFloat("Mass",			&physics->mass,			0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("Gravity",			&physics->gravity,		0.1f, -100.0f, 0.0f);
		ImGui::DragFloat("Drag",			&physics->drag,			0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Bounce Factor",	&physics->bounceFactor,	0.01f, 0.0f, 1.0f);
		ImGui::Checkbox("Use Gravity",		&physics->bUseGravity);

		// Read-only runtime info
		ImGui::Separator();
		ImGui::TextDisabled("Velocity: %.2f, %.2f, %.2f",
			physics->velocity.x,
			physics->velocity.y,
			physics->velocity.z
		);

		ImGui::TextDisabled("Grounded: %s", physics->bIsGrounded ? "Yes" : "No");
	}

	template<>
	void InspectorWindow::OnComponentUI<PlayerController>(PlayerController* pc) {
		ImGui::DragFloat("Move Speed",		&pc->moveSpeed,		0.1f,	0.001f, 50.0f);
		ImGui::DragFloat("Jump Force",		&pc->jumpForce,		0.1f,	0.001f, 50.0f);
		ImGui::DragFloat("Dash Force",		&pc->dashForce,		0.1f,	0.001f, 50.0f);
		ImGui::DragFloat("Dash Cooldown",	&pc->dashCooldown,	0.05f,	0.0f,	5.0f);
	}

#pragma endregion
}

#include "InspectorPanel.h"
#include <string.h>
#include <ImGui/imgui.h>
#include <Core/Components/Transform.h>
#include <Core/Math/MathUtils.h>
#include <Core/Math/Vector3.h>
#include <SharkEngine/Core/Components/MeshRendererComponent.h>
#include <SharkEngine/Source/Graphics/Resources/PrimitiveMesh.h>
#include <SharkEngine/Source/Graphics/Resources/MeshManager.h>

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
		ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer));

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
						std::cout << "Swapped to:" << pathBuffer << std::endl;
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

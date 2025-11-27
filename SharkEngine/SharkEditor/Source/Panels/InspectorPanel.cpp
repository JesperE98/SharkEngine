#include "InspectorPanel.h"

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
		euler = { rot[0], rot[1], rot[2] };
		transform.scale = { scale[0], scale[1], scale[2] };
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

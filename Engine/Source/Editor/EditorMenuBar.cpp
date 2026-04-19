#include "EditorMenuBar.h"
#include "Managers/WindowManager.h"

#include <Core/Engine/EngineContext.h>

namespace Shark::Editor {

	void EditorMenuBar::OnImGuiRender()
	{
		// --- COLORS --- 
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Black text for better contrast
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White background for menu bars
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White background for popups
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.9f, 0.9f, 0.9f, 1.0f)); // Light gray for hovered menu items

		// --- STYLE ---
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 4.0f)); // Increased padding for better clickability
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15.0f, 4.0f)); // Increased spacing between items for better readability

		if (ImGui::BeginMenuBar()) {
			DrawFileMenu();
			DrawEditMenu();
			DrawViewMenu();
			DrawWindowsMenu();
			DrawSettingsMenu();
			DrawHelpMenu();
			ImGui::EndMenuBar();
		}

		ImGui::PopStyleVar(2); // Pop FramePadding and ItemSpacing
		ImGui::PopStyleColor(4); // Pop the 5 colors we pushed
	}

	void EditorMenuBar::DrawFileMenu()
	{
		if(ImGui::BeginMenu("File")){
			if (ImGui::MenuItem("New Scene", "Ctrl+N")) { /* TODO: SceneManager::Get().CreateNewScene(); */ }
			if (ImGui::MenuItem("Open Scene", "Ctrl+O")) { /* TODO: Open File Dialog and load scene */ }
			ImGui::Separator();
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* TODO: SceneManager::Get().SaveActiveScene(); */ }
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4")) {
				// Accessing the Engine context to close the app
				/*glfwSetWindowShouldClose(EngineContext::Get().m_Window, true);*/
			}
			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DrawEditMenu()
	{
		if(ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* TODO: Implement Undo functionality */ }
			if (ImGui::MenuItem("Redo", "Ctrl+Y")) { /* TODO: Implement Redo functionality */ }
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Ctrl+X")) { /* TODO: Implement Cut functionality */ }
			if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* TODO: Implement Copy functionality */ }
			if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* TODO: Implement Paste functionality */ }
			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DrawViewMenu()
	{
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Toggle Fullscreen", "F11")) { /* TODO: Implement Fullscreen toggle */ }
			ImGui::Separator();
			if (ImGui::MenuItem("Zoom In", "Ctrl++")) { /* TODO: Implement Zoom In functionality */ }
			if (ImGui::MenuItem("Zoom Out", "Ctrl+-")) { /* TODO: Implement Zoom Out functionality */ }
			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DrawWindowsMenu()
	{
		if(ImGui::BeginMenu("Windows")) {
			WindowManager& wm = WindowManager::Get();
			for (auto* window : wm.GetWindows()) {
				bool isVisible = window->IsVisible();
				if (ImGui::MenuItem(window->GetWindowName().c_str(), nullptr, isVisible)) {
					window->SetWindowVisible(!isVisible);
				}
			}
			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DrawSettingsMenu()
	{
		if (ImGui::BeginMenu("Settings")) {
			if (ImGui::MenuItem("Editor Style")){ /* TODO: Open Style Settings Window */ }
			if (ImGui::MenuItem("Preferences")) { /* TODO: Open Preferences Window */ }
			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DrawHelpMenu()
	{
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Documentation")) { /* TODO: Open Documentation URL */ }
			if (ImGui::MenuItem("Report a Bug")) { /* TODO: Open Bug Report URL */ }
			if (ImGui::MenuItem("About")) { /* TODO: Open About Window */ }
			ImGui::EndMenu();
		}
	}
}
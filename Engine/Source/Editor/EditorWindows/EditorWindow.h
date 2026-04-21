#ifndef IPANEL_H
#define IPANEL_H

#include <string>
#include <ImGui/imgui.h>

namespace Shark::Editor {

	class EditorWindow {
	public:

		virtual ~EditorWindow() = default;

		virtual void OnInitialize() = 0;
		virtual void OnShutdown() = 0;

		void Render(float deltaTime) {
			if (!m_bIsVisible) return;

			if (ImGui::Begin(m_Name.c_str(), &m_bIsVisible, m_WindowFlags)) {
				m_bIsFocused = ImGui::IsWindowFocused();
				OnUpdateWindow(deltaTime);
			}

			ImGui::End();
		};

		const std::string SetWindowName(const std::string& name) { 
			m_Name = name; 
			return m_Name;
		}

		const std::string& GetWindowName() const { return m_Name; }

		const void SetWindowVisible(bool visible) { m_bIsVisible = visible; }
		const bool IsVisible() const { return m_bIsVisible; }

	protected:

		std::string m_Name = std::string();
		bool m_bIsVisible{ true };
		bool m_bIsFocused{ false };
		ImGuiWindowFlags m_WindowFlags{ 0 };

		explicit EditorWindow(const std::string& name = "", bool value = true) : m_Name(name), m_bIsVisible(value){};

		virtual void OnUpdateWindow(float deltaTime) = 0;
	};
}


#endif
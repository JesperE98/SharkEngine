#ifndef IPANEL_H
#define IPANEL_H

#include <Core/Utilities/Debug.h>
#include <ImGui/imgui.h>

namespace Shark::Editor {

	class WindowBase {
	public:

		virtual ~WindowBase() = default;

		virtual void OnInitialize() = 0;
		virtual void OnRenderPanel(float deltaTime) = 0;
		virtual void OnShutdown() = 0;

		const std::string SetWindowName(const std::string& name) { 
			m_Name = name; 
			return m_Name;
		}

		const std::string& GetWindowName() const { return m_Name; }

		const void SetWindowVisible(bool visible) { m_bIsVisible = visible; }
		const bool IsVisible() const { return m_bIsVisible; }

	protected:
		explicit WindowBase(const std::string& name = "", bool value = true) : m_Name(name), m_bIsVisible(value){};

		std::string m_Name = std::string();
		bool m_bIsVisible{true};
	};
}


#endif
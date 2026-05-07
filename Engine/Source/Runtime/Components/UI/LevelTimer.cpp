#include "LevelTimer.h"
#include <ImGui/imgui.h>

namespace Shark::Components {
	void LevelTimer::BeginPlay() {
		m_Elapsed = 0.0f;
		m_Running = true;
	}

	void LevelTimer::Update(float deltaTime) {
		if (m_Running) {
			m_Elapsed += deltaTime;
		}
	}
	void LevelTimer::DrawHUD() {
		ImGuiViewport* vp = ImGui::GetMainViewport();
		const float padding = 16.0f;

		// Top-right corner
		ImVec2 pos = ImVec2(
			vp->WorkPos.x + vp->WorkSize.x - padding,
			vp->WorkPos.y + padding
		);

		ImVec2 pivot = ImVec2(1.0f, 0.0f); // anchor at top-right of the window

		ImGui::SetNextWindowPos(pos, ImGuiCond_Always, pivot);
		ImGui::SetNextWindowBgAlpha(0.4f);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav;

		if (ImGui::Begin("##LevelTimerHUD", nullptr, flags)) {
			int minutes = (int)( m_Elapsed / 60.0f );
			float seconds = m_Elapsed - minutes * 60.0f;
			ImGui::Text("Time: %d:%05.2f", minutes, seconds);
		}
		ImGui::End();
	}
}


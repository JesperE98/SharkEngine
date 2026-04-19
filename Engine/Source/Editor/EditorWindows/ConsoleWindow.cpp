#include "ConsoleWindow.h"

#pragma region Engine includes
#include <Core/Events/SharkEvents.h>
#pragma endregion

namespace Shark::Editor {
	using Core::Debug;
	using Core::LogEntry;
	using Core::SharkEvents;

	void ConsoleWindow::OnInitialize()
	{
		SharkEvents::OnLogAdded().AddListener(this, &ConsoleWindow::OnLogReceived);
	}

	void ConsoleWindow::OnUpdateWindow(float deltaTime)
	{
		if (ImGui::Button("Clear")) {
			Debug::ClearLogHistory();
		}

		ImGui::SameLine();
		static bool autoScroll = true;
		ImGui::Checkbox("Auto-Scroll", &autoScroll);
		ImGui::Separator();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		
		ImGuiListClipper clipper;
		clipper.Begin(static_cast<int>(m_LocalHistory.size()));

		while (clipper.Step()) {
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
				const auto& entry = m_LocalHistory[i];
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.0f)); // White for timestamp
				std::string formattedTime = '[' + entry.RawData.TimeStamp + ']';
				ImGui::TextUnformatted(formattedTime.c_str());
				ImGui::PopStyleColor();

				ImGui::SameLine(0.0f, 5.0f);

				ImGui::PushStyleColor(ImGuiCol_Text, entry.Color);
				ImGui::TextUnformatted(entry.FormattedMessage.c_str());
				ImGui::PopStyleColor();
			}
		}

		// Auto Scroll Logic
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
			ImGui::SetScrollHereY(1.0f);
		}

		ImGui::EndChild();
	}

	void ConsoleWindow::OnShutdown()
	{
	}

	void ConsoleWindow::OnLogReceived(const Shark::Core::LogEntry& entry)
	{
		std::lock_guard<std::mutex> lock(m_LogMutex);
		ConsoleEntry ce;
		ce.RawData = entry;
		ce.Color = GetColorForLogLevel(entry.Level);

		std::string catStr = std::string(Shark::Core::Debug::CategoryToString(entry.Category));

		ce.FormattedMessage = "[" + catStr + "] " + "[" + entry.Level + "] " + entry.Message;
		m_LocalHistory.push_back(ce);
	}

	ImVec4 ConsoleWindow::GetColorForLogLevel(const std::string& level) const
	{
		if (level == "LOG") return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
		if (level == "WARNING") return ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
		if (level == "ERROR") return ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
		if (level == "SUCCESS") return ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
		if (level == "REQUEST" || level == "PROCESS") return ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Cyan
		if (level == "FATAL") return ImVec4(0.5f, 0.0f, 0.0f, 1.0f); // Dark Red

		return ImVec4(1.0, 1.0, 1.0, 1.0);
	}

}

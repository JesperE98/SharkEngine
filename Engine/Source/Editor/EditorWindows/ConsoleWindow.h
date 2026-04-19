#ifndef CONSOLE_PANEL_H
#define CONSOLE_PANEL_H


#include "EditorWindow.h"

#include <mutex>
#include <vector>

namespace Shark::Editor {

	struct ConsoleEntry {
		Core::LogEntry RawData;
		std::string FormattedMessage;
		ImVec4 Color;
	};

	class ConsoleWindow : virtual public EditorWindow
	{
	public:
		ConsoleWindow() : EditorWindow("Console", true) {}
		~ConsoleWindow() override = default;

#pragma region EditorWindow override functions
		void OnInitialize() override;
		void OnUpdateWindow(float deltaTime) override;
		void OnShutdown() override;
#pragma endregion


	private:

		std::vector<ConsoleEntry> m_LocalHistory;	// Local copy of log history for thread safety
		std::mutex m_LogMutex;

		void OnLogReceived(const Shark::Core::LogEntry& entry);
		ImVec4 GetColorForLogLevel(const std::string& level) const;
	};
}
#endif // CONSOLE_PANEL_H
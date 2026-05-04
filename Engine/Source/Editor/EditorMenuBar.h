#ifndef EDITOR_MENUBAR_H
#define EDITOR_MENUBAR_H

#include <string>

namespace Shark::Editor {
	class EditorMenuBar
	{
	public:
		EditorMenuBar() = default;
		~EditorMenuBar() = default;

		void OnImGuiRender();

	private:
		std::string m_CurrentScenePath = "";

		void DrawFileMenu();
		void DrawEditMenu();
		void DrawViewMenu();
		void DrawWindowsMenu();
		void DrawSettingsMenu();
		void DrawHelpMenu();
		void OnSaveScene();
		void OnLoadScene();
		void SaveSceneToPath(const std::string& path);
		void SpawnTestCubes(int count);
		void ClearTestCubes();
	};
}

#endif // EDITOR_MENUBAR_H
#ifndef EDITOR_MENUBAR_H
#define EDITOR_MENUBAR_H

namespace Shark::Editor {
	class EditorMenuBar
	{
	public:
		EditorMenuBar() = default;
		~EditorMenuBar() = default;

		void OnImGuiRender();

	private:
		void DrawFileMenu();
		void DrawEditMenu();
		void DrawViewMenu();
		void DrawWindowsMenu();
		void DrawSettingsMenu();
		void DrawHelpMenu();
		void OnSaveScene();
		void OnLoadScene();
	};
}

#endif // EDITOR_MENUBAR_H
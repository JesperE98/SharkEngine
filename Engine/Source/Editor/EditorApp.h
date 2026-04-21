#ifndef SHARKEDITOR_H
#define SHARKEDITOR_H

namespace Shark::Editor { class WindowManager; class EditorMenuBar; }

namespace Shark {
	class EditorApp
	{
	public:
		EditorApp() = default;
		~EditorApp();

		void Run();

	private:

		Editor::EditorMenuBar* m_MenuBar{ nullptr };

		void CreateEditorWindow();
		void BeginFrame();
		void Render();
	};
}


#endif // SHARKEDITOR_H
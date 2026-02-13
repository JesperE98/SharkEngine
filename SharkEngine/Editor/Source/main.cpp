#include "EditorApp.h"

int main()
{
	Shark::EditorApp* editor = new Shark::EditorApp();
	editor->Run();

	delete editor;
	editor = nullptr;

	return 0;
}
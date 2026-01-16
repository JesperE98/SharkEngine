#include "Core/Editor/Editor.h"	

int main()
{
	Editor* editor = new Editor();
	editor->Run();

	delete editor;
	editor = nullptr;

	return 0;
}
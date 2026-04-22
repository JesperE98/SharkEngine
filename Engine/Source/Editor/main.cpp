#include "Core/Engine/Engine.h"
#include "EditorApp.h"

int main()
{
	Shark::Core::Engine engine;
	engine.Initialize();

	Shark::EditorApp app;
	engine.Run(&app);

	engine.Shutdown();
	return 0;
}
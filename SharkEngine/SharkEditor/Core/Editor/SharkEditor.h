#ifndef SHARKEDITOR_H
#define SHARKEDITOR_H


#include <stdio.h>
#include <iostream>
#include <memory>
#include <vector>
#include <Core/Engine/SharkEngine.h>

#include "Source/Interfaces/IPanel.h"

class HierarchyPanel;
class InspectorPanel;
class SceneViewport;


class SharkEditor
{
public:
	SharkEditor();
	~SharkEditor();

	void Run();

private:
	// Helper function to check for errors
	static void CheckGLErrors(const std::string& context);

	void CreateEditorWindow();
	void InitPanels();
	void BeginFrame();
	void RenderPanels(float deltaTime);
	void RenderSceneViewport();
	void Render();
	std::vector<IPanel*> m_Panels;
	HierarchyPanel* m_HierarchyPanel{ nullptr };
	InspectorPanel* m_InspectorPanel{ nullptr };
	std::unique_ptr<SceneViewport> m_SceneViewport;

};

#endif // SHARKEDITOR_H
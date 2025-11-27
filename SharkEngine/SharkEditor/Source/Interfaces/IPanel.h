#ifndef IPANEL_H
#define IPANEL_H

#include <string>
#include <ImGui/imgui.h>

class IPanel {
public:

	virtual ~IPanel() = default;

	virtual void OnInit() = 0;
	virtual void OnRenderPanel(float deltaTime) = 0;
	virtual void OnShutdown() = 0;
	virtual const std::string& GetName() const = 0;
	virtual bool IsVisible() const = 0;
	virtual void SetVisible(bool visible) = 0;
};

#endif
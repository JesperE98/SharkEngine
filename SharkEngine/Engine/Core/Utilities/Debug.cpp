#include "Debug.h"

namespace Shark::Core {

	std::string_view Debug::CategoryToString(LogCategory category) {
		switch (category) {
		case LogCategory::Engine:		return "Engine";
		case LogCategory::Editor:		return "Editor";
		case LogCategory::Physics:		return "Physics";
		case LogCategory::Rendering:	return "Rendering";
		case LogCategory::OpenGL:		return "OpenGL";
		case LogCategory::IO:			return "IO";
		case LogCategory::Temp:			return "Temp";
		case LogCategory::Material:		return "Material";
		case LogCategory::Messaging:	return "Messaging";
		default:						return "General";
		}
	}
}
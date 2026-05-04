#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <string>

namespace Shark::Utilities {
	class FileDialog {
	public:
		// Returns selected path, or empty string if user cancelled.
		// Filter format: "Scene Files\0*.json\0All Files\0*.*\0"
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}

#endif // FILE_DIALOG_H
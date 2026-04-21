#include "PathManager.h"
#include "Core/Utilities/Debug.h"
#include <filesystem>

namespace Shark::IO {

	PathManager& PathManager::Get()
	{
		static PathManager instance;
		return instance;
	}

	void PathManager::OnInitialize()
	{
		std::filesystem::path currentPath = std::filesystem::current_path();

		// Search upwards to find the "Content" folder inside "Runtime" folder
		for (int i = 0; i < 5; i++) {
			// Updated to look for "Content" folder inside "Runtime"
			if (std::filesystem::exists(currentPath / "Content")) {
				// Found the folder containing "Runtime"
				// Sets the root folder to "Runtime/" so all future paths start from there
				m_RootPath = currentPath.make_preferred().string() + '\\';

				SE_SUCC(IO, "PathManager::OnInitialize() - Engine Root Found: {}", m_RootPath);
				return;
			}
			currentPath = currentPath.parent_path();
		}

		m_RootPath = "/.";
		SE_ERR(IO, "PathManager::OnInitialize() - Could not find Content folder! Fallback to /.");
	}

	std::string PathManager::GetPath(PathCategory category, const std::string& relativePath) const
	{
		switch (category) {
		case PathCategory::Root:	return m_RootPath + relativePath;
		case PathCategory::Content:	return m_RootPath + "Content/" + relativePath;
		case PathCategory::Shaders:	return m_RootPath + "Shaders/" + relativePath;
		}
		return m_RootPath + relativePath;
	}

	const std::string& PathManager::GetRootPath() const
	{
		return m_RootPath;
	}

}
#include "PathManager.h"
#include "Debug.h"
#include <filesystem>
PathManager& PathManager::GetInstance()
{
	static PathManager instance;
	return instance;
}

void PathManager::Initialize()
{
	std::filesystem::path currentPath = std::filesystem::current_path();

	// Search upwards to find the "Content" folder inside "Runtime" folder
	for (int i = 0; i < 5; i++) {
		// Updated to look for "Content" folder inside "Runtime"
		if (std::filesystem::exists(currentPath / "Content")) {
			m_RootPath = currentPath.string() + '/';
			SHARK_LOG(IO, "PathManager::Initialize() - Engine Root Found: {}", m_RootPath);
			return;
		}
		currentPath = currentPath.parent_path();
	}

	m_RootPath = "/.";
	SHARK_WARN(IO, "PathManager::Initialize() - Could not find Content folder! Fallback to /.");
}

std::string PathManager::GetContentPath(const std::string& relativePath) const
{
	return m_RootPath + "Content/" + relativePath;
}

const std::string& PathManager::GetRootPath() const
{
	return m_RootPath;
}

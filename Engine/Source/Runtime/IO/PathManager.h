#ifndef PATHMANAGER_H
#define PATHMANAGER_H

#include <string>

namespace Shark::Managers {

	class PathManager
	{
	public:
		// Delete copy constructor and assignment operator
		PathManager(const PathManager&) = delete;
		PathManager& operator=(const PathManager&) = delete;

		// Global access point
		static PathManager& Get();

		// Call this once during Engine::OnInitialize()
		void OnInitialize();

		// Helper to get full paths
		std::string GetContentPath(const std::string& relativePath) const;

		// Get root directory itself
		const std::string& GetRootPath() const;

	private:
		// Private constructor for Singleton
		PathManager() = default;

		std::string m_RootPath = "";
	};
}
#endif // PATHMANAGER_H
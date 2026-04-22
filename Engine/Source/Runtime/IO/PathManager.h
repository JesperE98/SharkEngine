#ifndef PATHMANAGER_H
#define PATHMANAGER_H

#include <string>

namespace Shark::IO {

	enum class PathCategory : uint8_t {
		Root,
		Content,
		Shaders
	};

	class PathManager
	{
	public:
		// Global access point
		static PathManager& Get();

		void Initialize();

		// Helper to get full paths
		std::string GetPath(PathCategory category, const std::string& relativePath = "") const;
		// Get root directory itself
		const std::string& GetRootPath() const;

		// Delete copy constructor and assignment operator
		PathManager(const PathManager&) = delete;
		PathManager& operator=(const PathManager&) = delete;
	private:
		// Private constructor for Singleton
		PathManager() = default;

		std::string m_RootPath = "";
	};
}
#endif // PATHMANAGER_H
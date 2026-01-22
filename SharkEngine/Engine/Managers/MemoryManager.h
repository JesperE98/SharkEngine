#ifndef ENGINE_CORE_UTILITIES_MEMORY_H
#define ENGINE_CORE_UTILITIES_MEMORY_H

namespace Shark::Managers {

	class MemoryManager
	{
	public:
		// Delete copy constructor and assignment operator
		MemoryManager(const MemoryManager&) = delete;
		MemoryManager& operator=(const MemoryManager&) = delete;

		// Global access point
		static MemoryManager& GetInstance();

		static void CheckMemoryStatus();

	private:
		// Private constructor for Singleton
		MemoryManager() = default;

		static void PrintMemoryStatus(const char* value);
	};
}
#endif // ENGINE_CORE_UTILITIES_MEMORY_H
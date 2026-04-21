#ifndef ENGINE_CORE_UTILITIES_MEMORY_H
#define ENGINE_CORE_UTILITIES_MEMORY_H

namespace Shark::Memory {

	class MemoryManager
	{
	public:

		// Global access point
		static MemoryManager& Get();

		void CheckMemoryStatus();
		unsigned long long GetAvailableMemory() const { return m_AvailableMemory / (1024 * 1024); }
		unsigned long long GetTotalMemory() const { return m_TotalMemory / (1024 * 1024); }

	private:

		// Private constructor for Singleton
		MemoryManager() = default;
		~MemoryManager() = default;

		// Delete copy constructor and assignment operator
		MemoryManager(const MemoryManager&) = delete;
		MemoryManager& operator=(const MemoryManager&) = delete;

		unsigned long long m_AvailableMemory{ 0 };
		unsigned long long m_TotalMemory{ 0 };

		static void PrintMemoryStatus(const char* value);
	};
}
#endif // ENGINE_CORE_UTILITIES_MEMORY_H
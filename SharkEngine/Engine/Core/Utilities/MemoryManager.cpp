#include "MemoryManager.h"
#include "Debug.h"

MemoryManager& MemoryManager::GetInstance()
{
	static MemoryManager instance;
	return instance;
}

void MemoryManager::CheckMemoryStatus()
{
	MEMORYSTATUSEX memInfo{};
	memInfo.dwLength = sizeof(memInfo);

	GlobalMemoryStatusEx(&memInfo);

	SHARK_LOG(Engine, "MemoryManager::CheckMemoryStatus - There is {} MB of physical memory available out of {} MB.",
		memInfo.ullAvailPhys / (1024 * 1024), memInfo.ullTotalPhys / (1024 * 1024));
}

void MemoryManager::PrintMemoryStatus(const char* value)
{
}

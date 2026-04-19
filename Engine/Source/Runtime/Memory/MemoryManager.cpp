#include "MemoryManager.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Shark::Managers {
	MemoryManager& MemoryManager::Get()
	{
		static MemoryManager instance;
		return instance;
	}

	void MemoryManager::CheckMemoryStatus()
	{
		MEMORYSTATUSEX memInfo{};
		memInfo.dwLength = sizeof(memInfo);

		if (GlobalMemoryStatusEx(&memInfo)) {
			// Store the values so the UI can read them
			m_AvailableMemory = memInfo.ullAvailPhys;
			m_TotalMemory = memInfo.ullTotalPhys;
		}

		memInfo.ullAvailPhys / (1024 * 1024), memInfo.ullTotalPhys / (1024 * 1024);
	}

	void MemoryManager::PrintMemoryStatus(const char* value)
	{
	}

}

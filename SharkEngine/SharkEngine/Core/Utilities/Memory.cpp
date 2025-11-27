#include "Memory.h"
#include <Windows.h>
#include <iostream>

void Memory::CheckMemoryStatus()
{
	MEMORYSTATUSEX memInfo{};
	memInfo.dwLength = sizeof(memInfo);

	GlobalMemoryStatusEx(&memInfo);

	std::cout << "There is " << memInfo.ullAvailPhys / (1024 * 1024) << " MB of physical memory available out of " 
		<< memInfo.ullTotalPhys / (1024 * 1024) << " MB." << std::endl;
}

#pragma once

class Memory
{
public:
	static void CheckMemoryStatus();

private:
	static void PrintMemoryStatus(const char* value);
};


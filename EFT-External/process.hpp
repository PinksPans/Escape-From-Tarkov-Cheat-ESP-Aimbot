#pragma once
#include <Windows.h>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

class Process;
class ProcessModule;

class Process
{
public:
	static uint32_t FindByWindowName(const wchar_t* windowName, HWND* windowHandle);
	static uint32_t FindByProcessName(const wchar_t* processName);

	Process();
	~Process();

	void SetHandle(HANDLE processHandle);
	bool Open(const uint32_t processId, const uint32_t accessRights = PROCESS_ALL_ACCESS);
	void Close();

	uint32_t GetId();
	uintptr_t GetModuleBase(const wchar_t* moduleName, uintptr_t* moduleSize = nullptr);

	bool CopyModule(const wchar_t* moduleName, ProcessModule* module);

	uintptr_t ReadPointerChain(const uintptr_t address, const std::vector<uintptr_t> offsets);

	bool ReadMemory(const uintptr_t address, void* buffer, const size_t size);

	bool WriteMemory(const uintptr_t address, const void* buffer, const size_t size);

	template <typename T> T ReadMemory(uint64_t addr) {
		T val = {};
		if (ReadMemory(addr, (void *)&val, sizeof(T))) {
			return val;
		}
		else {
			return {};
		}
	}
	
	template <typename type>
	bool ReadMemory(const uintptr_t address, type* buffer)
	{
		return ReadMemory(address, buffer, sizeof(type));
	}

	template <typename type, size_t arraylength>
	bool ReadMemory(const uintptr_t address, type (&buffer)[arraylength])
	{
		return ReadMemory(address, buffer, sizeof(buffer));
	}

	template <typename type>
	bool WriteMemory(const uintptr_t address, const type* buffer)
	{
		return WriteMemory(address, buffer, sizeof(type));
	}

	template <typename type, size_t arraylength>
	bool WriteMemory(const uintptr_t address, type(&buffer)[arraylength])
	{
		return WriteMemory(address, buffer, sizeof(buffer));
	}

private:
	HANDLE processHandle;
};


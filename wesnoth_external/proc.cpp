#include <Windows.h>
#include <iostream>
//#include <stdio.h>
#include <vector>
#include <TlHelp32.h>


DWORD GetProcId(const wchar_t* procName) {
	DWORD procId = 0;
	// The first argument is a flag:Includes all processes in the system in the snapshot
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		// The docs say that i need to set the dwSize (DWORD/32int) manually
		// sizeof will return the size in bytes
		procEntry.dwSize = sizeof(procEntry);

		// Process32First: Retrieves information about the first process encountered in a system snapshot.
		if (Process32First(hSnap, &procEntry)) {
			//loop trhu all process
			do {
				// wchar chars string comparrision... case insensitive
				if (!_wcsicmp(procEntry.szExeFile, procName)) {
					procId = procEntry.th32ProcessID;
					break;
				}

			} while (Process32Next(hSnap, &procEntry));

		}

	}

	CloseHandle(hSnap);
	return procId;
}

// A process module represents a. dll or .exe file that is loaded into a particular process.
// Its basically the same process of the above but for modules
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);

		if (Module32First(hSnap, &moduleEntry)) {
			do
			{
				if (!_wcsicmp(moduleEntry.szModule, modName)) {
					modBaseAddr = (uintptr_t)moduleEntry.modBaseAddr;
					break;
				}

			} while (Module32Next(hSnap, &moduleEntry));
		}

	}

	CloseHandle(hSnap);
	return modBaseAddr;
}


uintptr_t FindDMAAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = ptr;

	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), nullptr);
		addr += offsets[i];
	}

	return addr;
}
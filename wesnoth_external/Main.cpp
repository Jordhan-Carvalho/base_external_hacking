#include <iostream>
#include "proc.h"


int main() {
	// Get ProcId of the target process
	DWORD procId = GetProcId(L"wesnoth.exe");

	// Get module base address
	uintptr_t moduleBaseAddress = GetModuleBaseAddress(procId, L"wesnoth.exe");
	std::cout << "MODULE: " << moduleBaseAddress << std::endl;

	//Get handle to process
	HANDLE wesnoth_process = OpenProcess(PROCESS_ALL_ACCESS, true, procId);

	// Resolve base address of the pointer chain
	uintptr_t dynamicPtrBaseAddr = moduleBaseAddress + 0x01150D94;
	std::cout << "DynamicPointerBaseAddres: " << dynamicPtrBaseAddr << std::endl;

	// Resolve pointer chain to gold address
	std::vector<unsigned int> goldOffsets = { 0x1c, 0xc, 0x4, 0x4 };
	uintptr_t goldAddress = FindDMAAddress(wesnoth_process, dynamicPtrBaseAddr, goldOffsets);
	std::cout << "Gold address: 0x" << std::hex << goldAddress << std::endl;

	// Read gold value
	int goldValue = 0;
	ReadProcessMemory(wesnoth_process, (BYTE*)goldAddress, &goldValue, sizeof(goldValue), nullptr);
	std::cout << "Gold value: " << std::dec << goldValue <<  std::endl;

	// Write to it
	int newGoldValue = 555;
	WriteProcessMemory(wesnoth_process, (BYTE*)goldAddress, &newGoldValue, sizeof(newGoldValue), nullptr);

	// Read out again
	ReadProcessMemory(wesnoth_process, (BYTE*)goldAddress, &goldValue, sizeof(goldValue), nullptr);
	std::cout << "New gold value: " << std::dec << goldValue << std::endl;

	return 0;
}
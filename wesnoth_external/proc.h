#pragma once

#include <Windows.h>
//#include <stdio.h>
#include <vector>
//#include <TlHelp32.h>

//wchar_t is a wide character. It is used to represent characters which require more memory to represent them than a regular char. It is, for example, widely used in the Windows API.
DWORD GetProcId(const wchar_t* procName);

/* uintptr_t is an unsigned integer type that is capable of storing a data pointer(whether it can hold a function pointer is unspecified).
Which typically means that it's the same size as a pointer. */
// intptr_t and uintptr_t are used for address arithmetic, or sometimes to store data that may be a pointer or may be an integer.
// Unsigned... sem sinal... sempre positivo https://www.reddit.com/r/AskProgramming/comments/6z54u5/eli5_request_unsigned_versus_signed_int/
// int is 32 bits long. IntPtr is as long as a pointer for your architecture. Therefore, a pointer can be stored into an int only on 32 bit systems, while it can always be stored in an IntPtr.
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t FindDMAAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
// FindWindow, GetWindowThreadProcessId, OpenProcess, ReadProcessMemory, and WriteProcessMemory are all contained inside windows.h
#include <Windows.h>

int main2(int argc, char** argv) {
    /*
        To use ReadProcessMemory and WriteProcessMemory, we require a handle to the Wesnoth process.

        To get this handle, we require a process id. The quickest way to get a process id for a particular
        process is to use GetWindowThreadProcessId.

        GetWindowThreadProcessId requires a window handle (different than a process handle). To get this
        window handle, we use FindWindow.
    */

    // Find our Wesnoth window. Depending on your language settings, this might be different.
    /*first parameter is null to serarch all windows..we have to cast the string to the type.
    A string literal that begins with L, such as L"asdf", is a wide string literal. A wide string literal has type “array of n const
    wchar_t”, where n is the size of the string as defined below; it has static storage durationand is initialized with the given characters.*/
    HWND wesnoth_window = FindWindow(NULL, L"The Battle for Wesnoth - 1.14.9");

    // Get the process id for the Wesnoth process. GetWindowThreadProcessId does not return a process id, but
    // rather fills a provided variable with its value, hence the &.
    DWORD process_id = 0;
    GetWindowThreadProcessId(wesnoth_window, &process_id);

    // Open our Wesnoth process. PROCESS_ALL_ACCESS means we can both read and write to the process. However,
    // it also means that this program must be executed as an administrator to work.
    HANDLE wesnoth_process = OpenProcess(PROCESS_ALL_ACCESS, true, process_id);

    // Read the value at 0x017EED18 and place its value into the variable gold_value. 
    /* An LPCVOID is a 32 - bit pointer to a constant of any type.
    This type is declared as follows :
    typedef const void* LPCVOID;
    https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types */
    // So we can cast the int value... A cast is a special operator that forces one data type to be converted into another.
    DWORD gold_value = 0;
    ULONG_PTR bytes_read = 0;
    ReadProcessMemory(wesnoth_process, (void*)0x017EED18, &gold_value, 4, &bytes_read);

    // Add 0xA90 to the value read from the last step and then read the value at that new address. These 
    // offsets are covered in https://gamehacking.academy/lesson/13
    gold_value += 0xA90;
    ReadProcessMemory(wesnoth_process, (void*)gold_value, &gold_value, 4, &bytes_read);

    // Add 4 to the gold_value, which will then be pointing at the player's current gold address.
    // Write the value of new_gold_value (555) into this address
    gold_value += 4;
    DWORD new_gold_value = 555;
    ULONG_PTR bytes_written = 0;
    WriteProcessMemory(wesnoth_process, (void*)gold_value, &new_gold_value, 4, &bytes_written);

    return 0;
}

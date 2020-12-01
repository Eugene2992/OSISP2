// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <windows.h>
#include <iostream>
#include <algorithm>

#define DLL_EXPORT __declspec(dllexport)

void ChangeString(DWORD pid, std::string oldstr, std::string newstr);

extern "C" int DLL_EXPORT Printer(const WCHAR * string, int number);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            std::wcout << "Hello :)" << std::endl;
            std::string oldstr = "Left";
            std::string newstr = "Right";
            ChangeString(GetCurrentProcessId(), oldstr, newstr);
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void ChangeString(DWORD pid, std::string oldstr, std::string newstr)
{
    std::wcout << "Let's try" << std::endl;
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (processHandle == nullptr)
    {
        std::wcout << "Invalid process" << std::endl;
    }
    else
    {
        MEMORY_BASIC_INFORMATION memoryBasicInformation;
        unsigned char* regionPagesPointerAddress = nullptr;
        bool canRead = true;
        while (canRead)
        {
            if (!VirtualQueryEx(processHandle, regionPagesPointerAddress, &memoryBasicInformation, sizeof(memoryBasicInformation)))
            {
                canRead = false;
            }
            else
            {
                unsigned int regionSize = memoryBasicInformation.RegionSize;
                bool isCommitedPage = memoryBasicInformation.State == MEM_COMMIT;
                if (isCommitedPage)
                {
                    SIZE_T bytesWasRead;
                    std::string buffer;
                    buffer.resize(regionSize);
                    if (regionPagesPointerAddress != NULL)
                    {
                        if (ReadProcessMemory(processHandle, regionPagesPointerAddress, (LPVOID)buffer.data(), regionSize, &bytesWasRead))
                        {
                            std::string::iterator offset = std::search(buffer.begin(), buffer.end(), oldstr.begin(), oldstr.end());
                            if (offset != buffer.end())
                            {
                                LPVOID replaceAddress = regionPagesPointerAddress + std::distance(buffer.begin(), offset);
                                WriteProcessMemory(processHandle, replaceAddress, newstr.data(), newstr.size() + 1, nullptr);
                            }
                        }
                    }
                }
                regionPagesPointerAddress = regionPagesPointerAddress + regionSize;
            }
        }
        CloseHandle(processHandle);
    }
}


int Printer(const WCHAR* string, int number)
{
    std::wcout << string << std::endl;
    return number;
}

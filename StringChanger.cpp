// StringChanger.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <handleapi.h>
#include <securitybaseapi.h>
#include <winnt.h>
#include <processthreadsapi.h>

#define STRING_SIZE 1000

void WriteAllProcesses(HANDLE snaphandle);

DWORD GetProcessID(WCHAR* processName);

const char* dllInjectionPath = "C:/Users/User/Desktop/StringChanger-master/Debug/Injection.dll";;

int main()
{
	HANDLE snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapHandle == INVALID_HANDLE_VALUE)
	{
		std::wcout << "Fatal error :(" << std::endl;
	}
	else
	{
		WCHAR processName[STRING_SIZE];
		DWORD processId;
		HANDLE processHandle;

		std::wcout << "Welcome to string changer!" << std::endl;
		WriteAllProcesses(snapHandle);
		std::wcout << "Write the process to hack..." << std::endl << std::endl;
		std::wcin >> processName;
		CloseHandle(snapHandle);

		processId = GetProcessID(processName);
		if (processId == 0)
			std::wcout << "Invalid name of process" << std::endl;
		else
			std::wcout << "His ID is " << processId << std::endl;

		processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
		if (!processHandle)
		{
			std::wcout << "Failed to open process" << std::endl;
		}
		else
		{
			std::wcout << "Process was opened successfully" << std::endl;
			
			size_t length = strlen(dllInjectionPath) + 1;

			HMODULE hModule = GetModuleHandle(L"kernel32.dll");
			LPVOID LoadLibraryAStartAddress = (LPVOID)GetProcAddress(hModule, "LoadLibraryA");
			LPVOID DllPathRemoteParam = (LPVOID)VirtualAllocEx(processHandle, NULL, length, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (!WriteProcessMemory(processHandle, DllPathRemoteParam, dllInjectionPath, length, NULL))
			{
				std::wcout << "Failed to write in process" << std::endl;
			}
			else
			{
				HANDLE remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryAStartAddress, DllPathRemoteParam, 0, NULL);
				if (remoteThread == NULL)
				{
					std::wcout << "Failed to start thread" << std::endl;
				}
				else
				{
					WaitForSingleObject(remoteThread, INFINITE);
					std::wcout << "DLL was injected successfully" << std::endl;
					CloseHandle(remoteThread);
				}
			}
			
			CloseHandle(processHandle);
		}
	}
}

void WriteAllProcesses(HANDLE snaphandle)
{
	PROCESSENTRY32 processEntry = { 0 };

	if ((snaphandle == INVALID_HANDLE_VALUE))
		return;

	processEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(snaphandle, &processEntry);

	int index = 0;
	do 
	{
		index++;
		std::wcout << index << ". " << processEntry.szExeFile << std::endl;
	} while (Process32Next(snaphandle, &processEntry));	
}

DWORD GetProcessID(WCHAR* processname) 
{
	HANDLE snapHandle;
	PROCESSENTRY32 processEntry = { 0 };

	if ((snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
		return 0;

	processEntry.dwSize = sizeof(PROCESSENTRY32);
	Process32First(snapHandle, &processEntry);

	do {
		if (wcscmp(processEntry.szExeFile, processname) == 0) {
			return processEntry.th32ProcessID;
		}
	} while (Process32Next(snapHandle, &processEntry));

	if (snapHandle != INVALID_HANDLE_VALUE) {
		CloseHandle(snapHandle);
	}
	return 0;
}
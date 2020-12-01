// DllDynamicImporter.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>



typedef int TPrinter(const WCHAR*, int);

int main()
{
    TPrinter* pointerPrinter;
    HMODULE myDllHandle = LoadLibrary(L"Injection.dll");

    if (myDllHandle != 0)
    {
        pointerPrinter = (TPrinter*)GetProcAddress(myDllHandle, "Printer");
        if (pointerPrinter == 0)
        {
            int x = GetLastError();
            std::cout << x;
        }
        else 
        {
            const WCHAR* str = L"It works";
            (pointerPrinter)(str, 8);
        }
        FreeLibrary(myDllHandle);
        Sleep(500);
    }
}
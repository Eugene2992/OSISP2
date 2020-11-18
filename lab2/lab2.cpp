/*
    Лабораторная работа №2. Изучение вывода текста и шрифтов.
    - Разработать программу, которая вписывает в окно текстовую таблицу (N строк на M столбцов)
        таким образом, что все столбцы таблицы равномерно распределяются по ширине окна, 
        а высота строк таблицы подбирается таким образом, чтобы вместить текст каждой ячейки.
    - При изменении размеров окна таблица перерасчитывается и перерисовывается.
*/

#include "framework.h"
#include "lab2.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


// Мои переменные
const int rowNumber = 10;
const int columnNumber = 5;

const wchar_t* Strings[rowNumber][columnNumber] =
{
    {
        L"I",
        L"II",
        L"III",
        L"IV",
        L"V",
    },
    {
       L"One",
       L"Two",
       L"Three",
       L"Four",
       L"Five"
    },
    {L"Один", L"Два", L"Три", L"Четыре", L"Пять" },
    {L"1", L"2", L"3", L"4", L"5"},
    {L"een", L"twee", L"drie", L"vier", L"vijf"},
    {L"uno", L"dos", L"tres", L"cuatro", L"cinco"},
    {L"vienas", L"du", L"trys", L"keturi", L"penki"},
    {L"jeden", L"dwa", L"trzy", L"cztery", L"piêæ"},
    {L"um", L"dois", L"três", L"quatro", L"cinco"},
    {L"001", L"010", L"011", L"100", L"101"},
};

void DrawTable(HWND hWnd);

void DrawLine(HDC hdc, COLORREF color, int x1, int y1, int x2, int y2);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_WINDOWTITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_LAB2));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:
        {
            // Something    
        }
        break;

        case WM_PAINT:
        {
            DrawTable(hWnd);
        }
        break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void DrawTable(HWND hWnd)
{
    RECT wndRect;      // Прямоугольник окна
    RECT rect;         // Прямоугольник ячейки таблицы
    RECT textRect;     // Прямоугольник текста
    PAINTSTRUCT ps;
    HBRUSH brush;

    GetClientRect(hWnd, &wndRect);

    int indent = 2;                                 // Отступ
    int columnSize = wndRect.right / columnNumber;  // Размер одной колонны
    int rowSize = wndRect.bottom / rowNumber;       // Размер одного ряда
    int maxRowHeight = 0;

    
    HDC hdc = BeginPaint(hWnd, &ps);
    COLORREF colorText = RGB(20, 20, 20);    // Белый
    COLORREF colorBack = RGB(255, 255, 255);       // Черный
    COLORREF colorLine = RGB(255, 0, 0);      // Желтый

    brush = CreateSolidBrush(colorBack);
    SelectObject(hdc, brush);
    Rectangle(hdc, wndRect.left, wndRect.top, wndRect.right, wndRect.bottom);   // Задний фон таблицы
    DeleteObject(brush);

    for (int i = 0; i < rowNumber; i++)
    {
        rect.top = maxRowHeight;
        rect.bottom = wndRect.bottom / rowNumber * (i + 1);
        DrawLine(hdc, colorLine, 0, maxRowHeight, wndRect.right, maxRowHeight);

        for (int j = 0; j < columnNumber; j++)
        {
            rect.left = columnSize * j;
            rect.right = wndRect.right / columnNumber * (j + 1);

            DrawLine(hdc, colorLine, rect.left, rect.top, rect.left, rect.bottom);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, colorText);

            textRect.top = rect.top + indent;
            textRect.right = rect.right - indent;
            textRect.left = rect.left + indent;
            textRect.bottom = rect.bottom - indent;

            static LOGFONT lf;                                
            lf.lfCharSet = DEFAULT_CHARSET;
            lf.lfPitchAndFamily = DEFAULT_PITCH;
            strcpy_s((char *)lf.lfFaceName, _countof(lf.lfFaceName), "Times New Roman");
            lf.lfHeight = 17;
            lf.lfWidth = 10; 
            lf.lfWeight = 30; 
            lf.lfEscapement = 0;                          //шрифт без поворота

            HFONT hFont = CreateFontIndirect(&lf);
            SelectObject(hdc, hFont);

            DrawText(hdc, (LPCWSTR)Strings[i][j], -1, &textRect, DT_WORDBREAK | DT_CENTER);
            DeleteObject(hFont);

            DrawLine(hdc, colorLine, rect.right, rect.top, rect.right, rect.bottom);
        }
        maxRowHeight += wndRect.bottom / rowNumber;
    }
    DrawLine(hdc, colorLine, 0, maxRowHeight, wndRect.right, maxRowHeight);

    SetBkMode(hdc, OPAQUE);  // Переустанавить режим фона в состояние по умолчанию. 
    EndPaint(hWnd, &ps);
}

// Нарисовать линию из точки 1 в точку 2
void DrawLine(HDC hdc, COLORREF color, int x1, int y1, int x2, int y2)
{
    int width = 5;
    HPEN pen = CreatePen(PS_INSIDEFRAME, width, color);
    SelectObject(hdc, pen);
    MoveToEx(hdc, x1, y1, NULL);   // Обновить позицию заданной точки и вернуть ее предыдущее значение
    LineTo(hdc, x2, y2);          // Начертить линию
    DeleteObject(pen);
}
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
const int SHIFT_CODE = 4;
const int startWidth = 300;
const int startHeight = 200;

int spriteX = 100;
int spriteY = 100;
int spriteWidth = startWidth;
int spriteHeight = startHeight;

int windowWidth;
int windowHeight;

BOOL NotCr = TRUE;
BOOL isSprite = TRUE;
BOOL isMove = FALSE;
BOOL isDown = TRUE;
BOOL isRight = TRUE;


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Window Class";

    WNDCLASS windowClass = { };

    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = CLASS_NAME;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    RegisterClass(&windowClass);

    HWND hwnd = CreateWindowEx(
        0,                              
        CLASS_NAME,                     
        L"Lab 1",                       
        WS_OVERLAPPEDWINDOW,            
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,         
        NULL,       
        hInstance,  
        NULL        
    );

    if (hwnd == NULL)
    {
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    GdiplusShutdown(gdiplusToken);
    return 0;
}

void fillWindow(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    RECT clientRect;
    
    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
    if (isSprite) {
        spriteWidth = startWidth;
        spriteHeight = startHeight;
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
        SelectObject(hdc, brush);
        Ellipse(hdc, spriteX, spriteY, spriteX + spriteWidth, spriteY + spriteHeight);
        DeleteObject(brush);
    }
    else {
        Graphics graphics(hdc);
        /*if (NotCr) {
            Bitmap bmp(L"C:\\Users\\User\\Desktop\\Lab1\\Project1\\Project1\\1.bmp");
            NotCr = FALSE;
        }*/
        Image bmp(L"C:\\Users\\User\\Desktop\\Lab1\\Project1\\Project1\\1.png");
        graphics.DrawImage(&bmp, spriteX, spriteY);
        spriteWidth = bmp.GetWidth();
        spriteHeight = bmp.GetHeight();
    }
    GetClientRect(hwnd, &clientRect);
    windowWidth = clientRect.right - clientRect.left;
    windowHeight = clientRect.bottom - clientRect.top;

    EndPaint(hwnd, &ps);
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        fillWindow(hwnd, uMsg, wParam, lParam);
        return 0;
    }
    case WM_KEYDOWN:
    {
        int key = wParam;
        if (key == VK_RETURN) {
            isMove = !isMove;
        }
        if (key == VK_SPACE) {
            isSprite = !isSprite;
        }
        if (!isMove) {
            switch (key)
            {
            case VK_RIGHT:
                spriteX += 5;
                if (spriteX + spriteWidth > windowWidth)
                    spriteX -= 15;
                break;
            case VK_LEFT:
                spriteX -= 5;
                if (spriteX < 0)
                    spriteX += 15;
                break;
            case VK_UP:
                spriteY -= 5;
                if (spriteY < 0)
                    spriteY += 15;
                break;
            case VK_DOWN:
                spriteY += 5;
                if (spriteY + spriteHeight > windowHeight)
                    spriteY -= 15;
                break;
            }
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
        else {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    case WM_MOUSEWHEEL:
    {
        int wheel = GET_WHEEL_DELTA_WPARAM(wParam) / 24;
        int key = LOWORD(wParam);
        if (key == SHIFT_CODE) {
            spriteX += wheel*2;
            if (spriteX + spriteWidth > windowWidth)
                spriteX -= 15;
            if (spriteX < 0)
                spriteX += 15;
        }
        else {
            spriteY -= wheel;
            if (spriteY < 0)
                spriteY += 15;
            if (spriteY + spriteHeight > windowHeight)
                spriteY -= 15;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }
    case WM_SIZE:
        windowWidth = LOWORD(lParam); 
        windowHeight = HIWORD(lParam); 
        if (spriteX < 0)
            spriteX += 5;
        if (spriteY < 0)
            spriteY += 5;
        if (spriteX + spriteWidth > windowWidth)
            spriteX = windowWidth - spriteWidth;
        if (spriteY + spriteHeight > windowHeight)
            spriteY = windowHeight - spriteHeight;
        
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
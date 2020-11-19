#include "MainWindow.h"

#include <Windowsx.h>

wchar_t MainWindow::szClassName[] = L"Minesweeper";

BOOL MainWindow::RegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wc = {0};
    
    wc.cbSize = sizeof (WNDCLASSEX);
    wc.hInstance = hInstance;
    wc.lpszClassName = szClassName;
    wc.lpfnWndProc = WindowProcedure;
    
    wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    //wc.lpszMenuName = MAKEINTRESOURCE(IDM_MENU);
    
    return RegisterClassEx(&wc);
}

HWND MainWindow::Create(HINSTANCE hInstance)
{
    m_hwnd = CreateWindowEx (
                             0,                   /* Extended possibilites for variation */
                             szClassName,         /* Classname */
                             szClassName,         /* Title Text */
                             WS_OVERLAPPEDWINDOW, /* default window */
                             CW_USEDEFAULT,       /* Windows decides the position */
                             CW_USEDEFAULT,       /* where the window ends up on the screen */
                             480,                 /* The programs width */
                             640,                 /* and height in pixels */
                             HWND_DESKTOP,        /* The window is a child-window to desktop */
                             NULL,                /* menu */
                             hInstance,           /* Program Instance handler */
                             this                 /* MainWindow instance */
                             );
    return m_hwnd;
}

LRESULT CALLBACK MainWindow::WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MainWindow * pThis = reinterpret_cast<MainWindow *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    
    if (!pThis && uMsg == WM_CREATE)
    {
        LPCREATESTRUCT pcreatestruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = reinterpret_cast<MainWindow *>(pcreatestruct->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
    }
    
    if (pThis && uMsg == WM_DESTROY)
    {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
    }
    
    if (pThis)
    {
        return pThis->WndProc(hwnd, uMsg, wParam, lParam);
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_CLOSE, OnClose);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL MainWindow::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    UNREFERENCED_PARAMETER(hwnd);
    UNREFERENCED_PARAMETER(lpCreateStruct);
    
    return TRUE;
}

void MainWindow::OnClose(HWND hwnd)
{
    DestroyWindow(hwnd);
}

void MainWindow::OnDestroy(HWND hwnd)
{
    UNREFERENCED_PARAMETER(hwnd);
    
    PostQuitMessage(0);
}


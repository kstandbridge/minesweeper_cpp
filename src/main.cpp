#include <iostream>

#include <Windows.h>

#include "MainWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);
    
    MainWindow mainWindow;
    
    if(!MainWindow::RegisterClass(hInstance))
    {
        std::cout << "Failed to register window class due to error code: " << GetLastError() << std::endl;
        return EXIT_FAILURE;
    }
    
    HWND hwnd = mainWindow.Create(hInstance);
    
    if(hwnd == NULL)
    {
        std::cout << "Failed to create window due to error code: " << GetLastError() << std::endl;
        return EXIT_FAILURE;
    }
    
    ShowWindow(hwnd, nCmdShow);
    
    MSG uMsg = {0};
    while (GetMessage(&uMsg, NULL, 0, 0))
    {
        TranslateMessage(&uMsg);
        DispatchMessage(&uMsg);
    }
    
    return uMsg.wParam;
}

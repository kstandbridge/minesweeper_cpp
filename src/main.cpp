#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' ""version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <iostream>

#include <Windows.h>

#include "Logger.h"
#include "MainWindow.h"



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);
    
    Logger logger;
    Game game;
    MainWindow mainWindow(logger, game);
    
    if(!MainWindow::RegisterClass(hInstance))
    {
        logger.ErrorHandler(L"RegisterClass");
        
        return EXIT_FAILURE;
    }
    
    HWND hwnd = mainWindow.Create(hInstance);
    
    if(hwnd == NULL)
    {
        logger.ErrorHandler(L"CreateWindowEx");
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
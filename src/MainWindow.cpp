#include "MainWindow.h"

#include <Windowsx.h>
#include <CommCtrl.h>
#include <sstream>

#include "Resource.h"
#include "SettingsDialog.h"

wchar_t MainWindow::szClassName[] = L"Minesweeper";

MainWindow::MainWindow(Logger& logger) 
: m_logger(logger)
{
}

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
    
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
    
    return RegisterClassEx(&wc);
}

HWND MainWindow::Create(HINSTANCE hInstance)
{
    m_hwnd = CreateWindowEx (0,                   /* Extended possibilites for variation */
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
        HANDLE_MSG(hwnd, WM_SIZE, OnSize);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL MainWindow::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    UNREFERENCED_PARAMETER(lpCreateStruct);
    
    HWND hStatus = CreateWindowEx(0, STATUSCLASSNAME, L"",
                                  SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
                                  0, 0, 0, 0,
                                  hwnd, (HMENU)IDC_STATUS, GetModuleHandle(NULL), NULL);
    
    if(hStatus == NULL)
    {
        m_logger.ErrorHandler(L"CreateWindowEx");
        return FALSE;
    }
    
    int status_width[] = { 100, -1 };
    SendMessage(hStatus, SB_SETPARTS, sizeof(status_width)/sizeof(int), (LPARAM)status_width);
    
    InitalizeGrid(hwnd);
    
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

void MainWindow::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    UNREFERENCED_PARAMETER(state);
    UNREFERENCED_PARAMETER(cx);
    UNREFERENCED_PARAMETER(cy);
    
    HWND hStatus = GetDlgItem(hwnd, IDC_STATUS);
    if(hStatus == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
    }
    SendMessage(hStatus, WM_SIZE, 0, 0);
}

void MainWindow::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    UNREFERENCED_PARAMETER(hwndCtl);
    UNREFERENCED_PARAMETER(codeNotify);
    
    switch(id)
    {
        case IDM_GAME_SETTINGS:
        {
            OnCommand_Game_Settings(hwnd);
        } break;
        case IDM_GAME_EXIT:
        {
            OnCommand_Game_Exit();
        } break;
        default:
        {
            OnCommand_Tile(hwnd, id, hwndCtl);
        } break;
    }
}

void MainWindow::OnCommand_Game_Settings(HWND hwnd)
{
    SettingsDialog settingsDlg(m_logger);
    int res = settingsDlg.ShowDialog(GetModuleHandle(NULL), hwnd);
    if(res == IDOK)
    {
        std::wstringstream ss;
        ss 
            << L"Rows: " << settingsDlg.get_rows() << std::endl
            << L"Columns: " << settingsDlg.get_columns() << std::endl
            << L"Mines: " << settingsDlg.get_mines() << L"%";
        MessageBox(hwnd, ss.str().c_str(), L"Settings", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        MessageBox(hwnd, L"Must have clicked cancel", L"Settings", MB_OK | MB_ICONWARNING);
    }
}

void MainWindow::OnCommand_Game_Exit()
{
    PostQuitMessage(0);
}

void MainWindow::UpdateStatusText(HWND hwnd, int index, LPTSTR lpszText)
{
    HWND hStatus = GetDlgItem(hwnd, IDC_STATUS);
    if(hStatus == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
    }
    SendMessage(hStatus, SB_SETTEXT, index, (LPARAM)lpszText);
}

BOOL MainWindow::InitalizeGrid(HWND hwnd)
{
    RECT rect;
    if(!GetClientRect(hwnd, &rect))
    {
        m_logger.ErrorHandler(L"GetClientRect");
        return FALSE;
    }
    
    HFONT hFont = CreateFont(24,0,0,0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
    
    if(hFont == NULL)
    {
        m_logger.ErrorHandler(L"CreateFont");
        return FALSE;
    }
    
    int columns = 10;
    int rows = 15;
    
    for(int x = 0; x < columns; x++)
        for(int y = 0; y < rows; y++)
    {
        int button_id = IDC_BUTTON + (y * columns  + x);
        
        HWND hButton = CreateWindow(L"BUTTON", L"", 
                                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                    (1+x)*32, (1+y)*32, 32, 32,
                                    hwnd, (HMENU)button_id, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        
        if(hButton == NULL)
        {
            m_logger.ErrorHandler(L"CreateWindow");
            return FALSE;
        }
    }
    
    UpdateStatusText(hwnd, 1, L"Good Luck!");
    return TRUE;
}

void MainWindow::OnCommand_Tile(HWND hwnd, int id, HWND hwndCtl)
{
    UNREFERENCED_PARAMETER(hwnd);
    
    Button_Enable(hwndCtl, FALSE);
    
    int columns = 10;
    //int rows = 15;
    
    int x = (id - IDC_BUTTON) % columns;
    int y = (id - IDC_BUTTON) / columns;
    
    std::wstringstream ss;
    ss 
        << x << ", " << y << std::endl
        << "Button: " << id;
    
    MessageBox(hwnd, ss.str().c_str(), L"Tile Clicked", MB_OK | MB_ICONINFORMATION);
}
#include "MainWindow.h"

#include <Windowsx.h>
#include <CommCtrl.h>
#include <sstream>

#include "Resource.h"
#include "SettingsDialog.h"

wchar_t MainWindow::szClassName[] = L"Minesweeper";

MainWindow::MainWindow(Logger& logger, Game& game) 
: m_logger(logger)
, m_game(game)
, m_timerSeconds(0)
{
}

BOOL MainWindow::RegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wc = {0};
    
    wc.cbSize = sizeof (WNDCLASSEX);
    wc.hInstance = hInstance;
    wc.lpszClassName = szClassName;
    wc.lpfnWndProc = WindowProcedure;
    
    wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND + 1;
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
        HANDLE_MSG(hwnd, WM_TIMER, OnTimer);
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL MainWindow::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    UNREFERENCED_PARAMETER(lpCreateStruct);
    
    srand((unsigned int)time(NULL));
    
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
    KillTimer(hwnd, IDT_TIMER);
    
    PostQuitMessage(0);
}

void MainWindow::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    UNREFERENCED_PARAMETER(state);
    
    int window_width = cx;
    int window_height = cy;
    
    HWND hStatus = GetDlgItem(hwnd, IDC_STATUS);
    if(hStatus == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
        return;
    }
    SendMessage(hStatus, WM_SIZE, 0, 0);
    
    RECT rcStatus;
    if(!GetWindowRect(hStatus, &rcStatus))
    {
        m_logger.ErrorHandler(L"GetWindowRect");
        return;
    }
    
    int status_height = rcStatus.bottom - rcStatus.top;
    window_height = window_height - status_height;
    
    int columns = m_game.get_columns();
    int rows = m_game.get_rows();
    
    int offset_left = window_width % columns / 2;
    int offset_top = window_height % rows / 2;
    
    for(int x = 0; x < columns; x++)
        for(int y = 0; y < rows; y++)
    {
        int button_id = IDC_BUTTON + (y * columns + x);
        
        int button_width = window_width / columns;
        int button_height = window_height / rows;
        int button_left = (window_width / columns * x) + offset_left;
        int button_top = (window_height / rows * y) + offset_top;
        
        HWND hButton = GetDlgItem(hwnd, button_id);
        if(hButton == NULL)
        {
            m_logger.ErrorHandler(L"GetDlgItem");
            return;
        }
        
        MoveWindow(hButton, button_left, button_top, button_width, button_height, FALSE);
    }
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);
}

void MainWindow::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    UNREFERENCED_PARAMETER(hwndCtl);
    UNREFERENCED_PARAMETER(codeNotify);
    
    switch(id)
    {
        case IDM_GAME_NEW:
        {
            OnCommand_Game_New(hwnd);
        } break;
        case IDM_GAME_SETTINGS:
        {
            OnCommand_Game_Settings(hwnd);
        } break;
        case IDM_GAME_EXIT:
        {
            OnCommand_Game_Exit();
        } break;
        case IDM_DEBUG_SHOW_MINES:
        {
            OnCommand_Debug_ShowMines(hwnd);
        } break;
        default:
        {
            OnCommand_Tile(hwnd, id, hwndCtl);
        } break;
    }
}

void MainWindow::OnCommand_Game_New(HWND hwnd)
{
    if(!CleanUpGrid(hwnd))
    {
        MessageBox(hwnd, L"Failed to clean up grid", L"Error!", MB_OK | MB_ICONERROR);
        return;
    }
    
    if(!InitalizeGrid(hwnd))
    {
        MessageBox(hwnd, L"Failed to initalize up grid", L"Error!", MB_OK | MB_ICONERROR);
        return;
    }
    
    if(!CallOnSize(hwnd))
    {
        MessageBox(hwnd, L"Failed to resize grid", L"Error", MB_OK | MB_ICONERROR);
        return;
    }
}

void MainWindow::OnCommand_Game_Settings(HWND hwnd)
{
    SettingsDialog settingsDlg(m_logger);
    int res = settingsDlg.ShowDialog(GetModuleHandle(NULL), hwnd);
    if(res == IDOK)
    {
        if(!CleanUpGrid(hwnd))
        {
            MessageBox(hwnd, L"Failed to clean up grid", L"Error!", MB_ICONERROR);
            return;
        }
        
        m_game.set_columns(settingsDlg.get_columns());
        m_game.set_rows(settingsDlg.get_rows());
        m_game.set_mines(settingsDlg.get_mines());
        
        if(!InitalizeGrid(hwnd))
        {
            MessageBox(hwnd, L"Failed to initalize up grid", L"Error!", MB_ICONERROR);
            return;
            
        }
        
        if(!CallOnSize(hwnd))
        {
            MessageBox(hwnd, L"Failed to resize grid", L"Error", MB_OK | MB_ICONERROR);
            return;
        }
        
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

void MainWindow::OnCommand_Debug_ShowMines(HWND hwnd)
{
    HMENU hMenu = GetMenu(hwnd);
    if(hMenu == NULL)
    {
        m_logger.ErrorHandler(L"GetMenu");
        return;
    }
    DWORD res = GetMenuState(hMenu, IDM_DEBUG_SHOW_MINES, MF_BYCOMMAND);
    if(res == MF_CHECKED)
    {
        res = CheckMenuItem(hMenu, IDM_DEBUG_SHOW_MINES, MF_BYCOMMAND | MF_UNCHECKED);
        if(res == -1)
        {
            m_logger.ErrorHandler(L"CheckMenuItem");
            return;
        }
        ToggleShowMines(hwnd, FALSE);
    }
    else
    {
        res = CheckMenuItem(hMenu, IDM_DEBUG_SHOW_MINES, MF_BYCOMMAND | MF_CHECKED);
        if(res == -1)
        {
            m_logger.ErrorHandler(L"CheckMenuItem");
            return;
        }
        ToggleShowMines(hwnd, TRUE);
    }
}

void MainWindow::OnTimer(HWND hwnd, UINT id)
{
    switch(id)
    {
        case IDT_TIMER:
        {
            std::wstringstream ss;
            ss
                << m_timerSeconds++ << L" seconds";
            UpdateStatusText(hwnd, 0, ss.str().c_str());
        } break;
    }
}

void MainWindow::UpdateStatusText(HWND hwnd, int index, LPCWSTR lpszText)
{
    HWND hStatus = GetDlgItem(hwnd, IDC_STATUS);
    if(hStatus == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
    }
    SendMessage(hStatus, SB_SETTEXT, index, (LPARAM)lpszText);
}

BOOL MainWindow::CleanUpGrid(HWND hwnd)
{
    int columns = m_game.get_columns();
    int rows = m_game.get_rows();
    
    for(int x = 0; x < columns; x++)
        for(int y = 0; y < rows; y++)
    {
        int button_id = IDC_BUTTON + (y * columns  + x);
        
        HWND hButton = GetDlgItem(hwnd, button_id);
        if(hButton == NULL)
        {
            m_logger.ErrorHandler(L"GetDlgItem");
            return FALSE;
        }
        
        DestroyWindow(hButton);
    }
    
    return TRUE;
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
    
    int columns = m_game.get_columns();
    int rows = m_game.get_rows();
    
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
    
    m_game.InitGame();
    
    HMENU hMenu = GetMenu(hwnd);
    if(hMenu == NULL)
    {
        m_logger.ErrorHandler(L"GetMenu");
        return FALSE;
    }
    DWORD res = CheckMenuItem(hMenu, IDM_DEBUG_SHOW_MINES, MF_BYCOMMAND | MF_UNCHECKED);
    if(res == -1)
    {
        m_logger.ErrorHandler(L"CheckMenuItem");
        return FALSE;
    }
    
    m_timerSeconds = 0;
    SetTimer(hwnd, IDT_TIMER, 1000, NULL);
    
    UpdateStatusText(hwnd, 1, L"Good Luck!");
    return TRUE;
}

void MainWindow::OnCommand_Tile(HWND hwnd, int id, HWND hwndCtl)
{
    Button_Enable(hwndCtl, FALSE);
    
    int columns = m_game.get_columns();
    int rows = m_game.get_rows();
    
    int x = (id - IDC_BUTTON) % columns;
    int y = (id - IDC_BUTTON) / columns;
    TILE_STATE tileState = m_game.CheckTileState(x, y);
    
    if(tileState == EXPLODE)
    {
        ToggleShowMines(hwnd, TRUE);
        if(!DisableTiles(hwnd))
        {
            MessageBox(hwnd, L"Failed to disable all tiles", L"Error", MB_OK | MB_ICONERROR);
            return;
        }
        KillTimer(hwnd, IDT_TIMER);
        MessageBox(hwnd, L"Game Over!!!", L"BOOM!!!", MB_OK | MB_ICONWARNING);
    }
    else if(tileState == CLEAR)
    {
        Button_Enable(hwndCtl, FALSE);
        for(int i = -1; i < 2; i++)
        {
            for(int j = -1; j < 2; j++)
            {
                if(x + i >= 0 && x + i < columns && y + j >= 0 && y + j < rows)
                {
                    if(m_game.GetTileState(x + i, j + y) == MINE)
                    {
                        continue;
                    }
                    int button_id = IDC_BUTTON + ((y + j) * columns + (x + i));
                    HWND hButton  = GetDlgItem(hwnd, button_id);
                    if(hButton == NULL)
                    {
                        m_logger.ErrorHandler(L"GetDlgItem");
                        return;
                    }
                    if(IsWindowEnabled(hButton))
                    {
                        OnCommand_Tile(hwnd, button_id, hButton);
                    }
                }
            }
        }
    }
    else
    {
        wchar_t buf[255];
        swprintf_s(buf, sizeof(buf), L"%d", tileState);
        
        Button_SetText(hwndCtl, buf);
        Button_Enable(hwndCtl, FALSE);
    }
    
    HWND hStatus = GetDlgItem(hwnd, IDC_STATUS);
    if(hStatus == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
        return;
    }
    int tiles_to_check = m_game.get_tiles_to_check();
    
    if(tiles_to_check == 0)
    {
        ToggleShowMines(hwnd, TRUE);
        if(!DisableTiles(hwnd))
        {
            MessageBox(hwnd, L"Failed to disable all tiles", L"Error", MB_OK | MB_ICONERROR);
            return;
        }
        SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)L"Winner!!!");
        KillTimer(hwnd, IDT_TIMER);
        MessageBox(hwnd, L"Winner!!!", L"Success!!!", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        std::wstringstream ss;
        ss << "Tiles to check: " << tiles_to_check;
        SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)ss.str().c_str());
    }
}

BOOL MainWindow::CallOnSize(HWND hwnd)
{
    RECT rcClient;
    if(!GetClientRect(hwnd, &rcClient))
    {
        m_logger.ErrorHandler(L"GetClientRect");
        return FALSE;
    }
    
    int window_width = rcClient.right - rcClient.left;
    int window_height = rcClient.bottom - rcClient.top;
    OnSize(hwnd, 0, window_width, window_height);
    return TRUE;
}

void MainWindow::ToggleShowMines(HWND hwnd, BOOL show_mines)
{
    int columns = m_game.get_columns();
    int rows = m_game.get_rows();
    
    for(int x = 0; x < columns; x++)
        for(int y = 0; y < rows; y++)
    {
        int button_id = IDC_BUTTON + (y * columns  + x);
        HWND button_hwnd = GetDlgItem(hwnd, button_id);
        if(button_hwnd == NULL)
        {
            MessageBox(hwnd, L"Failed to get button handle!", L"Error", MB_OK | MB_ICONERROR);
            return;
        }
        TILE_STATE state = m_game.GetTileState(x, y);
        switch(state)
        {
            case MINE:
            {
                if(show_mines)
                {
                    Button_SetText(button_hwnd, L"B");
                }
                else
                {
                    Button_SetText(button_hwnd, L"");
                }
            } break;
            case EXPLODE:
            {
                Button_SetText(button_hwnd, L"E");
            } break;
            case CLEAR:
            {
                Button_SetText(button_hwnd, L"");
            } break;
            default:
            {
                wchar_t buf[255];
                swprintf_s(buf, sizeof(buf), L"%d", state);
                
                Button_SetText(button_hwnd, buf);
            } break;
        }
    }
}

BOOL MainWindow::DisableTiles(HWND hwnd)
{
    int columns = m_game.get_columns();
    int rows = m_game.get_rows();
    
    for(int x = 0; x < columns; x++)
        for(int y = 0; y < rows; y++)
    {
        int button_id = IDC_BUTTON + (y * columns + x);
        
        HWND hButton = GetDlgItem(hwnd, button_id);
        if(hButton == NULL)
        {
            m_logger.ErrorHandler(L"GetDlgItem");
            return FALSE;
        }
        Button_Enable(hButton, FALSE);
    }
    return TRUE;
}
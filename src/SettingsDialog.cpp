#include "SettingsDialog.h"

#include <Windowsx.h>
#include <CommCtrl.h>

#include "Resource.h"

SettingsDialog::SettingsDialog(Logger& logger)
: m_logger(logger)
, m_columns(0)
, m_rows(0)
, m_mines(0)
{
}

int SettingsDialog::ShowDialog(HINSTANCE hInstance, HWND hwnd)
{
    int res = DialogBoxParam(hInstance,
                             MAKEINTRESOURCE(IDD_SETTINGS),
                             hwnd,
                             SettingsDialog::DialogProcedure,
                             reinterpret_cast<LPARAM>(this));
    if(res == -1)
    {
        m_logger.ErrorHandler(L"DialogBoxParam");
    }
    return res;
}

BOOL CALLBACK SettingsDialog::DialogProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SettingsDialog* pThis = reinterpret_cast<SettingsDialog*>(GetWindowLongPtr(hwnd, DWLP_USER));
    
    if (!pThis && uMsg == WM_INITDIALOG)
    {
        pThis = reinterpret_cast<SettingsDialog*>(lParam);
        SetWindowLongPtr(hwnd, DWLP_USER, reinterpret_cast<LPARAM>(pThis));
    }
    
    if (pThis && uMsg == WM_DESTROY)
    {
        SetWindowLongPtr(hwnd, DWLP_USER, NULL);
    }
    
    if (pThis)
    {
        return pThis->DlgProc(hwnd, uMsg, wParam, lParam);
    }
    
    /* for messages that we don't deal with */
    return FALSE;
}

BOOL CALLBACK SettingsDialog::DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
    }
    return FALSE;
}

BOOL SettingsDialog::OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(hwndFocus);
    UNREFERENCED_PARAMETER(lParam);
    
    HWND slider = GetDlgItem(hwnd, IDC_SETTINGS_MINES);
    if(slider == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
    }
    
    SendMessage(slider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(20, 80));
    SendMessage(slider, TBM_SETPAGESIZE, 0, (LPARAM)10);
    SendMessage(slider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)50);
    
    return TRUE;
}

void SettingsDialog::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    UNREFERENCED_PARAMETER(hwndCtl);
    UNREFERENCED_PARAMETER(codeNotify);
    
    switch(id)
    {
        case IDC_SETTINGS_EASY:
        case IDC_SETTINGS_NORMAL:
        case IDC_SETTINGS_HARD:
        case IDC_SETTINGS_CUSTOM:
        {
            OnCommand_Settings(hwnd, id);
        } break;
        case IDOK:
        {
            OnCommand_OK(hwnd);
        } break;
        case IDCANCEL:
        {
            EndDialog(hwnd, IDCANCEL);
        }
    }
}

void SettingsDialog::OnCommand_Settings(HWND hwnd, int id)
{
    HWND hSlider = GetDlgItem(hwnd, IDC_SETTINGS_MINES);
    if(hSlider == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
    }
    
    SendDlgItemMessage(hwnd, IDC_SETTINGS_EASY, BM_SETCHECK, 0, 0);
    SendDlgItemMessage(hwnd, IDC_SETTINGS_NORMAL, BM_SETCHECK, 0, 0);
    SendDlgItemMessage(hwnd, IDC_SETTINGS_HARD, BM_SETCHECK, 0, 0);
    SendDlgItemMessage(hwnd, IDC_SETTINGS_CUSTOM, BM_SETCHECK, 0, 0);
    
    SendDlgItemMessage(hwnd, id, BM_SETCHECK, 1, 0);
    
    switch(id)
    {
        case IDC_SETTINGS_EASY:
        {
            SendDlgItemMessage(hwnd, IDC_SETTINGS_COLUMNS, WM_SETTEXT, 0, (LPARAM)L"10");
            SendDlgItemMessage(hwnd, IDC_SETTINGS_ROWS, WM_SETTEXT, 0, (LPARAM)L"15");
            SendMessage(hSlider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)20);
        } break;
        case IDC_SETTINGS_NORMAL:
        {
            SendDlgItemMessage(hwnd, IDC_SETTINGS_COLUMNS, WM_SETTEXT, 0, (LPARAM)L"15");
            SendDlgItemMessage(hwnd, IDC_SETTINGS_ROWS, WM_SETTEXT, 0, (LPARAM)L"20");
            SendMessage(hSlider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)30);
        } break;
        case IDC_SETTINGS_HARD:
        {
            SendDlgItemMessage(hwnd, IDC_SETTINGS_COLUMNS, WM_SETTEXT, 0, (LPARAM)L"20");
            SendDlgItemMessage(hwnd, IDC_SETTINGS_ROWS, WM_SETTEXT, 0, (LPARAM)L"30");
            SendMessage(hSlider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)40);
        } break;
    }
    
    BOOL bEnable = (id == IDC_SETTINGS_CUSTOM);
    
    HWND hRows = GetDlgItem(hwnd, IDC_SETTINGS_COLUMNS);
    if(hRows == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
    }
    EnableWindow(hRows, bEnable);
    
    HWND hCols = GetDlgItem(hwnd, IDC_SETTINGS_ROWS);
    if(hCols == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
    }
    EnableWindow(hCols, bEnable);
    
    HWND hMines = GetDlgItem(hwnd, IDC_SETTINGS_MINES);
    if(hMines == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
    }
    EnableWindow(hMines, bEnable);
    
}

void SettingsDialog::OnCommand_OK(HWND hwnd)
{
    m_columns = GetDlgItemInt(hwnd, IDC_SETTINGS_COLUMNS, NULL, FALSE);
    m_rows = GetDlgItemInt(hwnd, IDC_SETTINGS_ROWS, NULL, FALSE);
    
    HWND hMines = GetDlgItem(hwnd, IDC_SETTINGS_MINES);
    if(hMines == NULL)
    {
        m_logger.ErrorHandler(L"GetDlgItem");
    }
    m_mines = SendMessage(hMines, TBM_GETPOS, 0, 0);
    
    EndDialog(hwnd, IDOK);
}
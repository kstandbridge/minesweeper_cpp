/* date = November 20th 2020 10:46 am */

#ifndef _SETTINGS_DIALOG_H
#define _SETTINGS_DIALOG_H

#include "Logger.h"

class SettingsDialog
{
    private:
    Logger m_logger;
    int m_columns;
    int m_rows;
    int m_mines;
    
    public:
    SettingsDialog(Logger& logger);
    INT_PTR ShowDialog(HINSTANCE hInstance, HWND hwnd);
    
    int get_columns() { return m_columns; }
    int get_rows() { return m_rows; }
    int get_mines() { return m_mines; }
    
    protected:
    
    static INT_PTR CALLBACK DialogProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    virtual BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    private:
    
    BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
    void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
    void OnCommand_OK(HWND hwnd);
    void OnCommand_Settings(HWND hwnd, int id);
    
};

#endif //_SETTINGS_DIALOG_H

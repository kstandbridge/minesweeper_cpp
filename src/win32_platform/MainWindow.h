/* date = November 19th 2020 0:44 pm */

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include <Windows.h>

#include "Logger.h"
#include "Game.h"

class MainWindow
{
    private:
    HWND m_hwnd;
    Logger m_logger;
    Game m_game;
    int m_timerSeconds;
    
    public:
    MainWindow(Logger& logger, Game& game);
    static BOOL RegisterClass(HINSTANCE hInstance);
    
    HWND Create(HINSTANCE hInstance);
    
    protected:
    
    static wchar_t szClassName[];
    
    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK ButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubClass, DWORD_PTR dwRefData);
    
    virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT CALLBACK BtnProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubClass, DWORD_PTR dwRefData);
    
    private:
    BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
    void OnClose(HWND hwnd);
    void OnDestroy(HWND hwnd);
    void OnSize(HWND hwnd, UINT state, int cx, int cy);
    void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
    void OnCommand_Game_New(HWND hwnd);
    void OnCommand_Game_Settings(HWND hwnd);
    void OnCommand_Game_Exit();
    void OnCommand_Debug_ShowMines(HWND hwnd);
    void OnTimer(HWND hwnd, UINT id);
    void Button_OnRButtonUp(HWND hwnd, int x, int y, UINT flags);
    
    
    void UpdateStatusText(HWND hwnd, int index, LPCWSTR lpszText);
    BOOL CleanUpGrid(HWND hwnd);
    BOOL InitalizeGrid(HWND hwnd);
    void OnCommand_Tile(HWND hwnd, int id, HWND hwndCtl);
    BOOL CallOnSize(HWND hwnd);
    void ToggleShowMines(HWND hwnd, BOOL show_mines);
    BOOL DisableTiles(HWND hwnd);
    
};

#endif //_MAIN_WINDOW_H

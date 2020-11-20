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
    
    public:
    MainWindow(Logger& logger, Game& game);
    static BOOL RegisterClass(HINSTANCE hInstance);
    
    HWND Create(HINSTANCE hInstance);
    
    protected:
    
    static wchar_t szClassName[];
    
    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    private:
    BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
    void OnClose(HWND hwnd);
    void OnDestroy(HWND hwnd);
    void OnSize(HWND hwnd, UINT state, int cx, int cy);
    void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
    void OnCommand_Game_Settings(HWND hwnd);
    void OnCommand_Game_Exit();
    
    void UpdateStatusText(HWND hwnd, int index, LPTSTR lpszText);
    BOOL CleanUpGrid(HWND hwnd);
    BOOL InitalizeGrid(HWND hwnd);
    void OnCommand_Tile(HWND hwnd, int id, HWND hwndCtl);
};

#endif //_MAIN_WINDOW_H

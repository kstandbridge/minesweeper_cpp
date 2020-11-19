/* date = November 19th 2020 0:44 pm */

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include <Windows.h>

class MainWindow
{
    private:
    HWND m_hwnd;
    
    public:
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
};

#endif //_MAIN_WINDOW_H
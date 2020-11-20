/* date = November 19th 2020 0:40 pm */

#ifndef RESOURCE_H
#define RESOURCE_H

#ifdef IDC_STATIC
#undef IDC_STATIC
#endif
#define IDC_STATIC (-1) // all static control

#define IDR_MENU             1000
#define IDM_GAME_NEW         1001
#define IDM_GAME_SETTINGS    1002
#define IDM_GAME_EXIT        1003
#define IDM_DEBUG_SHOW_MINES 1101

#define IDC_STATUS 1100

#define IDD_SETTINGS         2000
#define IDC_SETTINGS_EASY    2001
#define IDC_SETTINGS_NORMAL  2002
#define IDC_SETTINGS_HARD    2003
#define IDC_SETTINGS_CUSTOM  2004
#define IDC_SETTINGS_MINES   2005
#define IDC_SETTINGS_COLUMNS 2006
#define IDC_SETTINGS_ROWS    2007

#define IDC_BUTTON 9000

#endif //RESOURCE_H

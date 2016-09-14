#include <windows.h>
#include <winuser.h> //user32.lib
#include <tchar.h>

#include "langdef.h"
#include "resource.h"
#include "scrnfuns.h"

#include "main.h"

const char gwMClassName[] = "MainWindowClass";
const char gwSClassName[] = "ScreenWindowClass";

TCHAR TestString[] = _T("This is a \ttest.");

OSVERSIONINFO osv;

HWND MainWindow = NULL, BigScreen = NULL, SettingsWin = NULL;
int NScr = 0, ScrX = 0, ScrY = 0, ScrW = 0, ScrH = 0, Scr0W = 0, Scr0H = 0;


HWND GetScreenHandle()
{
    //Wrapper function
    return BigScreen;
}

int GetScrDetails(int ScrDetail)
{
    //Wrapper Function
    switch (ScrDetail)
    {
        case SCRD_N: return NScr;
        case SCRD_X: return ScrX;
        case SCRD_Y: return ScrY;
        case SCRD_W: return ScrW;
        case SCRD_H: return ScrH;
        case SCRD_0W: return Scr0W;
        case SCRD_0H: return Scr0H;
    }
    return 0;
}

BOOL AddItemToListBox(int AListBox, LPTSTR ItemText)
{
        TCHAR errtxt[1024];
    HWND hLB = GetDlgItem(MainWindow, AListBox);
    if (hLB==NULL)
    {
        _stprintf(errtxt,_T("Error %lu adding '%s' to listbox!"),GetLastError(),ItemText);
        MessageBox(NULL, errtxt, _T("ListBox Error!"), MB_OK);
        return FALSE;
    }
    LRESULT res = SendMessage(hLB, LB_ADDSTRING, 0, (LPARAM) ItemText);
    if (res == LB_ERR || res == LB_ERRSPACE) return FALSE;
    return TRUE;
}

UINT GetWindowShowState(HWND hwnd)
{
    WINDOWPLACEMENT wp;

    wp.length = sizeof(WINDOWPLACEMENT);
    if (GetWindowPlacement(hwnd, &wp))
    {
        return wp.showCmd;
    }
    else
    {
        return BAD_SHOW_STATE;
    }
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            HFONT hfDefault;
            HWND hSchedLB, hSongLB;

            MainWindow = hwnd;
            
            hSchedLB = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", "", WS_CHILD | WS_VSCROLL | WS_TABSTOP |
                                      WS_VISIBLE | LBS_HASSTRINGS | LBS_USETABSTOPS | LBS_WANTKEYBOARDINPUT,
                                      0, 0, 100, 100, hwnd, (HMENU) IDC_SCHEDULE_LISTBOX,
                                      GetModuleHandle(NULL), NULL);
            if (hSchedLB == NULL)
            {
                MessageBox(hwnd, _T("Could not create Schedule List Box!"), _T("Error Creating Components!"), MB_OK | MB_ICONERROR);
                break;
            }
            hfDefault = GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hSchedLB, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

            hSongLB = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", _T(""), WS_CHILD | WS_VSCROLL | WS_TABSTOP |
                                      WS_VISIBLE | LBS_HASSTRINGS | LBS_USETABSTOPS | LBS_WANTKEYBOARDINPUT,
                                      0, 100, 100, 100, hwnd, (HMENU) IDC_SONGS_LISTBOX,
                                      GetModuleHandle(NULL), NULL);
            if (hSongLB == NULL)
            {
                MessageBox(hwnd, _T("Could not create Songs List Box!"), _T("Error Creating Components!"), MB_OK | MB_ICONERROR);
                break;
            }
            SendMessage(hSongLB, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
            SendMessage(hSongLB, LB_ADDSTRING, 0, (LPARAM) TestString);
            
            SongLBSuck(NULL, FALSE, TRUE, TRUE, TRUE, TRUE);
            AddItemToListBox(IDC_SONGS_LISTBOX, _T("A Second Test"));
        }
        break;
        
        
        case WM_CLOSE:
          DestroyWindow(hwnd);
        break;

        case WM_DESTROY:
          PostQuitMessage(0);
        break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case ID_FILE_EXIT:
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                break;

                case ID_SHOW_SCREEN:
                {
                    MENUITEMINFO mii;
                    ZeroMemory(&mii, sizeof(MENUITEMINFO));
                    mii.cbSize = sizeof(MENUITEMINFO);
                    mii.fMask = MIIM_STATE;
                    if (!(GetMenuItemInfo(GetMenu(hwnd), ID_SHOW_SCREEN, FALSE, &mii)))
                    {
                        MessageBox(hwnd, _T("Failed to get menu item state!"), _T("Error Showing/Hiding Screen!"), MB_ICONERROR | MB_OK);
                        break;
                    }
                    
                    //UINT NewShowState;//, CurrentShowState = GetWindowShowState(BigScreen);
                    BOOL NewWindowVisibility;
                    BOOL WindowVisibility = IsWindowVisible(BigScreen);  //Rewrite all this to use SetWindowPos to
                                                                         //change visibility
                    
                    if (WindowVisibility == FALSE)
                    {
                        mii.fState |= MFS_CHECKED;
                        NewWindowVisibility = TRUE;
                        //NewShowState = SW_SHOWDEFAULT; //SW_SHOW; //SW_SHOWNOACTIVATE;
                    }
                    else
                    {
                        mii.fState &= (~MFS_CHECKED);
                        //NewShowState = SW_HIDE;
                        NewWindowVisibility = FALSE;
                    }

                    //if (!(ShowWindow(BigScreen, NewShowState)))
                    if (!(ShowHideScreen(NewWindowVisibility)))
                    {
                        MessageBox(hwnd, _T("Failed to change screen window show-state!"), _T("Error Showing/Hiding Screen!"), MB_ICONERROR | MB_OK);
                        break;
                    }
                    UpdateWindow(BigScreen);
                    if (!(SetMenuItemInfo(GetMenu(hwnd), ID_SHOW_SCREEN, FALSE, &mii)))
                    {
                        MessageBox(hwnd, _T("Failed to set menu item state!"), _T("Error Showing/Hiding Screen!"), MB_ICONERROR | MB_OK);
                        break;
                    }
                }
                break;
            }
        break;

        default:
          return DefWindowProc(hwnd, msg, wParam, lParam);
        break;
    }
    return 0;
}

LRESULT CALLBACK ScreenWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            //Do Nothing
        break;
        case WM_CREATE:
        {
            NONCLIENTMETRICS hfNCM;
            ZeroMemory(&hfNCM,sizeof(NONCLIENTMETRICS));
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
//This preprocessor structure may not be necessary given that Open Trebuchet is targetted at Windows Vista onwards.
#if (_WIN32_WINNT >= 0x0600)
            if (osv.dwMajorVersion < 6)
            {
                hfNCM.cbSize = sizeof(NONCLIENTMETRICS)-sizeof(hfNCM.iPaddedBorderWidth);
                SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS)-sizeof(hfNCM.iPaddedBorderWidth),&hfNCM,0);
            }
            else
            {
                hfNCM.cbSize = sizeof(NONCLIENTMETRICS);
                SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS),&hfNCM,0);
            }
#else
            hfNCM.cbSize = sizeof(NONCLIENTMETRICS);
            SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS),&hfNCM,0);
#endif
            //g_hfFont = /*GetStockObject(DEFAULT_GUI_FONT);*/ CreateFontIndirect(&(hfNCM.lfMessageFont));
        }
        break;

        case WM_PAINT:
        {
            //PaintBS();
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        break;
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  //Get TCHAR Command Line lptCmdLine
#ifdef _UNICODE
  LPTSTR lptCmdLine = GetCommandLineW();
#else
  LPTSTR lptCmdLine = lpCmdLine;
#endif
  
  WNDCLASSEX mwc, swc;
  HWND hmwnd, hswnd;
  MSG Msg;

  //Get OS Version Info
  ZeroMemory(&osv,sizeof(OSVERSIONINFO));
  osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osv);

  //Make sure there's no Console window
  FreeConsole();

  //Get Screen-size info
  NScr = GetSystemMetrics(SM_CMONITORS);
  ScrX = GetSystemMetrics(SM_XVIRTUALSCREEN);
  ScrY = GetSystemMetrics(SM_YVIRTUALSCREEN);
  ScrW = GetSystemMetrics(SM_CXVIRTUALSCREEN);
  ScrH = GetSystemMetrics(SM_CYVIRTUALSCREEN);
  Scr0W = GetSystemMetrics(SM_CXSCREEN);
  Scr0H = GetSystemMetrics(SM_CYSCREEN);

  //Register Window Classes
  mwc.cbSize = sizeof(WNDCLASSEX);
  mwc.style = 0;
  mwc.lpfnWndProc = MainWndProc; //Create this procedure
  mwc.cbClsExtra = 0;
  mwc.cbWndExtra = 0;
  mwc.hInstance = hInstance;
  mwc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  mwc.hCursor = LoadCursor(NULL, IDC_ARROW);
  mwc.hbrBackground = (HBRUSH) (COLOR_BTNFACE+1); //(COLOR_WINDOW+1);
  mwc.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);
  mwc.lpszClassName = gwMClassName;
  mwc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  swc.cbSize = sizeof(WNDCLASSEX);
  swc.style = CS_VREDRAW | CS_HREDRAW;
  swc.lpfnWndProc = ScreenWndProc;
  swc.cbClsExtra = 0;
  swc.cbWndExtra = 0;
  swc.hInstance = hInstance;
  swc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  swc.hCursor = LoadCursor(NULL, IDC_ARROW);
  swc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
  swc.lpszMenuName = NULL;
  swc.lpszClassName = gwSClassName;
  swc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
  
  if (!RegisterClassEx(&mwc))
  {
      MessageBox(NULL, _T("Failed to register main window!"), _T("Error Starting Open Trebuchet!"), MB_ICONERROR | MB_OK);
      return 1;
  }

  if (!RegisterClassEx(&swc))
  {
      MessageBox(NULL, _T("Failed to register screen!"), _T("Error Starting Open Trebuchet!"), MB_ICONERROR | MB_OK);
      return 1;
  }

  //Create Windows
  hmwnd = CreateWindowEx(WS_EX_CLIENTEDGE, gwMClassName, _T("Open Trebuchet"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 480, 240, NULL, NULL, hInstance, NULL);
  hswnd = CreateWindowEx(WS_EX_WINDOWEDGE, gwSClassName, _T("Open Trebuchet Screen"),WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, NULL, NULL, hInstance, NULL);

  if (hmwnd == NULL || hswnd == NULL)
  {
      if (hmwnd != NULL) DestroyWindow(hmwnd);
      if (hswnd != NULL) DestroyWindow(hswnd);
      MessageBox(NULL, _T("Failed to create main window!"), _T("Error Starting Open Trebuchet!"), MB_ICONERROR | MB_OK);
      return 1;
  }

  //Assign Windows to Generic Handles
  MainWindow = hmwnd;
  BigScreen = hswnd;

  //Show the Windows
  ShowWindow(hmwnd, nCmdShow);
  UpdateWindow(hmwnd);

  SetWindowLong(hswnd, GWL_STYLE, 0);
  ShowWindow(hswnd, nCmdShow);
  UpdateWindow(hswnd);
  AutoPositionScreen();

  SetForegroundWindow(hmwnd);

  //Message Loop
  BOOL GMsg;
  while ((GMsg = GetMessage(&Msg, NULL, 0, 0)) > 0)
  {
    if(!IsDialogMessage(SettingsWin, &Msg))
    {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
    }
  }
  if (GMsg < 0) return 1;
  
  return Msg.wParam;
}

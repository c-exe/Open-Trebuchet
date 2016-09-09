#include <windows.h>
#include <tchar.h>

#include "langdef.h"
#include "resource.h"

#include "main.h"

const char gwMClassName[] = "MainWindowClass";
const char gwSClassName[] = "ScreenWindowClass";

OSVERSIONINFO osv;

HWND MainWindow = NULL, BigScreen = NULL, SettingsWin = NULL;
int NScr = 0, ScrX = 0, ScrY = 0, ScrW = 0, ScrH = 0, Scr0W = 0, Scr0H = 0;


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
          DestroyWindow(hwnd);
        break;

        case WM_DESTROY:
          PostQuitMessage(0);
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
  mwc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
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

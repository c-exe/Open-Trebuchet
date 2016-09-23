#ifndef __MAIN_H__

#define __MAIN_H__ 1


#define SCRD_N 1
#define SCRD_X 2
#define SCRD_Y 3
#define SCRD_W 4
#define SCRD_H 5
#define SCRD_0W 6
#define SCRD_0H 7

#define BAD_SHOW_STATE 1000

#define MIN_LB_HEIGHT 15
#define SOFGRP_HEIGHT 110

#include <windows.h>

HWND GetScreenHandle();
int GetScrDetails(int ScrDetail);
BOOL AddItemToListBox(int AListBox, LPTSTR ItemText);
BOOL ClearListBox(int AListBox);
BOOL SongsByTitleOrByFLine();
BOOL ApplySongsFilter();
UINT GetWindowShowState(HWND hwnd);
BOOL TickMenuItem(HWND hWindow, UINT menuitem);
BOOL UntickMenuItem(HWND hWindow, UINT menuitem);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ScreenWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL ShowHideWindow(HWND AWindow, BOOL ShowWindow);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);


#endif

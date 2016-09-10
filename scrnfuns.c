//Screen functions (scrnfuns.c)

#include <windows.h>
#include <tchar.h>
#include <windef.h>

#include "langdef.h"
#include "resource.h"
#include "main.h"

#include "scrnfuns.h"

BOOL MoveScreen(int X, int Y)
{
  //Set the position of the Screen Window
  int Width, Height;
  RECT CurrentPos;
  BOOL Retb;
  if (!(GetWindowRect(GetScreenHandle(), &CurrentPos))) return FALSE;
  Width = (int) (CurrentPos.right - CurrentPos.left);
  Height = (int) (CurrentPos.bottom - CurrentPos.top);
  Retb = MoveWindow(GetScreenHandle(), X, Y, Width, Height, TRUE);
  return Retb;
}

BOOL AutoPositionScreen()
{
  //Automatically calculate and set the position of the Screen Window
  int NScr = GetScrDetails(SCRD_N), ScrX = GetScrDetails(SCRD_X), ScrY = GetScrDetails(SCRD_Y),
      Scr0W = GetScrDetails(SCRD_0W), Scr0H = GetScrDetails(SCRD_0H);
  int XOff, YOff, Width, Height;
  RECT CurrentPos;
  BOOL Retb;
  
  if (!(GetWindowRect(GetScreenHandle(), &CurrentPos))) return FALSE;
  Width = (int) (CurrentPos.right - CurrentPos.left);
  Height = (int) (CurrentPos.bottom - CurrentPos.top);
  
  if (NScr>1)
  {
    XOff = ScrX + Scr0W;
    YOff = ScrY;
  }
  else
  {
    XOff = ScrX + (Scr0W/2);
    YOff = ScrY;
  }
  Retb = MoveWindow(GetScreenHandle(), XOff, YOff, Width, Height, TRUE);
  return Retb;
}

BOOL MoveScreenXPosByPercent(int Percent)
{
  //Set the horizontal position of the Screen Window to a % across the screen
  int ScrX = GetScrDetails(SCRD_X), ScrY = GetScrDetails(SCRD_Y), ScrW = GetScrDetails(SCRD_W);
  int XOff, YOff, Width, Height;
  RECT CurrentPos;
  BOOL Retb;
  
  if (!(GetWindowRect(GetScreenHandle(), &CurrentPos))) return FALSE;
  Width = (int) (CurrentPos.right - CurrentPos.left);
  Height = (int) (CurrentPos.bottom - CurrentPos.top);
  
  XOff = ScrX + ((ScrW * Percent) / 100);
  YOff = ScrY;
  Retb = MoveWindow(GetScreenHandle(), XOff, YOff, Width, Height, TRUE);
  return Retb;
}

BOOL SizeScreen(int Width, int Height)
{
  //Set the size of the Screen Window
  int X, Y;
  RECT CurrentPos;
  BOOL Retb;
  if (!(GetWindowRect(GetScreenHandle(), &CurrentPos))) return FALSE;
  X = (int) CurrentPos.left;
  Y = (int) CurrentPos.top;
  Retb = MoveWindow(GetScreenHandle(), X, Y, Width, Height, TRUE);
  return Retb;
}

BOOL AutoSizeScreen()
{
  //Automatically calculate and set the size of the Screen Window
  int NScr = GetScrDetails(SCRD_N), ScrW = GetScrDetails(SCRD_W), Scr0W = GetScrDetails(SCRD_0W),
      ScrH = GetScrDetails(SCRD_H), Scr0H = GetScrDetails(SCRD_0H);
  int Width, Height;
  
  if (NScr>1)
  {
    Width = ScrW - Scr0W;
    Height = ScrH;
  }
  else
  {
    Width = Scr0W/2;
    Height = Scr0H;
  }
  return SizeScreen(Width, Height);
}

BOOL ShowHideScreen(BOOL ShowScreen)
{
    WINDOWPLACEMENT wp;
    
    ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
    wp.length = sizeof(WINDOWPLACEMENT);

    if (!(GetWindowPlacement(GetScreenHandle(), &wp)))
    {
        return FALSE;
    }

    if (ShowScreen) wp.showCmd = SW_SHOWNOACTIVATE;
    else wp.showCmd = SW_HIDE;

    if (!(SetWindowPlacement(GetScreenHandle(), &wp))) return FALSE;

    return TRUE;
}

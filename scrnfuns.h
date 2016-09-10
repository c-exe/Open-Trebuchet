#ifndef __SCRNFUNS_H__

#define __SCRNFUNS_H__ 1


#include <windows.h>

BOOL MoveScreen(int X, int Y);
BOOL AutoPositionScreen();
BOOL MoveScreenXPosByPercent(int Percent);
BOOL SizeScreen(int Width, int Height);
BOOL AutoSizeScreen();
BOOL ShowHideScreen(BOOL ShowScreen);


#endif

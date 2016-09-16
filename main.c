#include <windows.h>
#include <winuser.h> //user32.lib
#include <tchar.h>

#include "langdef.h"
#include "resource.h"
#include "scrnfuns.h"
#include "errmsgs.h"

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
        exterrmsg(NULL, ERR_CAT_LISTBOX, errtxt);
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
            HWND hSchedLB, hSongLB, hSongFLLB, hSongsBtn, hBiblesBtn, hImagesBtn, hByTitleBtn, hByFLineBtn;

            MainWindow = hwnd;

            //Create IDC_SCHEDULE_LISTBOX
            
            hSchedLB = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", "", WS_CHILD | WS_VSCROLL | WS_TABSTOP |
                                      WS_VISIBLE | LBS_HASSTRINGS | LBS_USETABSTOPS | LBS_WANTKEYBOARDINPUT,
                                      0, 0, 150, 100, hwnd, (HMENU) IDC_SCHEDULE_LISTBOX,
                                      GetModuleHandle(NULL), NULL);
            if (hSchedLB == NULL)
            {
                doerrmsg(hwnd, ERR_CAT_CREATECONTROL, ERR_SUB_CREATESCHEDLB);
                break;
            }
            hfDefault = GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hSchedLB, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

            //Create IDC_SONGS_LISTBOX

            hSongLB = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", _T(""), WS_CHILD | WS_VSCROLL | WS_TABSTOP |
                                     WS_VISIBLE | LBS_HASSTRINGS | LBS_USETABSTOPS | LBS_WANTKEYBOARDINPUT,
                                     0, 155, 150, 100, hwnd, (HMENU) IDC_SONGS_LISTBOX,
                                     GetModuleHandle(NULL), NULL);
            if (hSongLB == NULL)
            {
                doerrmsg(hwnd, ERR_CAT_CREATECONTROL, ERR_SUB_CREATESONGSLB);
                break;
            }
            SendMessage(hSongLB, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
            SendMessage(hSongLB, LB_ADDSTRING, 0, (LPARAM) TestString);
            
            SongLBSuck(NULL, FALSE, TRUE, TRUE, TRUE, TRUE);
            AddItemToListBox(IDC_SONGS_LISTBOX, _T("A Second Test"));

            //Create IDC_SONGFIRSTLINES_LISTBOX

            hSongFLLB = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", _T(""), WS_CHILD | WS_VSCROLL | WS_TABSTOP |
                                       LBS_HASSTRINGS | LBS_USETABSTOPS | LBS_WANTKEYBOARDINPUT,
                                       0, 155, 150, 100, hwnd, (HMENU) IDC_SONGFIRSTLINES_LISTBOX,
                                       GetModuleHandle(NULL), NULL);
            if (hSongFLLB == NULL)
            {
                doerrmsg(hwnd, ERR_CAT_CREATECONTROL, ERR_SUB_CREATESONGFLLB);
                break;
            }
            SendMessage(hSongFLLB, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
            SendMessage(hSongFLLB, LB_ADDSTRING, 0, (LPARAM) TestString);

            SongFLLBSuck(NULL, FALSE, TRUE, TRUE, TRUE, TRUE);

            //Buttons for choice of Songs/Bible/Images

            hSongsBtn = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", _T("So&ngs"), WS_CHILD | WS_TABSTOP | WS_VISIBLE |
                                       WS_GROUP | BS_AUTORADIOBUTTON | BS_CENTER | BS_PUSHLIKE | BS_TEXT | BS_VCENTER,
                                       0, 100, 50, 20, hwnd, (HMENU) IDC_SONGS_BTN,
                                       GetModuleHandle(NULL), NULL);
            if (hSongsBtn == NULL)
            {
                doerrmsg(hwnd, ERR_CAT_CREATECONTROL, ERR_SUB_CREATESONGSBTN);
                break;
            }
            SendMessage(hSongsBtn, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
            SendMessage(hSongsBtn, BM_SETCHECK, (WPARAM)BST_CHECKED, MAKELPARAM(0, 0));
            
            hBiblesBtn = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", _T("&Bibles"), WS_CHILD | WS_TABSTOP | WS_VISIBLE |
                                        BS_AUTORADIOBUTTON | BS_CENTER | BS_PUSHLIKE | BS_TEXT | BS_VCENTER,
                                        50, 100, 50, 20, hwnd, (HMENU) IDC_BIBLES_BTN,
                                        GetModuleHandle(NULL), NULL);
            if (hBiblesBtn == NULL)
            {
                doerrmsg(hwnd, ERR_CAT_CREATECONTROL, ERR_SUB_CREATEBIBLESBTN);
                break;
            }
            SendMessage(hBiblesBtn, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
            SendMessage(hBiblesBtn, BM_SETCHECK, (WPARAM)BST_UNCHECKED, MAKELPARAM(0, 0));
            
            hImagesBtn = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", _T("&Images"), WS_CHILD | WS_TABSTOP | WS_VISIBLE |
                                        BS_AUTORADIOBUTTON | BS_CENTER | BS_PUSHLIKE | BS_TEXT | BS_VCENTER,
                                        100, 100, 50, 20, hwnd, (HMENU) IDC_IMAGES_BTN,
                                        GetModuleHandle(NULL), NULL);
            if (hImagesBtn == NULL)
            {
                doerrmsg(hwnd, ERR_CAT_CREATECONTROL, ERR_SUB_CREATEIMAGESBTN);
                break;
            }
            SendMessage(hImagesBtn, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
            SendMessage(hImagesBtn, BM_SETCHECK, (WPARAM)BST_UNCHECKED, MAKELPARAM(0, 0));

            //Buttons for choice of "Select By..."
            
            hByTitleBtn = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", _T("Songs By &Title"), WS_CHILD | WS_TABSTOP | WS_VISIBLE |
                                         WS_GROUP | BS_AUTORADIOBUTTON | BS_CENTER | BS_PUSHLIKE | BS_TEXT | BS_VCENTER | BS_MULTILINE,
                                         0, 125, 75, 30, hwnd, (HMENU) IDC_BYTITLE_BTN,
                                         GetModuleHandle(NULL), NULL);
            if (hByTitleBtn == NULL)
            {
                doerrmsg(hwnd, ERR_CAT_CREATECONTROL, ERR_SUB_CREATESONGSBYTITLEBTN);
                break;
            }
            SendMessage(hByTitleBtn, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
            SendMessage(hByTitleBtn, BM_SETCHECK, (WPARAM)BST_CHECKED, MAKELPARAM(0, 0));
            
            hByFLineBtn = CreateWindowEx(WS_EX_STATICEDGE, "BUTTON", _T("Songs By First &Line"), WS_CHILD | WS_TABSTOP | WS_VISIBLE |
                                         BS_AUTORADIOBUTTON | BS_CENTER | BS_PUSHLIKE | BS_TEXT | BS_VCENTER | BS_MULTILINE,
                                         75, 125, 75, 30, hwnd, (HMENU) IDC_BYFLINE_BTN,
                                         GetModuleHandle(NULL), NULL);
            if (hByFLineBtn == NULL)
            {
                doerrmsg(hwnd, ERR_CAT_CREATECONTROL, ERR_SUB_CREATESONGSBYFLBTN);
                break;
            }
            SendMessage(hByFLineBtn, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
            SendMessage(hByFLineBtn, BM_SETCHECK, (WPARAM)BST_UNCHECKED, MAKELPARAM(0, 0));

            
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
                        doerrmsg(hwnd, ERR_CAT_SHOWHIDESCREEN, ERR_SUB_SHOWHIDENOMENUITEMSTATE);
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
                        doerrmsg(hwnd, ERR_CAT_SHOWHIDESCREEN, ERR_SUB_SHOWHIDECHANGESCRNSHOWSTATE);
                        break;
                    }
                    UpdateWindow(BigScreen);
                    if (!(SetMenuItemInfo(GetMenu(hwnd), ID_SHOW_SCREEN, FALSE, &mii)))
                    {
                        doerrmsg(hwnd, ERR_CAT_SHOWHIDESCREEN, ERR_SUB_SHOWHIDESETMENUITEMSTATE);
                        break;
                    }
                }
                break;

                case ID_DATABASE_CLEAN:
                  CleanUpDB();
                break;

                case ID_DATABASE_SHOW_SONGSBYTITLE:
                {
                    HWND hSongsBtn = GetDlgItem(hwnd, IDC_SONGS_BTN);
                    if (hSongsBtn == NULL) break;
                    HWND hSongsLB = GetDlgItem(hwnd, IDC_SONGS_LISTBOX);
                    if (hSongsLB == NULL) break;
                    HWND hSongsFLLB = GetDlgItem(hwnd, IDC_SONGFIRSTLINES_LISTBOX);
                    if (hSongsFLLB == NULL) break;
                    LRESULT SongsVisible = SendMessage(hSongsBtn, BM_GETCHECK, (WPARAM)0, MAKELPARAM(0, 0));
                    if (SongsVisible == BST_CHECKED)
                    {
                        if (ShowHideWindow(hSongsLB, TRUE) == FALSE)
                        {
                            doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVCONTROLVISIBILITY);
                            break;
                        }
                        if (ShowHideWindow(hSongsFLLB, FALSE) == FALSE)
                        {
                            doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVCONTROLVISIBILITY);
                            break;
                        }
                        
                    }
                    else if (SongsVisible != BST_UNCHECKED)
                    {
                        /* Error - should never happen! */
                        break;
                    }
                    /* Update the buttons! */
                    HWND hByTitleBtn = GetDlgItem(hwnd, IDC_BYTITLE_BTN);
                    HWND hByFLineBtn = GetDlgItem(hwnd, IDC_BYFLINE_BTN);
                    if (hByTitleBtn == NULL || hByFLineBtn == NULL)
                    {
                        doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVACCESSBTNS);
                        break;
                    }
                    SendMessage(hByTitleBtn, BM_SETCHECK, (WPARAM)BST_CHECKED, MAKELPARAM(0, 0));
                    SendMessage(hByFLineBtn, BM_SETCHECK, (WPARAM)BST_UNCHECKED, MAKELPARAM(0, 0));
                    /* Update the Menu */
                    MENUITEMINFO mii, mii2;
                    ZeroMemory(&mii, sizeof(MENUITEMINFO));
                    mii.cbSize = sizeof(MENUITEMINFO);
                    mii.fMask = MIIM_STATE;
                    if (!(GetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYTITLE, FALSE, &mii)))
                    {
                        doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVNOMENUITEMSTATE);
                        break;
                    }
                    ZeroMemory(&mii2, sizeof(MENUITEMINFO));
                    mii2.cbSize = sizeof(MENUITEMINFO);
                    mii2.fMask = MIIM_STATE;
                    if (!(GetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYFL, FALSE, &mii2)))
                    {
                        doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVNOMENUITEMSTATE);
                        break;
                    }
                    mii.fState |= MFS_CHECKED;
                    mii2.fState &= (~MFS_CHECKED);
                    if (!(SetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYTITLE, FALSE, &mii)))
                    {
                        doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVSETMENUITEMSTATE);
                        break;
                    }
                    if (!(SetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYFL, FALSE, &mii2)))
                    {
                        doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVSETMENUITEMSTATE);
                        break;
                    }
                    
                }
                break;

                case ID_DATABASE_SHOW_SONGSBYFL:
                {
                    HWND hSongsBtn = GetDlgItem(hwnd, IDC_SONGS_BTN);
                    if (hSongsBtn == NULL) break;
                    HWND hSongsLB = GetDlgItem(hwnd, IDC_SONGS_LISTBOX);
                    if (hSongsLB == NULL) break;
                    HWND hSongsFLLB = GetDlgItem(hwnd, IDC_SONGFIRSTLINES_LISTBOX);
                    if (hSongsFLLB == NULL) break;
                    LRESULT SongsVisible = SendMessage(hSongsBtn, BM_GETCHECK, (WPARAM)0, MAKELPARAM(0, 0));
                    if (SongsVisible == BST_CHECKED)
                    {
                        if (ShowHideWindow(hSongsLB, FALSE) == FALSE)
                        {
                            doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVCONTROLVISIBILITY);
                            break;
                        }
                        if (ShowHideWindow(hSongsFLLB, TRUE) == FALSE)
                        {
                            doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVCONTROLVISIBILITY);
                            break;
                        }
                        
                    }
                    else if (SongsVisible != BST_UNCHECKED)
                    {
                        /* Error - should never happen! */
                        break;
                    }
                    /* Update the buttons! */
                    HWND hByTitleBtn = GetDlgItem(hwnd, IDC_BYTITLE_BTN);
                    HWND hByFLineBtn = GetDlgItem(hwnd, IDC_BYFLINE_BTN);
                    if (hByTitleBtn == NULL || hByFLineBtn == NULL)
                    {
                        doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVACCESSBTNS);
                        break;
                    }
                    SendMessage(hByFLineBtn, BM_SETCHECK, (WPARAM)BST_CHECKED, MAKELPARAM(0, 0));
                    SendMessage(hByTitleBtn, BM_SETCHECK, (WPARAM)BST_UNCHECKED, MAKELPARAM(0, 0));
                    /* Update the Menu */
                    MENUITEMINFO mii, mii2;
                    ZeroMemory(&mii, sizeof(MENUITEMINFO));
                    mii.cbSize = sizeof(MENUITEMINFO);
                    mii.fMask = MIIM_STATE;
                    if (!(GetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYTITLE, FALSE, &mii)))
                    {
                        doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVNOMENUITEMSTATE);
                        break;
                    }
                    ZeroMemory(&mii2, sizeof(MENUITEMINFO));
                    mii2.cbSize = sizeof(MENUITEMINFO);
                    mii2.fMask = MIIM_STATE;
                    if (!(GetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYFL, FALSE, &mii2)))
                    {
                        doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVNOMENUITEMSTATE);
                        break;
                    }
                    mii2.fState |= MFS_CHECKED;
                    mii.fState &= (~MFS_CHECKED);
                    if (!(SetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYTITLE, FALSE, &mii)))
                    {
                        doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVSETMENUITEMSTATE);
                        break;
                    }
                    if (!(SetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYFL, FALSE, &mii2)))
                    {
                        doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVSETMENUITEMSTATE);
                        break;
                    }
                    
                }
                break;

                case IDC_SONGS_BTN:
                  switch (HIWORD(wParam))
                  {
                      case BN_CLICKED:
                      {
                          /* Make:
                             IDC_SONGS_LISTBOX Visible (if selected)
                             (else) IDC_SONGFIRSTLINES_LISTBOX Visible
                             IDC_BYTITLE_BTN Visible
                             IDC_BYFLINE_BTN Visible
                             IDC_BIBLETEXT_LISTBOX Invisible
                             IDC_BIBLEBOOKS_COMBO Invisible
                             IDC_BIBLES_COMBO Invisible
                             IDC_IMAGES_LISTBOX Invisible */
                          
                      }
                      break;

                      default:
                        return DefWindowProc(hwnd, msg, wParam, lParam);
                      break;
                  }
                break;

                case IDC_BIBLES_BTN:
                  switch (HIWORD(wParam))
                  {
                      case BN_CLICKED:
                        /* Make:
                           IDC_BIBLETEXT_LISTBOX Visible
                           IDC_BIBLEBOOKS_COMBO Visible
                           IDC_BIBLES_COMBO Visible
                           IDC_SONGS_LISTBOX Invisible
                           IDC_SONGFIRSTLINES_LISTBOX Invisible
                           IDC_BYTITLE_BTN Invisible
                           IDC_BYFLINE_BTN Invisible
                           IDC_IMAGES_LISTBOX Invisible */
                      break;

                      default:
                        return DefWindowProc(hwnd, msg, wParam, lParam);
                      break;
                  }
                break;

                case IDC_IMAGES_BTN:
                  switch (HIWORD(wParam))
                  {
                      case BN_CLICKED:
                        /* Make:
                           IDC_IMAGES_LISTBOX Visible
                           IDC_SONGS_LISTBOX Invisible
                           IDC_SONGFIRSTLINES_LISTBOX Invisible
                           IDC_BYTITLE_BTN Invisible
                           IDC_BYFLINE_BTN Invisible
                           IDC_BIBLETEXT_LISTBOX Invisible
                           IDC_BIBLEBOOKS_COMBO Invisible
                           IDC_BIBLES_COMBO Invisible */
                      break;

                      default:
                        return DefWindowProc(hwnd, msg, wParam, lParam);
                      break;
                  }
                break;

                case IDC_BYTITLE_BTN:
                  switch (HIWORD(wParam))
                  {
                      case BN_CLICKED:
                      {
                          /* (If visible) Make:
                             IDC_SONGS_LISTBOX Visible
                             IDC_SONGFIRSTLINES_LISTBOX Invisible */
                          HWND hSongsBtn = GetDlgItem(hwnd, IDC_SONGS_BTN);
                          if (hSongsBtn == NULL) break;
                          HWND hSongsLB = GetDlgItem(hwnd, IDC_SONGS_LISTBOX);
                          if (hSongsLB == NULL) break;
                          HWND hSongsFLLB = GetDlgItem(hwnd, IDC_SONGFIRSTLINES_LISTBOX);
                          if (hSongsFLLB == NULL) break;
                          LRESULT SongsVisible = SendMessage(hSongsBtn, BM_GETCHECK, (WPARAM)0, MAKELPARAM(0, 0));
                          if (SongsVisible == BST_CHECKED)
                          {
                              if (ShowHideWindow(hSongsLB, TRUE) == FALSE)
                              {
                                  doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVCONTROLVISIBILITY);
                                  break;
                              }
                              if (ShowHideWindow(hSongsFLLB, FALSE) == FALSE)
                              {
                                  doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVCONTROLVISIBILITY);
                                  break;
                              }
                              
                          }
                          else if (SongsVisible != BST_UNCHECKED)
                          {
                              /* Error - should never happen! */
                              break;
                          }
                          /* Update the menu */
                          MENUITEMINFO mii, mii2;
                          ZeroMemory(&mii, sizeof(MENUITEMINFO));
                          mii.cbSize = sizeof(MENUITEMINFO);
                          mii.fMask = MIIM_STATE;
                          if (!(GetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYTITLE, FALSE, &mii)))
                          {
                              doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVNOMENUITEMSTATE);
                              break;
                          }
                          ZeroMemory(&mii2, sizeof(MENUITEMINFO));
                          mii2.cbSize = sizeof(MENUITEMINFO);
                          mii2.fMask = MIIM_STATE;
                          if (!(GetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYFL, FALSE, &mii2)))
                          {
                              doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVNOMENUITEMSTATE);
                              break;
                          }
                          mii.fState |= MFS_CHECKED;
                          mii2.fState &= (~MFS_CHECKED);
                          if (!(SetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYTITLE, FALSE, &mii)))
                          {
                              doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVSETMENUITEMSTATE);
                              break;
                          }
                          if (!(SetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYFL, FALSE, &mii2)))
                          {
                              doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVSETMENUITEMSTATE);
                              break;
                          }
                          UpdateWindow(hwnd);
                      }
                      break;

                      default:
                        return DefWindowProc(hwnd, msg, wParam, lParam);
                      break;
                  }
                break;
                
                case IDC_BYFLINE_BTN:
                  switch (HIWORD(wParam))
                  {
                      case BN_CLICKED:
                      {
                          /* (If visible) Make:
                             IDC_SONGFIRSTLINES_LISTBOX Visible
                             IDC_SONGS_LISTBOX Invisible */
                          HWND hSongsBtn = GetDlgItem(hwnd, IDC_SONGS_BTN);
                          if (hSongsBtn == NULL) break;
                          HWND hSongsLB = GetDlgItem(hwnd, IDC_SONGS_LISTBOX);
                          if (hSongsLB == NULL) break;
                          HWND hSongsFLLB = GetDlgItem(hwnd, IDC_SONGFIRSTLINES_LISTBOX);
                          if (hSongsFLLB == NULL) break;
                          LRESULT SongsVisible = SendMessage(hSongsBtn, BM_GETCHECK, (WPARAM)0, MAKELPARAM(0, 0));
                          if (SongsVisible == BST_CHECKED)
                          {
                              if (ShowHideWindow(hSongsFLLB, TRUE) == FALSE)
                              {
                                  doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVCONTROLVISIBILITY);
                                  break;
                              }
                              if (ShowHideWindow(hSongsLB, FALSE) == FALSE)
                              {
                                  doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVCONTROLVISIBILITY);
                                  break;
                              }
                              
                          }
                          else if (SongsVisible != BST_UNCHECKED)
                          {
                              /* Error - should never happen! */
                              break;
                          }
                          /* Update the menu */
                          MENUITEMINFO mii, mii2;
                          ZeroMemory(&mii, sizeof(MENUITEMINFO));
                          mii.cbSize = sizeof(MENUITEMINFO);
                          mii.fMask = MIIM_STATE;
                          if (!(GetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYTITLE, FALSE, &mii)))
                          {
                              doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVNOMENUITEMSTATE);
                              break;
                          }
                          ZeroMemory(&mii2, sizeof(MENUITEMINFO));
                          mii2.cbSize = sizeof(MENUITEMINFO);
                          mii2.fMask = MIIM_STATE;
                          if (!(GetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYFL, FALSE, &mii2)))
                          {
                              doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVNOMENUITEMSTATE);
                              break;
                          }
                          mii2.fState |= MFS_CHECKED;
                          mii.fState &= (~MFS_CHECKED);
                          if (!(SetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYTITLE, FALSE, &mii)))
                          {
                              doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVSETMENUITEMSTATE);
                              break;
                          }
                          if (!(SetMenuItemInfo(GetMenu(hwnd), ID_DATABASE_SHOW_SONGSBYFL, FALSE, &mii2)))
                          {
                              doerrmsg(hwnd, ERR_CAT_CHGCONTROLVIEW, ERR_SUB_CHGVSETMENUITEMSTATE);
                              break;
                          }
                          
                      }
                      break;

                      default:
                        return DefWindowProc(hwnd, msg, wParam, lParam);
                      break;
                  }
                break;
                
                default:
                  return DefWindowProc(hwnd, msg, wParam, lParam);
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

BOOL ShowHideWindow(HWND AWindow, BOOL ShowWindow)
{
    WINDOWPLACEMENT wp;
    
    ZeroMemory(&wp, sizeof(WINDOWPLACEMENT));
    wp.length = sizeof(WINDOWPLACEMENT);

    if (!(GetWindowPlacement(AWindow, &wp)))
    {
        return FALSE;
    }

    if (ShowWindow) wp.showCmd = SW_SHOWNOACTIVATE;
    else wp.showCmd = SW_HIDE;

    if (!(SetWindowPlacement(AWindow, &wp))) return FALSE;

    return TRUE;
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
      doerrmsg(NULL, ERR_CAT_STARTPROG, ERR_SUB_STARTPREGMWIN);
      return 1;
  }

  if (!RegisterClassEx(&swc))
  {
      doerrmsg(NULL, ERR_CAT_STARTPROG, ERR_SUB_STARTPREGSWIN);
      return 1;
  }

  //Create Windows
  hmwnd = CreateWindowEx(WS_EX_CLIENTEDGE, gwMClassName, _T("Open Trebuchet"), WS_OVERLAPPEDWINDOW , CW_USEDEFAULT, CW_USEDEFAULT, 480, 240, NULL, NULL, hInstance, NULL);
  hswnd = CreateWindowEx(WS_EX_WINDOWEDGE, gwSClassName, _T("Open Trebuchet Screen"),WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, NULL, NULL, hInstance, NULL);

  if (hmwnd == NULL || hswnd == NULL)
  {
      if (hmwnd == NULL) doerrmsg(NULL, ERR_CAT_STARTPROG, ERR_SUB_STARTPCREATEMWIN);
      else doerrmsg(NULL, ERR_CAT_STARTPROG, ERR_SUB_STARTPCREATESWIN);
      if (hmwnd != NULL) DestroyWindow(hmwnd);
      if (hswnd != NULL) DestroyWindow(hswnd);
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
    /*if(!IsDialogMessage(SettingsWin, &Msg))
    {*/
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
    /*}*/
  }
  if (GMsg < 0) return 1;
  
  return Msg.wParam;
}

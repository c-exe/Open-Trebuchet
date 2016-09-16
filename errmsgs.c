/* Error Messages Functions */

#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "langdef.h"

#include "errmsgs.h"


int doerrmsg(HWND hwnd, unsigned long errorcategory, unsigned long suberror)
{
    switch (errorcategory)
    {
        case ERR_CAT_CREATECONTROL:
          switch (suberror)
          {
              case ERR_SUB_CREATESCHEDLB:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Could not create Schedule List Box!"), ERR_CATTXT_CREATECONTROL, MB_OK | MB_ICONERROR);
#endif
              break;

              case ERR_SUB_CREATESONGSLB:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Could not create Songs List Box!"), ERR_CATTXT_CREATECONTROL, MB_OK | MB_ICONERROR);
#endif
              break;

              case ERR_SUB_CREATESONGFLLB:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Could not create Song First Lines List Box!"), ERR_CATTXT_CREATECONTROL, MB_OK | MB_ICONERROR);
#endif
              break;
              
              case ERR_SUB_CREATESONGSBTN:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Could not create Songs Button!"), ERR_CATTXT_CREATECONTROL, MB_OK | MB_ICONERROR);
#endif
              break;

              case ERR_SUB_CREATEBIBLESBTN:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Could not create Bibles Button!"), ERR_CATTXT_CREATECONTROL, MB_OK | MB_ICONERROR);
#endif
              break;

              case ERR_SUB_CREATEIMAGESBTN:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Could not create Images Button!"), ERR_CATTXT_CREATECONTROL, MB_OK | MB_ICONERROR);
#endif
              break;

              case ERR_SUB_CREATESONGSBYTITLEBTN:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Could not create Songs By Title Button!"), ERR_CATTXT_CREATECONTROL, MB_OK | MB_ICONERROR);
#endif
              break;

              case ERR_SUB_CREATESONGSBYFLBTN:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Could not create Songs By First Line Button!"), ERR_CATTXT_CREATECONTROL, MB_OK | MB_ICONERROR);
#endif
              break;

              default:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Could not create Unknown Control!"), ERR_CATTXT_CREATECONTROL, MB_OK | MB_ICONERROR);
#endif
              break;
          }
        break;

        case ERR_CAT_SHOWHIDESCREEN:
          switch (suberror)
          {
              case ERR_SUB_SHOWHIDENOMENUITEMSTATE:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to get menu item state!"), ERR_CATTXT_SHOWHIDESCREEN, MB_OK | MB_ICONERROR);
#endif
              break;
              
              case ERR_SUB_SHOWHIDECHANGESCRNSHOWSTATE:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to change screen window show-state!"), ERR_CATTXT_SHOWHIDESCREEN, MB_OK | MB_ICONERROR);
#endif
              break;
              
              case ERR_SUB_SHOWHIDESETMENUITEMSTATE:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to set menu item state!"), ERR_CATTXT_SHOWHIDESCREEN, MB_OK | MB_ICONERROR);
#endif
              break;

              default:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to complete unknown task!"), ERR_CATTXT_SHOWHIDESCREEN, MB_OK | MB_ICONERROR);
#endif
              break;
          }
        break;

        case ERR_CAT_CHGCONTROLVIEW:
          switch (suberror)
          {
              case ERR_SUB_CHGVCONTROLVISIBILITY:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to change control visibility!"), ERR_CATTXT_CHGCONTROLVIEW, MB_OK | MB_ICONERROR);
#endif
              break;

              case ERR_SUB_CHGVACCESSBTNS:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to access buttons!"), ERR_CATTXT_CHGCONTROLVIEW, MB_OK | MB_ICONERROR);
#endif
              break;

              case ERR_SUB_CHGVNOMENUITEMSTATE:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to get menu item state!"), ERR_CATTXT_CHGCONTROLVIEW, MB_OK | MB_ICONERROR);
#endif
              break;

              case ERR_SUB_CHGVSETMENUITEMSTATE:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to set menu item state!"), ERR_CATTXT_CHGCONTROLVIEW, MB_OK | MB_ICONERROR);
#endif
              break;

              default:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Unknown error encountered changing view!"), ERR_CATTXT_CHGCONTROLVIEW, MB_OK | MB_ICONERROR);
#endif
              break;
          }
        break;

        case ERR_CAT_STARTPROG:
          switch (suberror)
          {
              case ERR_SUB_STARTPREGMWIN:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to register main window!"), ERR_CATTXT_STARTPROG, MB_OK | MB_ICONERROR);
#endif
              break;
              
              case ERR_SUB_STARTPREGSWIN:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to register screen window!"), ERR_CATTXT_STARTPROG, MB_OK | MB_ICONERROR);
#endif
              break;
              
              case ERR_SUB_STARTPCREATEMWIN:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to create main window!"), ERR_CATTXT_STARTPROG, MB_OK | MB_ICONERROR);
#endif
              break;
              
              case ERR_SUB_STARTPCREATESWIN:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Failed to create screen window!"), ERR_CATTXT_STARTPROG, MB_OK | MB_ICONERROR);
#endif
              break;
              
              default:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Unknown error encountered starting Open Trebuchet!"), ERR_CATTXT_STARTPROG, MB_OK | MB_ICONERROR);
#endif
              break;
          }
        break;

        case ERR_CAT_DB:
          switch (suberror)
          {
              case ERR_SUB_DBOPEN:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Cannot open database."), ERR_CATTXT_DB, MB_OK | MB_ICONERROR);
#endif
              break;
              
              case ERR_SUB_DBOOM:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Out of Memory!"), ERR_CATTXT_DB, MB_OK | MB_ICONERROR);
#endif
              break;
              
              case ERR_SUB_DBCONVFILTER:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Error Converting Filter!"), ERR_CATTXT_DB, MB_OK | MB_ICONERROR);
#endif
              break;
              
              case ERR_SUB_DBSQL:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("SQL Error!"), ERR_CATTXT_DB, MB_OK | MB_ICONERROR);
#endif
              break;
              
              default:
#if ERRORLANG == _LANG_ENGLISH
                MessageBox(hwnd, _T("Unknown database error!"), ERR_CATTXT_DB, MB_OK | MB_ICONERROR);
#endif
              break;
          }
        break;
    }
}

int exterrmsg(HWND hwnd, unsigned long errorcategory, LPTSTR theerrmsg)
{
    switch (errorcategory)
    {
        case ERR_CAT_LISTBOX:
#if ERRORLANG == _LANG_ENGLISH
          MessageBox(hwnd, theerrmsg, _T("ListBox Error!"), MB_OK | MB_ICONERROR);
#endif
        break;
    }
}

int exterrmsgA(HWND hwnd, unsigned long errorcategory, char *theerrmsg)
{
    switch (errorcategory)
    {
        case ERR_CAT_DB:
#if ERRORLANG == _LANG_ENGLISH
          MessageBoxA(hwnd, theerrmsg, "Database Error!", MB_OK | MB_ICONERROR);
#endif
        break;
    }
}

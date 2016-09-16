/* Error Messages Functions Definitions */
#ifndef __ERRMSGS_H__

#define __ERRMSGS_H__ 1


#include <tchar.h>
#include "langdef.h"

#define ERR_CAT_LISTBOX 1
#define ERR_CAT_CREATECONTROL 2
#define   ERR_SUB_CREATESCHEDLB 1
#define   ERR_SUB_CREATESONGSLB 2
#define   ERR_SUB_CREATESONGFLLB 3
#define   ERR_SUB_CREATESONGSBTN 4
#define   ERR_SUB_CREATEBIBLESBTN 5
#define   ERR_SUB_CREATEIMAGESBTN 6
#define   ERR_SUB_CREATESONGSBYTITLEBTN 7
#define   ERR_SUB_CREATESONGSBYFLBTN 8
#define ERR_CAT_SHOWHIDESCREEN 3
#define   ERR_SUB_SHOWHIDENOMENUITEMSTATE 1
#define   ERR_SUB_SHOWHIDECHANGESCRNSHOWSTATE 2
#define   ERR_SUB_SHOWHIDESETMENUITEMSTATE 3
#define ERR_CAT_CHGCONTROLVIEW 4
#define   ERR_SUB_CHGVCONTROLVISIBILITY 1
#define   ERR_SUB_CHGVACCESSBTNS 2
#define   ERR_SUB_CHGVNOMENUITEMSTATE 3
#define   ERR_SUB_CHGVSETMENUITEMSTATE 4
#define ERR_CAT_STARTPROG 5
#define   ERR_SUB_STARTPREGMWIN 1
#define   ERR_SUB_STARTPREGSWIN 2
#define   ERR_SUB_STARTPCREATEMWIN 3
#define   ERR_SUB_STARTPCREATESWIN 4
#define ERR_CAT_DB 6
#define   ERR_SUB_DBOPEN 1
#define   ERR_SUB_DBOOM 2
#define   ERR_SUB_DBCONVFILTER 3
#define   ERR_SUB_DBSQL 4


#ifndef ERRORLANG
#define ERRORLANG _LANG_ENGLISH
#endif

#if ERRORLANG == _LANG_ENGLISH
#define ERR_CATTXT_CREATECONTROL _T("Error Creating Controls!")
#define ERR_CATTXT_SHOWHIDESCREEN _T("Error Showing/Hiding Screen!")
#define ERR_CATTXT_CHGCONTROLVIEW _T("Error Changing View!")
#define ERR_CATTXT_STARTPROG _T("Error Starting Open Trebuchet!")
#define ERR_CATTXT_DB _T("Database Error!")
#elif ERRORLANG == _LANG_FRANCAIS
#define ERR_CATTXT_CREATECONTROL _T("Erreur Cr�ation de Contr�les!")
#define ERR_CATTXT_SHOWHIDESCREEN _T("Erreur Affichage/Masquage de l'�cran!")
#define ERR_CATTXT_CHGCONTROLVIEW _T("Erreur lors du Changement View!")
#define ERR_CATTXT_STARTPROG _T("Erreur lors du D�marrage Open Trebuchet!")
#define ERR_CATTXT_DB _T("Erreur de la Base de Donn�es!")
#elif ERRORLANG == _LANG_CYMRAIG
#define ERR_CATTXT_CREATECONTROL _T("Gwall Creu Rheolaethau!")
#define ERR_CATTXT_SHOWHIDESCREEN _T("Gwall Dangos/Cuddio Sgrin!")
#define ERR_CATTXT_CHGCONTROLVIEW _T("Gwall Newid Ymddangosiad!")
#define ERR_CATTXT_STARTPROG _T("Gwall Dechrau Open Trebuchet!")
#define ERR_CATTXT_DB _T("Gwall Cronfa Ddata!")
#endif


int doerrmsg(HWND hwnd, unsigned long errorcategory, unsigned long suberror);
int exterrmsg(HWND hwnd, unsigned long errorcategory, LPTSTR theerrmsg);
int exterrmsgA(HWND hwnd, unsigned long errorcategory, char *theerrmsg);


#endif

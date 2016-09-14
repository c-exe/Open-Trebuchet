/* Functions for exchanging song data with the database.  Not to be confused with sqlite3.h/sqlite3.a which is
   for actually interacting with the database at all.  Functions beginning with 't' are for TCHARs. */

#ifndef __DBFUNCS_H__

#define __DBFUNCS_H__ 1

#include <windows.h>
#include <tchar.h>

typedef unsigned int UTF16;
typedef unsigned long int UTF32;

char *TCHARToUTF8(TCHAR c);
int TSTRToUTF8Str(char *utf8str, LPTSTR tstr); /* Ensure utf8str >=3*len(tstr)! */
void UTF8StrToTSTR(LPTSTR tstr, char *utf8str, TCHAR unknownchar); /* Ensure tstr >= len(utf8str)! */
void escapeapostrophes(char *outstr, char *instr); /* Ensure outstr >=2*len(instr)! */
void tescapeapostrophes(LPTSTR outstr, LPTSTR instr); /* Ensure outstr >=2*len(instr)! */
int tescapeforlike(LPTSTR outstr, LPTSTR instr); /* Ensure outstr >=4*len(instr)! */
void stripescapedapostrophes(char *outstr, char *instr); /* Ensure outstr >= len(instr)! */
void tstripescapedapostrophes(LPTSTR outstr, LPTSTR instr); /* Ensure outstr >= len(instr)! */
void strtoupper(char *astr);
void tstrtoupper(LPTSTR astr);
int tsafestrtoupper(LPTSTR astr, DWORD astrlen);  /* Safer version */
void stripspaces(char *astr);
void tstripspaces(LPTSTR astr);
void strleft(char *outstr, char *instr, int chars); /* Ensure outstr >= chars! */
int tstrleft(LPTSTR outstr, LPTSTR instr, int chars); /* Ensure outstr >= chars! */
int streq(char *astr, char *bstr);
int tstreq(LPTSTR astr, LPTSTR bstr);
void stripmarkup(char *outstr, char *instr); /* Ensure outstr >= len(instr)! */
void tstripmarkup(LPTSTR outstr, LPTSTR instr); /* Ensure outstr >= len(instr)! */
void firstlineonly(char *astr);
void tfirstlineonly(LPTSTR astr);
long tsafestrlen(LPTSTR astr);
void shift4charsleft(char *chars4, char newchar);
void tshift4charsleft(LPTSTR chars4, TCHAR newchar);
char **slidessplit(char *songtext);
LPTSTR *tslidessplit(LPTSTR songtext);

#endif

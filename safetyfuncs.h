/* Safety Funcs - Basically a replacement for the functions defined in strsafe.h, which is unavailable for MinGW32. */

#ifndef __SAFETYFUNCS_H__

#define __SAFETYFUNCS_H__ 1


#include <windows.h>
#include <tchar.h>
#include <limits.h>

#define SAFETY_OK 1
#define SAFETY_INSUFFICIENT_BUFFER 0
#define SAFETY_INVALID_PARAMETER -1
#define SAFETY_MAX_CH INT_MAX

long stringchlength(LPTSTR astr, long maxlen);
int stringchcopy(LPTSTR outstr, long buffsize, LPCTSTR instr);


#endif

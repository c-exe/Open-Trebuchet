/* Safety Funcs - Basically a replacement for the functions defined in strsafe.h, which is unavailable for MinGW32. */

#include <string.h>
#include <windows.h>
#include <limits.h>
#include <tchar.h>

#include "safetyfuncs.h"

long stringchlength(LPTSTR astr, long maxlen)
{
    long i;
    if (astr == NULL || maxlen < 0) return SAFETY_INVALID_PARAMETER;
    for (i=0;i<maxlen;i++)
    {
        if (astr[i] == 0) break;
    }
    return i;
}

int stringchcopy(LPTSTR outstr, long buffsize, LPCTSTR instr)
{
    long i;
    if (buffsize > INT_MAX || buffsize <= 0 || outstr == NULL || instr == NULL) return SAFETY_INVALID_PARAMETER;
    for (i=0;i<buffsize;i++)
    {
        outstr[i] = instr[i];
        if (instr[i] == 0) break;
    }
    if (i>=buffsize) i = buffsize-1;
    if (instr[i] != 0)
    {
        outstr[i] = 0;
        return SAFETY_INSUFFICIENT_BUFFER;
    }
    return SAFETY_OK;
}

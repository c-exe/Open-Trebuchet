#include <Windows.h>
#include <winuser.h>  //Include User32.lib
#include <winbase.h>  //Include Kernel32.lib
#include <winerror.h>
/*#include <strsafe.h>*/ //Use "safetyfuncs.h" instead
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include "safetyfuncs.h"
#include "dbfuncs.h"

/* Functions for exchanging song data with the database.  Not to be confused with sqlite3.h/sqlite3.a which is for actually interacting with the database at all. */

void escapeapostrophes(char *outstr, char *instr)
{
    /* Escape apostrophes for input into the database.
       Outstr should be double the size of instr for safety! */
    long i, j=0;
    if (instr == NULL || outstr == NULL) return;
    for (i=0;instr[i]!=0;i++)
    {
        if (instr[i] != '\'')
        {
            outstr[j] = instr[i];
            j++;
        }
        else
        {
            outstr[j] = '\'';
            outstr[j+1] = '\'';
            j+=2;
        }
    }
    outstr[j] = 0;
}

void tescapeapostrophes(LPTSTR outstr, LPTSTR instr)
{
    /* TCHAR version of above routine.
       Escape apostrophes for input into the database.
       Outstr should be double the size of instr for safety! */
    long j=0, diff, k;
    LPTSTR sptr, nsptr;
    if (instr == NULL || outstr == NULL) return;
    for (sptr = instr;sptr[0]!=0;sptr = CharNext(sptr))
    {
        if (sptr[0] != _T('\''))
        {
            nsptr = CharNext(sptr);
            diff = (long) (nsptr-sptr);
            for (k=0; k<diff; k++)
            {
                outstr[j] = sptr[k];
                j++;
            }
        }
        else
        {
            outstr[j] = _T('\'');
            outstr[j+1] = _T('\'');
            j+=2;
        }
    }
    outstr[j] = 0;
}

void stripescapedapostrophes(char *outstr, char *instr)
{
    /* Strip escaped apostrophes from database return.
       Outstr should be at least the size of instr for safety! */
    long i, j=0;
    int lastwasa = 0;
    if (instr == NULL || outstr == NULL) return;
    for (i=0;instr[i]!=0;i++)
    {
        if (instr[i] != '\'')
        {
            outstr[j] = instr[i];
            lastwasa = 0;
            j++;
        }
        else
        {
            if (lastwasa != 0)
            {
                outstr[j] = '\'';
                lastwasa = 0;
                j++;
            }
            else lastwasa = 1;
        }
    }
    outstr[j] = 0;
}

void tstripescapedapostrophes(LPTSTR outstr, LPTSTR instr)
{
    /* TCHAR version of above routine.
       Strip escaped apostrophes from database return.
       Outstr should be at least the size of instr for safety! */
    long j=0, diff, k;
    int lastwasa = 0;
    LPTSTR sptr, nsptr;
    if (instr == NULL || outstr == NULL) return;
    for (sptr = instr;sptr[0]!=0;sptr = CharNext(sptr))
    {
        if (sptr[0] != _T('\''))
        {
            nsptr = CharNext(sptr);
            diff = (long) (nsptr-sptr);
            for (k=0; k<diff; k++)
            {
                outstr[j] = sptr[k];
                lastwasa = 0;
                j++;
            }
        }
        else
        {
            if (lastwasa != 0)
            {
                outstr[j] = _T('\'');
                lastwasa = 0;
                j++;
            }
            else lastwasa = 1;
        }
    }
    outstr[j] = 0;
}

void strtoupper(char *astr)
{
    /* Make astr uppercase. */
    long i;
    if (astr == NULL) return;
    for (i=0;astr[i]!=0;i++)
    {
        astr[i] = toupper(astr[i]);
    }
}

void tstrtoupper(LPTSTR astr)
{
    /* TCHAR version of above routine.
       Make astr uppercase. */
    long i;
    i = lstrlen(astr);
    CharUpperBuff(astr, i);
}

int tsafestrtoupper(LPTSTR astr, DWORD astrlen)
{
    /* Safe TCHAR version of above routine.
       Make astr of length astrlen uppercase. */
    unsigned long alen = (unsigned long) stringchlength(astr, (long) astrlen);
    /*if (SUCCEEDED( StringCchLength(astr, (size_t) astrlen, &alen ) != TRUE) return 0;*/
    if (alen<0) return 0;
    if (CharUpperBuff(astr, alen) != alen) return 0;
    return 1;
}

void stripspaces(char *astr)
{
    /* Remove spaces (and apostrophes) from astr. */
    long i, j=0;
    if (astr == NULL) return;
    for (i=0;astr[i]!=0;i++)
    {
        if (astr[i] != ' ' && astr[i] != '\'')
        {
            astr[j] = astr[i];
            j++;
        }
    }
    astr[j] = 0;
}

void tstripspaces(LPTSTR astr)
{
    /* TCHAR version of above routine.
       Remove spaces (and apostrophes) from astr. */
    long i, j=0, diff, k;
    LPTSTR sptr, nsptr;
    if (astr == NULL) return;
    for (sptr = astr;sptr[0]!=0;sptr = CharNext(sptr))
    {
        if (sptr[0] != _T(' ') && sptr[0] != _T('\''))
        {
            nsptr = CharNext(sptr);
            diff = (long) (nsptr-sptr);
            for (k=0; k<diff; k++)
            {
                astr[j] = sptr[k];
                j++;
            }
        }
    }
    astr[j] = 0;
}

void strleft(char *outstr, char *instr, int chars)
{
    /* Outstr = chars leftmost characters of instr.
       Outstr should be at least chars+1 characters in size! */
    int i;
    if (instr == NULL || outstr == NULL) return;
    for (i=0;i<chars;i++)
    {
        if (instr[i] == 0) break;
        outstr[i] = instr[i];
    }
    outstr[i] = 0;
}

int tstrleft(LPTSTR outstr, LPTSTR instr, int chars)
{
    /* TCHAR version of above routine.
       Outstr = chars leftmost characters of instr.
       Outstr should be at least chars+1 characters in size! */
    if (instr == NULL || outstr == NULL) return;
    if (stringchcopy(outstr, chars + 1, instr) != SAFETY_INVALID_PARAMETER) return 1;
    return 0;
}

int streq(char *astr, char *bstr)
{
    /* if astr == bstr then is 1, else is 0. */
    long al, bl;
    if (astr == NULL || bstr == NULL) return 0;
    al = strlen(astr) * sizeof(char);
    bl = strlen(bstr) * sizeof(char);
    if (al != bl) return 0;
    if (memcmp(astr, bstr, al) == 0) return 1;
    return 0;
}

int tstreq(LPTSTR astr, LPTSTR bstr)
{
    /* TCHAR version of above routine.
       if astr == bstr then is 1, else is 0. */
    long al, bl;
    if (astr == NULL || bstr == NULL) return 0;
    al = lstrlen(astr) * sizeof(TCHAR);
    bl = lstrlen(bstr) * sizeof(TCHAR);
    if (al != bl) return 0;
    if (memcmp(astr, bstr, al) == 0) return 1;
    return 0;
}

void stripmarkup(char *outstr, char *instr)
{
    /* Strip all the markup from instr and return as outstr.
       Outstr must be at least the size of instr for safety! */
    long i, j = 0;
    int inmarkup = 0, lastwasopener = 0;
    if (instr == NULL || outstr == NULL) return;
    for (i=0;instr[i]!=0;i++)
    {
        if (inmarkup != 0)
        {
            switch (instr[i])
            {
                case '[':
                  outstr[j] = '[';
                  j++;
                  inmarkup = 0;
                  lastwasopener = 0;
                break;

                case ']':
                  if (lastwasopener != 0)
                  {
                      outstr[j] = ']';
                      j++;
                      lastwasopener = 0;
                      inmarkup = 0;
                  }
                  else
                  {
                      inmarkup = 0;
                  }
                break;

                default:
                  lastwasopener = 0;
                break;
            }
        }
        else
        {
            switch (instr[i])
            {
                case '[':
                  inmarkup = 1;
                  lastwasopener = 1;
                break;

                default:
                  outstr[j] = instr[i];
                  j++;
                break;
            }
        }
    }
    outstr[j] = 0;
}

void tstripmarkup(LPTSTR outstr, LPTSTR instr)
{
    /* TCHAR version of above routine.
       Strip all the markup from instr and return as outstr.
       Outstr must be at least the size of instr for safety! */
    long i, j = 0, diff, k;
    LPTSTR sptr, nsptr;
    int inmarkup = 0, lastwasopener = 0;
    if (instr == NULL || outstr == NULL) return;
    for (sptr = instr;sptr[0]!=0;sptr = CharNext(sptr))
    {
        if (inmarkup != 0)
        {
            switch (sptr[0])
            {
                case _T('['):
                  outstr[j] = _T('[');
                  j++;
                  inmarkup = 0;
                  lastwasopener = 0;
                break;

                case _T(']'):
                  if (lastwasopener != 0)
                  {
                      outstr[j] = _T(']');
                      j++;
                      lastwasopener = 0;
                      inmarkup = 0;
                  }
                  else
                  {
                      inmarkup = 0;
                  }
                break;

                default:
                  lastwasopener = 0;
                break;
            }
        }
        else
        {
            switch (sptr[0])
            {
                case _T('['):
                  inmarkup = 1;
                  lastwasopener = 1;
                break;

                default:
                  nsptr = CharNext(sptr);
                  diff = (long) (nsptr-sptr);
                  for (k=0; k<diff; k++)
                  {
                    outstr[j] = sptr[k];
                    j++;
                  }
                break;
            }
        }
    }
    outstr[j] = 0;
}

void firstlineonly(char *astr)
{
    /* Shorten string to be only the first line. */
    long i;
    for (i=0;astr[i]!=0;i++)
    {
        if (astr[i]=='\n')
        {
            if (i>0 && astr[i-1]=='\r') astr[i-1] = 0;
            else astr[i] = 0;
            return;
        }
    }
}

void tfirstlineonly(LPTSTR astr)
{
    /* TCHAR version of above routine.
       Shorten string to be only the first line. */
    long i;
    
    for (i=0;astr[i]!=0;i++)
    {
        if (astr[i]==_T('\n'))
        {
            if (i>0 && astr[i-1]==_T('\r')) astr[i-1] = 0;
            else astr[i] = 0;
            return;
        }
    }
}

long tsafestrlen(LPTSTR astr)
{
      /* Safer version of lstrlen - wrapper */
      unsigned long alen;
      alen = stringchlength(astr, SAFETY_MAX_CH);
      if (alen<0) return 0;
      /*if (SUCCEEDED(StringCchLength( astr, SAFETY_MAX_CH, &alen )) == TRUE) return alen;*/
      return alen;
}

void shift4charsleft(char *chars4, char newchar)
{
    /* Shift a block of 4 characters to the left, shifting in newchar and dropping the 'carry'. */
    int i;
    char a = newchar, b;
    for (i=3;i>=0;i--)
    {
        b = chars4[i];
        chars4[i] = a;
        a = b;
    }
}

void tshift4charsleft(LPTSTR chars4, TCHAR newchar)
{
    /* TCHAR version of above routine.
       Shift a block of 4 characters to the left, shifting in newchar and dropping the 'carry'. */
    int i;
    TCHAR a = newchar, b;
    for (i=3;i>=0;i--)
    {
        b = chars4[i];
        chars4[i] = a;
        a = b;
    }
}

char **slidessplit(char *songtext)
{
    /* Split the slides into a Null-terminated string array */
    long nslides = 1, i, j = 0, m = 0;
    char last4[5] = "    ";
    char **slides = NULL;
    char *rdslide = NULL;
    
    /* Count number of slides first */
    for (i=0;songtext[i]!=0;i++)
    {
        shift4charsleft(last4,songtext[i]);
        if (streq(last4,"\r\n\r\n") != 0 || streq(last4+(2*sizeof(char)),"\n\n") != 0)
        {
            nslides++;
            last4[3] = ' ';
        }
    }

    /* Create the slides array */
    slides = (char **) malloc((nslides+1) * sizeof(char *));
    if (slides == NULL) return NULL;
    slides[nslides] = NULL;

    /* Create the slides in the array */
    rdslide = (char *) malloc((1+strlen(songtext))*sizeof(char));
    if (rdslide == NULL)
    {
        free(slides);
        return NULL;
    }
    for (i=0;songtext[i]!=0;i++)
    {
        shift4charsleft(last4,songtext[i]);
        if (streq(last4,"\r\n\r\n") != 0 || streq(last4+(2*sizeof(char)),"\n\n") != 0)
        {
            rdslide[j] = 0;
            slides[m] = (char *) malloc(sizeof(char)*(1+strlen(rdslide)));
            if (slides[m] == NULL)
            {
                free(rdslide);
                for (i=0;i<m;i++) free(slides[i]);
                free(slides);
                return NULL;
            }
            strcpy(slides[m],rdslide);
            
            m++;
            if (m>=nslides)
            {
                /* Should never happen... */
                free(rdslide);
                for (i=0;i<m;i++) free(slides[i]);
                free(slides);
                return NULL;
            }
            rdslide[0] = 0;
            j = 0;
            last4[3] = ' ';
        }
        else
        {
            rdslide[j] = songtext[i];
            j++;
        }
    }
    rdslide[j] = 0;
    slides[m] = (char *) malloc(sizeof(char)*(1+strlen(rdslide)));
    if (slides[m] == NULL)
    {
        free(rdslide);
        for (i=0;i<m;i++) free(slides[i]);
        free(slides);
        return NULL;
    }
    strcpy(slides[m],rdslide);
    free(rdslide);
    
    return slides;
}

LPTSTR *tslidessplit(LPTSTR songtext)
{
    /* TCHAR version of above routine.
       Split the slides into a Null-terminated string array */
    long nslides = 1, i, j = 0, m = 0;
    TCHAR last4[5] = _T("    ");
    LPTSTR *slides = NULL;
    LPTSTR rdslide = NULL;
    
    /* Count number of slides first */
    for (i=0;songtext[i]!=0;i++)
    {
        tshift4charsleft(last4,songtext[i]);
        if (tstreq(last4,_T("\r\n\r\n")) != 0 || tstreq(last4+(2*sizeof(TCHAR)),_T("\n\n")) != 0)
        {
            nslides++;
            last4[3] = _T(' ');
        }
    }

    /* Create the slides array */
    slides = (LPTSTR *) malloc((nslides+1) * sizeof(LPTSTR));
    if (slides == NULL) return NULL;
    slides[nslides] = NULL;

    /* Create the slides in the array */
    rdslide = (LPTSTR) malloc((1+tsafestrlen(songtext))*sizeof(TCHAR));
    if (rdslide == NULL)
    {
        free(slides);
        return NULL;
    }
    for (i=0;songtext[i]!=0;i++)
    {
        tshift4charsleft(last4,songtext[i]);
        if (tstreq(last4,_T("\r\n\r\n")) != 0 || tstreq(last4+(2*sizeof(TCHAR)),_T("\n\n")) != 0)
        {
            rdslide[j] = 0;
            slides[m] = (LPTSTR) malloc(sizeof(TCHAR)*(1+tsafestrlen(rdslide)));
            if (slides[m] == NULL)
            {
                free(rdslide);
                for (i=0;i<m;i++) free(slides[i]);
                free(slides);
                return NULL;
            }
            //strcpy(slides[m],rdslide);
            stringchcopy(slides[m], (1+tsafestrlen(rdslide)), rdslide);
            
            m++;
            if (m>=nslides)
            {
                /* Should never happen... */
                free(rdslide);
                for (i=0;i<m;i++) free(slides[i]);
                free(slides);
                return NULL;
            }
            rdslide[0] = 0;
            j = 0;
            last4[3] = _T(' ');
        }
        else
        {
            rdslide[j] = songtext[i];
            j++;
        }
    }
    rdslide[j] = 0;
    slides[m] = (LPTSTR) malloc(sizeof(TCHAR)*(1+tsafestrlen(rdslide)));
    if (slides[m] == NULL)
    {
        free(rdslide);
        for (i=0;i<m;i++) free(slides[i]);
        free(slides);
        return NULL;
    }
    //strcpy(slides[m],rdslide);
    stringchcopy(slides[m], (1+tsafestrlen(rdslide)), rdslide);
    free(rdslide);
    
    return slides;
}

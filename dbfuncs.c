#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void stripspaces(char *astr)
{
    /* Remove spaces from astr. */
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

int streq(char *astr, char *bstr)
{
    /* if astr == bstr then is 1, else is 0. */
    long al, bl;
    if (astr == NULL || bstr == NULL) return 0;
    al = strlen(astr);
    bl = strlen(bstr);
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

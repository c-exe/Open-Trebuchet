/* DBInsert.c - Functions for inserting data from the database into the listboxes */

#include <windows.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>

#include "resource.h"
#include "main.h"
#include "dbfuncs.h"

#include "dbinsert.h"


static int songcallback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;
  int flagsnum = 0, isfav = 0, titleoffset = -1;
  char *utf8list = NULL;
  LPTSTR itemtext = NULL;
  /* 1: get the columns
     2: Use UTF8StrToTSTR(LPTSTR tstr, char *utf8str, TCHAR unknownchar) to convert
     3: Use BOOL AddItemToListBox(IDC_SONGS_LISTBOX, LPTSTR ItemText) to add
     4: repeat until done! */
  titleoffset = -1;
  for(i=0; i<argc; i++)
  {
      if (streq(azColName[i], "Flags") && argv[i] != NULL)
      {
          sscanf(argv[i],"%d",&flagsnum);
          if ((flagsnum & 1) != 0) isfav = 1;
          else isfav = 0;
      }
      else if (streq(azColName[i], "SongTitle") && argv[i] != NULL)
      {
          titleoffset = i;
      }
  }
  if (titleoffset == -1) return 0; //Not an error, just skip this record

  utf8list = (char *) malloc(sizeof(char) * (4+strlen(argv[titleoffset])));
  if (utf8list == NULL) return -1;

  if (isfav!=0) strcpy(utf8list,"F: "); else strcpy(utf8list,"_: ");
  strcat(utf8list, argv[titleoffset]);

  itemtext = (LPTSTR) malloc(sizeof(TCHAR) * (1+strlen(utf8list)));
  if (itemtext == NULL)
  {
      free(utf8list);
      return -1;
  }
  UTF8StrToTSTR(itemtext,utf8list,_T('?'));
  free(utf8list);

  if (AddItemToListBox(IDC_SONGS_LISTBOX, itemtext) == FALSE)
  {
      free(itemtext);
      return -1;
  }

  free(itemtext);
  
  return 0;
}

int SongLBSuck(LPTSTR SongFilter, BOOL FavOnly, BOOL IncSong, BOOL IncHymn, BOOL IncLit, BOOL IncMeta)
{
    sqlite3 *db;
    char *AnErrMsg = NULL;
    char *zErrMsg = NULL;
    char *SFUTF8 = NULL;
    LPTSTR EscSF = NULL;
    char SQLStatement[3072] = "";
    char WhereStatement[2048] = "";
    char FlagStatement[256] = "";
    int rc;

    rc = sqlite3_open(DB_FILENAME, &db); //Do something about this, it should not be hardcoded like this!
    if( rc )
    {
        AnErrMsg = (char *) malloc(2048*sizeof(char));
        if (AnErrMsg != NULL)
        {
            sprintf(AnErrMsg, "Cannot open database: %s.", sqlite3_errmsg(db));
            MessageBoxA(NULL, AnErrMsg, "Database Error!", MB_ICONERROR | MB_OK);
            free(AnErrMsg);
        } else MessageBoxA(NULL, "Cannot open database.", "Database Error!", MB_ICONERROR | MB_OK);
        sqlite3_close(db);
        return 0;
    }
    if (SongFilter != NULL && SongFilter[0] != 0)
    {
        EscSF = (TCHAR *) malloc(sizeof(TCHAR)*(4*(1+1+_tcslen(SongFilter))));
        if (EscSF == NULL)
        {
            MessageBoxA(NULL, "Out of Memory!", "Database Error!", MB_ICONERROR | MB_OK);
            sqlite3_close(db);
            return 0;
        }
        if (tescapeforlike(EscSF,SongFilter) == 0)
        {
            MessageBoxA(NULL, "Error Converting Filter!", "Database Error!", MB_ICONERROR | MB_OK);
            free(EscSF);
            sqlite3_close(db);
            return 0;
        }
        _tcscat(EscSF,_T("%"));
        
        SFUTF8 = (char *) malloc(sizeof(char)*(3*(1+_tcslen(EscSF))));
        if (SFUTF8 == NULL)
        {
            MessageBoxA(NULL, "Out of Memory!", "Database Error!", MB_ICONERROR | MB_OK);
            free(EscSF);
            sqlite3_close(db);
            return 0;
        }
        if (TSTRToUTF8Str(SFUTF8,EscSF) == 0)
        {
            MessageBoxA(NULL, "Out of Memory!", "Database Error!", MB_ICONERROR | MB_OK);
            free(EscSF);
            free(SFUTF8);
            sqlite3_close(db);
            return 0;
        }
        free(EscSF);
        strcpy(WhereStatement,"WHERE SongTitle LIKE '");
        strcat(WhereStatement,SFUTF8);
        strcat(WhereStatement,"' ESCAPE '\\' ");
        free(SFUTF8);
    }
    if (FavOnly == TRUE)
    {
        if (WhereStatement[0] == 0)
        {
            strcpy(WhereStatement, "WHERE ");
        }
        else
        {
            strcat(WhereStatement, "AND ");
        }
        sprintf(FlagStatement, "(Flags & %d) > 0 ", DBFLAG_FAV);
        strcat(WhereStatement, FlagStatement);
    }
    if (IncSong == FALSE)
    {
        if (WhereStatement[0] == 0)
        {
            strcpy(WhereStatement, "WHERE ");
        }
        else
        {
            strcat(WhereStatement, "AND ");
        }
        sprintf(FlagStatement, "(Flags & %d) != %d ", DBFLAG_TYPE, DBFLAG_TYPE_SONG);
        strcat(WhereStatement, FlagStatement);
    }
    if (IncHymn == FALSE)
    {
        if (WhereStatement[0] == 0)
        {
            strcpy(WhereStatement, "WHERE ");
        }
        else
        {
            strcat(WhereStatement, "AND ");
        }
        sprintf(FlagStatement, "(Flags & %d) != %d ", DBFLAG_TYPE, DBFLAG_TYPE_HYMN);
        strcat(WhereStatement, FlagStatement);
    }
    if (IncLit == FALSE)
    {
        if (WhereStatement[0] == 0)
        {
            strcpy(WhereStatement, "WHERE ");
        }
        else
        {
            strcat(WhereStatement, "AND ");
        }
        sprintf(FlagStatement, "(Flags & %d) != %d ", DBFLAG_TYPE, DBFLAG_TYPE_LITURGY);
        strcat(WhereStatement, FlagStatement);
    }
    if (IncMeta == FALSE)
    {
        if (WhereStatement[0] == 0)
        {
            strcpy(WhereStatement, "WHERE ");
        }
        else
        {
            strcat(WhereStatement, "AND ");
        }
        sprintf(FlagStatement, "(Flags & %d) != %d ", DBFLAG_TYPE, DBFLAG_TYPE_META);
        strcat(WhereStatement, FlagStatement);
    }
    sprintf(SQLStatement,"SELECT SongTitle, Flags FROM Song %s LIMIT 32000;",WhereStatement);
    
    rc = sqlite3_exec(db, SQLStatement, songcallback, 0, &zErrMsg);

    if( rc!=SQLITE_OK )
    {
        if (zErrMsg != NULL) AnErrMsg = (char *) malloc((256+strlen(zErrMsg))*sizeof(char));
        else AnErrMsg = (char *) malloc(256*sizeof(char));
        if (AnErrMsg != NULL)
        {
            if (zErrMsg != NULL) sprintf(AnErrMsg, "SQL error: %s.", zErrMsg);
            else strcpy(AnErrMsg, "Unknown Database Error!");
            MessageBoxA(NULL, AnErrMsg, "Database Error!", MB_ICONERROR | MB_OK);
            free(AnErrMsg);
        }
        else
        {
            MessageBoxA(NULL, "SQL Error!", "Database Error!", MB_ICONERROR | MB_OK);
        }
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return 0;
    }
    sqlite3_close(db);
    return 1;
}

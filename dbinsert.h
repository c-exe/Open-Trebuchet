/* DBInsert.h - Functions for inserting data from the database into the listboxes */
#ifndef __DBINSERT_H__

#define __DBINSERT_H__ 1

#define DBFLAG_FAV 1
#define DBFLAG_TYPE 6
#define DBFLAG_TYPE_SONG 0
#define DBFLAG_TYPE_HYMN 2
#define DBFLAG_TYPE_LITURGY 4
#define DBFLAG_TYPE_META 6
#define DB_FILENAME "otdb.db"

#include <windows.h>

static int songcallback(void *NotUsed, int argc, char **argv, char **azColName);
static int songflcallback(void *NotUsed, int argc, char **argv, char **azColName);
int SongLBSuck(LPTSTR SongFilter, BOOL FavOnly, BOOL IncSong, BOOL IncHymn, BOOL IncLit, BOOL IncMeta);
int SongFLLBSuck(LPTSTR SongFLFilter, BOOL FavOnly, BOOL IncSong, BOOL IncHymn, BOOL IncLit, BOOL IncMeta);


#endif

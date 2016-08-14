#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>


int NumRes = 0;
char *resnote[16] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
int resverse = 0, resnotenum = 0;
char resvid[64] = "";

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

static int getresultsn(void *NotUsed, int argc, char **argv, char **azColName){
  NumRes = argc;
  return 0;
}


void freeresnotes()
{
    int i;
    for (i=0;i<16;i++)
    {
        if (resnote[i] != NULL)
        {
            free(resnote[i]);
            resnote[i] = NULL;
        }
    }
}

void strtoupper(char *astr)
{
    long i;
    for (i=0;astr[i]!=0;i++)
    {
        astr[i] = toupper(astr[i]);
    }
}

void stripspaces(char *astr)
{
    long i, j=0;
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

void escapeapostrophes(char *outstr, char *instr)
{
    long i, j=0;
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

int streq(char *astr, char *bstr)
{
    long al, bl;
    if (astr == NULL || bstr == NULL) return 0;
    al = strlen(astr);
    bl = strlen(bstr);
    if (al != bl) return 0;
    if (memcmp(astr, bstr, al) == 0) return 1;
    return 0;
}

int main(int argc, char **argv)
{
  FILE *bibfile;
  char rdbyte;
  char tmpstr[65535] = "", *tcol = NULL;
  char *col0 = NULL /*ID*/, *col1 = NULL /*Trans*/, *col2 = NULL /*Book*/, *col3 = NULL /*Chapter*/;
  char *col4 = NULL /*Verse*/, *col5 = NULL /*Content*/, *col6 = NULL /*IsNote*/, *col7 = NULL /*HasNote*/;
  int fieldnum = 0, charnum = 0, hasread = 0, isnote = 0;
  unsigned long enteredrecs = 0;
  char *transid = NULL, *bookid = NULL, vid[64] = "", checkst[256] = "";
  int verse = 0, chapter = 0;
  
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  if( argc!=2 ){
    fprintf(stderr, "Usage: %s DATABASE BIBLE-FILE\r\n", argv[0]);
    return(1);
  }
  
  bibfile = fopen(argv[2],"rb");
  if (bibfile == NULL)
  {
      fprintf(stderr, "Cannot open Bible-File %s!\r\n", argv[2]);
      return 1;
  }
  
  rc = sqlite3_open(argv[1], &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    fclose(bibfile);
    return(2);
  }
  
  while (fread(&rdbyte,1,1,bibfile) > 0)
  {
      if (rdbyte == 0x1E)
      {
          if (hasread != 0)
          {
              /*Write to DB, then free cols*/
              /*Format Cols:*/
              if (transid != NULL) free(transid);
              if (col1 != NULL && col1[0] != 0)
              {
                  transid = malloc(sizeof(char)*(1+strlen(col1)));
                  if (transid == NULL)
                  {
                      /*OoM*/
                      fprintf(stderr, "Out of Memory after writing %lu records to DB!\r\n", enteredrecs);
                      fclose(bibfile);
                      if (col0 != NULL) free(col0);
                      if (col1 != NULL) free(col1);
                      if (col2 != NULL) free(col2);
                      if (col3 != NULL) free(col3);
                      if (col4 != NULL) free(col4);
                      if (col5 != NULL) free(col5);
                      if (col6 != NULL) free(col6);
                      if (col7 != NULL) free(col7);
                      if (transid != NULL) free(transid);
                      if (bookid != NULL) free(bookid);
                      freeresnotes();
                      sqlite3_close(db);
                      return 3;
                  }
                  strcpy(transid,col1);
                  strtoupper(transid);
                  stripspaces(transid);
                  if (transid[0] == 0)
                  {
                      strcpy(transid,"_");
                  }
                  else if (strlen(transid) > 16)
                  {
                      transid[16] = 0;
                  }
              }
              else
              {
                  transid = malloc(sizeof(char)*8);
                  if (transid == NULL)
                  {
                      /* OoM */
                      fprintf(stderr, "Out of Memory after writing %lu records to DB!\r\n", enteredrecs);
                      fclose(bibfile);
                      if (col0 != NULL) free(col0);
                      if (col1 != NULL) free(col1);
                      if (col2 != NULL) free(col2);
                      if (col3 != NULL) free(col3);
                      if (col4 != NULL) free(col4);
                      if (col5 != NULL) free(col5);
                      if (col6 != NULL) free(col6);
                      if (col7 != NULL) free(col7);
                      if (transid != NULL) free(transid);
                      if (bookid != NULL) free(bookid);
                      freeresnotes();
                      sqlite3_close(db);
                      return 3;
                  }
                  strcpy(transid,"UNKNOWN");
              }
              if (col1 == NULL) col1 = transid;
              if (strlen(col1)>255) col1[255] = 0;
              if (bookid != NULL) free(bookid);
              if (col2 != NULL && col2[0] != 0)
              {
                  bookid = malloc(sizeof(char)*(1+strlen(col2)));
                  if (bookid == NULL)
                  {
                      /*OoM*/
                      fprintf(stderr, "Out of Memory after writing %lu records to DB!\r\n", enteredrecs);
                      fclose(bibfile);
                      if (col0 != NULL) free(col0);
                      if (col1 != NULL) free(col1);
                      if (col2 != NULL) free(col2);
                      if (col3 != NULL) free(col3);
                      if (col4 != NULL) free(col4);
                      if (col5 != NULL) free(col5);
                      if (col6 != NULL) free(col6);
                      if (col7 != NULL) free(col7);
                      if (transid != NULL) free(transid);
                      if (bookid != NULL) free(bookid);
                      freeresnotes();
                      sqlite3_close(db);
                      return 3;
                  }
                  strcpy(bookid,col2);
                  strtoupper(bookid);
                  stripspaces(bookid);
                  if (bookid[0] == 0)
                  {
                      strcpy(bookid,"_");
                  }
              }
              else
              {
                  bookid = malloc(sizeof(char)*8);
                  if (bookid == NULL)
                  {
                      /* OoM */
                      fprintf(stderr, "Out of Memory after writing %lu records to DB!\r\n", enteredrecs);
                      fclose(bibfile);
                      if (col0 != NULL) free(col0);
                      if (col1 != NULL) free(col1);
                      if (col2 != NULL) free(col2);
                      if (col3 != NULL) free(col3);
                      if (col4 != NULL) free(col4);
                      if (col5 != NULL) free(col5);
                      if (col6 != NULL) free(col6);
                      if (col7 != NULL) free(col7);
                      if (transid != NULL) free(transid);
                      if (bookid != NULL) free(bookid);
                      freeresnotes();
                      sqlite3_close(db);
                      return 3;
                  }
                  strcpy(bookid,"UNKNOWN");
              }
              if (col2 == NULL) col2 = bookid;
              if (strlen(col2)>255) col2[255] = 0;
              chapter = 0;
              if (col3 != NULL)
              {
                  stripspaces(col3);
                  if (col3[0] != 0)
                  {
                      sscanf(col3,"%d",&chapter);
                  }
              }
              verse = 0;
              if (col4 != NULL)
              {
                  stripspaces(col4);
                  if (col4[0] != 0)
                  {
                      sscanf(col4,"%d",&verse);
                  }
              }
              isnote = 0;
              if (col6 != NULL)
              {
                  stripspaces(col6);
                  if (col6[0] != 0)
                  {
                      sscanf(col6,"%d",&isnote);
                  }
              }
              sprintf(vid,"%s_%s%04X%04X",transid,bookid,chapter,verse);

              /*Escape cols 1, 2 & 5:*/
              tcol = malloc(sizeof(char)*2*(1+strlen(col1)));
              if (tcol == NULL)
              {
                  /*OoM*/
                  fprintf(stderr, "Out of Memory after writing %lu records to DB!\r\n", enteredrecs);
                  fclose(bibfile);
                  if (col0 != NULL) free(col0);
                  if (col1 != NULL) free(col1);
                  if (col2 != NULL) free(col2);
                  if (col3 != NULL) free(col3);
                  if (col4 != NULL) free(col4);
                  if (col5 != NULL) free(col5);
                  if (col6 != NULL) free(col6);
                  if (col7 != NULL) free(col7);
                  if (transid != NULL) free(transid);
                  if (bookid != NULL) free(bookid);
                  freeresnotes();
                  sqlite3_close(db);
                  return 3;
              }
              escapeapostrophes(tcol,col1);
              free(col1);
              col1 = tcol;
              tcol = malloc(sizeof(char)*2*(1+strlen(col2)));
              if (tcol == NULL)
              {
                  /*OoM*/
                  fprintf(stderr, "Out of Memory after writing %lu records to DB!\r\n", enteredrecs);
                  fclose(bibfile);
                  if (col0 != NULL) free(col0);
                  if (col1 != NULL) free(col1);
                  if (col2 != NULL) free(col2);
                  if (col3 != NULL) free(col3);
                  if (col4 != NULL) free(col4);
                  if (col5 != NULL) free(col5);
                  if (col6 != NULL) free(col6);
                  if (col7 != NULL) free(col7);
                  if (transid != NULL) free(transid);
                  if (bookid != NULL) free(bookid);
                  freeresnotes();
                  sqlite3_close(db);
                  return 3;
              }
              escapeapostrophes(tcol,col2);
              free(col2);
              col2 = tcol;
              tcol = malloc(sizeof(char)*2*(1+strlen(col5)));
              if (tcol == NULL)
              {
                  /*OoM*/
                  fprintf(stderr, "Out of Memory after writing %lu records to DB!\r\n", enteredrecs);
                  fclose(bibfile);
                  if (col0 != NULL) free(col0);
                  if (col1 != NULL) free(col1);
                  if (col2 != NULL) free(col2);
                  if (col3 != NULL) free(col3);
                  if (col4 != NULL) free(col4);
                  if (col5 != NULL) free(col5);
                  if (col6 != NULL) free(col6);
                  if (col7 != NULL) free(col7);
                  if (transid != NULL) free(transid);
                  if (bookid != NULL) free(bookid);
                  freeresnotes();
                  sqlite3_close(db);
                  return 3;
              }
              escapeapostrophes(tcol,col5);
              free(col5);
              col5 = tcol;
              tcol = NULL;
              
              /*Write any leftover notes if moved on:*/
              if (resnotenum > 0 && verse != resverse && streq(vid,resvid) == 0)
              {
                  int i;
                  for (i=0;i<resnotenum;i++)
                  {
                      if (resnote[i] != NULL)
                      {
                          rc = sqlite3_exec(db, resnote[i], getresultsn, 0, &zErrMsg);
                          if( rc!=SQLITE_OK ){
                            fprintf(stderr, "SQL error adding note: %s\n", zErrMsg);
                            sqlite3_free(zErrMsg);
                          }
                      }
                  }
                  freeresnotes();
              }
              
              /*Write to DB:*/
              sprintf(checkst,"SELECT * FROM BibleTrans WHERE BibID = '%s';",transid);
              rc = sqlite3_exec(db, checkst, getresultsn, 0, &zErrMsg);
              if( rc!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
              }
              if (NumRes == 0)
              {
                  sprintf(tmpstr,"INSERT INTO BibleTrans(BibID, Name, Copyright, Year, Notes) VALUES ('%s', '%s', NULL, NULL, NULL);",transid,col1);
                  rc = sqlite3_exec(db, tmpstr, getresultsn, 0, &zErrMsg);
                  if( rc!=SQLITE_OK ){
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                  }
              }
              
              sprintf(checkst,"SELECT * FROM BibleBook WHERE BookID = '%s';",bookid);
              rc = sqlite3_exec(db, checkst, getresultsn, 0, &zErrMsg);
              if( rc!=SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
              }
              if (NumRes == 0)
              {
                  sprintf(tmpstr,"INSERT INTO BibleBook(BookID, Name) VALUES ('%s', '%s');",bookid,col2);
                  rc = sqlite3_exec(db, tmpstr, getresultsn, 0, &zErrMsg);
                  if( rc!=SQLITE_OK ){
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                  }
              }

              if (strlen(col5)>65000)
              {
                  tcol = malloc(sizeof(char)*(501+strlen(col5)));
                  if (tcol == NULL)
                  {
                      /*OoM*/
                      fprintf(stderr, "Out of Memory after writing %lu records to DB!\r\n", enteredrecs);
                      fclose(bibfile);
                      if (col0 != NULL) free(col0);
                      if (col1 != NULL) free(col1);
                      if (col2 != NULL) free(col2);
                      if (col3 != NULL) free(col3);
                      if (col4 != NULL) free(col4);
                      if (col5 != NULL) free(col5);
                      if (col6 != NULL) free(col6);
                      if (col7 != NULL) free(col7);
                      if (transid != NULL) free(transid);
                      if (bookid != NULL) free(bookid);
                      freeresnotes();
                      sqlite3_close(db);
                      return 3;
                  }
              }
              else
              {
                  tcol = tmpstr;
              }
              if (isnote == 0)
              {
                  /*Do Verse*/
                  sprintf(tcol, "INSERT INTO BibleVerse (VerseID, BibID, BookID, ChapterNum, VerseNum, VerseText) VALUES ('%s', '%s', '%s', %d, %d, '%s');", vid, transid, bookid, chapter, verse, col5);
                  rc = sqlite3_exec(db, tcol, getresultsn, 0, &zErrMsg);
                  if( rc!=SQLITE_OK ){
                    fprintf(stderr, "SQL error adding verse: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                  }
              }
              else
              {
                  /*Do Note*/
                  sprintf(tcol, "INSERT INTO BibleNote (NoteID, VerseID, NoteText) VALUES (NULL, '%s', '%s');", vid, col5);

                  sprintf(checkst,"SELECT * FROM BibleVerse WHERE VerseID = '%s';",vid);
                  rc = sqlite3_exec(db, checkst, getresultsn, 0, &zErrMsg);
                  if( rc!=SQLITE_OK ){
                    fprintf(stderr, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                  }
                  if (NumRes == 0)
                  {
                      resverse = verse;
                      if (resnotenum > 15)
                      {
                          fprintf(stderr, "Too many notes on VerseID %s!  Ignoring this one.\r\n",vid);
                      }
                      else
                      {
                          resnote[resnotenum] = malloc(sizeof(char)*(1+strlen(tcol)));
                          if (resnote[resnotenum] == NULL)
                          {
                              /*OoM*/
                              fprintf(stderr, "Out of Memory after writing %lu records to DB!\r\n", enteredrecs);
                              fclose(bibfile);
                              if (col0 != NULL) free(col0);
                              if (col1 != NULL) free(col1);
                              if (col2 != NULL) free(col2);
                              if (col3 != NULL) free(col3);
                              if (col4 != NULL) free(col4);
                              if (col5 != NULL) free(col5);
                              if (col6 != NULL) free(col6);
                              if (col7 != NULL) free(col7);
                              if (transid != NULL) free(transid);
                              if (bookid != NULL) free(bookid);
                              freeresnotes();
                              sqlite3_close(db);
                              return 3;
                          }
                          strcpy(resnote[resnotenum], tcol);
                          strcpy(resvid, vid);
                          resnotenum++;
                      }
                  }
                  else
                  {
                      rc = sqlite3_exec(db, tcol, getresultsn, 0, &zErrMsg);
                      if( rc!=SQLITE_OK ){
                        fprintf(stderr, "SQL error adding note: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                      }
                  }
                  
              }
              if (tcol != tmpstr) free(tcol);
              tcol = NULL;
              
              /*Freeing Cols:*/
              if (col0 != NULL) free(col0);
              if (col1 != NULL) free(col1);
              if (col2 != NULL) free(col2);
              if (col3 != NULL) free(col3);
              if (col4 != NULL) free(col4);
              if (col5 != NULL) free(col5);
              if (col6 != NULL) free(col6);
              if (col7 != NULL) free(col7);
              if (transid != NULL) free(transid);
              if (bookid != NULL) free(bookid);
              col0 = NULL;
              col1 = NULL;
              col2 = NULL;
              col3 = NULL;
              col4 = NULL;
              col5 = NULL;
              col6 = NULL;
              col7 = NULL;
              transid = NULL;
              bookid = NULL;
          }
          fieldnum = 0;
      }
      else if (rdbyte == 0x1F)
      {
          tmpstr[charnum] = 0;
          if (fieldnum < 8)
          {
              tcol = malloc(sizeof(char)*(charnum+1));
              if (tcol == NULL)
              {
                  /*OoM*/
                  fprintf(stderr, "Out of Memory after writing %lu records to DB!\r\n", enteredrecs);
                  fclose(bibfile);
                  if (col0 != NULL) free(col0);
                  if (col1 != NULL) free(col1);
                  if (col2 != NULL) free(col2);
                  if (col3 != NULL) free(col3);
                  if (col4 != NULL) free(col4);
                  if (col5 != NULL) free(col5);
                  if (col6 != NULL) free(col6);
                  if (col7 != NULL) free(col7);
                  if (transid != NULL) free(transid);
                  if (bookid != NULL) free(bookid);
                  freeresnotes();
                  sqlite3_close(db);
                  return 3;
              }
              strcpy(tcol,tmpstr);
          }
          switch (fieldnum)
          {
              case 0:
                col0 = tcol;
              break;

              case 1:
                col1 = tcol;
              break;

              case 2:
                col2 = tcol;
              break;

              case 3:
                col3 = tcol;
              break;

              case 4:
                col4 = tcol;
              break;

              case 5:
                col5 = tcol;
              break;

              case 6:
                col6 = tcol;
              break;

              case 7:
                col7 = tcol;
              break;
          }
          tcol = NULL;
          fieldnum++;
          charnum = 0;
          tmpstr[0] = 0;
      }
      else
      {
          tmpstr[charnum] = rdbyte;
          charnum++;
          hasread = 1;
      }
  }
  if (!(eof(bibfile)))
  {
      fprintf(stderr, "Error reading file after writing %lu records to DB!\r\n", enteredrecs);
  }
  else
  {
      printf("%lu records written to DB!\r\n",enteredrecs);
  }
  
  /*rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }*/
  fclose(bibfile);
  if (col0 != NULL) free(col0);
  if (col1 != NULL) free(col1);
  if (col2 != NULL) free(col2);
  if (col3 != NULL) free(col3);
  if (col4 != NULL) free(col4);
  if (col5 != NULL) free(col5);
  if (col6 != NULL) free(col6);
  if (col7 != NULL) free(col7);
  if (transid != NULL) free(transid);
  if (bookid != NULL) free(bookid);
  sqlite3_close(db);
  freeresnotes();
  return 0;
}

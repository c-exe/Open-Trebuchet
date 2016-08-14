# Open Trebuchet Database Design

The following is the design for the back-end database for Open Trebuchet.  This design will be implemented in SQLite.  For the older design for MySQL, see the document "DatabaseDesignForMySQL.md".

Since SQLite does not implement users and privileges, these will be managed by the program.  As SQLite has limited data-types, the sizes of fields will be detailed in the notes columns.

## BibleTrans:

The Bible translations stored.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
BibID | TEXT | NO | Primary | NULL |   | (16-Character Translation) identifier (E.G. WEB)
Name | TEXT | NO | NO | NULL |    | (255-Character) Name of the translation (E.G. World English Bible)
Copyright | TEXT | YES | NO | NULL |    | (65535-Character) Copyright information for the Bible translation
Year | TEXT | YES | NO | NULL |    | (8-Character) Year of publication
Notes | TEXT | YES | NO | NULL |    | (65535-Character) Any extra information the publisher wishes to store

```sqlite
CREATE TABLE BibleTrans (
BibID TEXT NOT NULL PRIMARY KEY,
Name TEXT NOT NULL,
Copyright TEXT DEFAULT NULL,
Year TEXT DEFAULT NULL,
Notes TEXT DEFAULT NULL
);
```


## BibleBook:

The Names of the Books.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
BookID | TEXT | NO | Primary | NULL |    | (16-Character) Short name of Book (E.G. 2CHRON)
Name | TEXT | NO | NO | NULL |    | (255-Character) Name of Book (E.G. 2 Chronicles)

```sqlite
CREATE TABLE BibleBook (
BookID TEXT NOT NULL PRIMARY KEY,
Name TEXT NOT NULL
);
```


## BibleVerse:

The actual verses of the Bible.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
VerseID | TEXT | NO | Primary | NULL |    | (64-Character) Verse ID, format: {BibID}{BookID}HEX({ChapterNum})HEX({VerseNum})
BibID | TEXT | NO | Foreign | NULL |    | (16-Character) Bible ID, Links to BibleTrans
BookID | TEXT | NO | Foreign | NULL |    | (16-Character) Book ID Links to BibleBook
ChapterNum | INTEGER | NO | NO | 0 |    | (0-32767) Chapter
VerseNum | INTEGER | NO | NO | 0 |    | (0-32767) Verse
VerseText | TEXT | YES | NO | NULL |    | (65535-Chararcter) Text of the verse

```sqlite
CREATE TABLE BibleVerse (
VerseID TEXT NOT NULL PRIMARY KEY,
BibID TEXT NOT NULL,
BookID TEXT NOT NULL,
ChapterNum INTEGER NOT NULL DEFAULT 0,
VerseNum INTEGER NOT NULL DEFAULT 0,
VerseText TEXT DEFAULT NULL,
FOREIGN KEY (BibID) REFERENCES BibleTrans (BibID),
FOREIGN KEY (BookID) REFERENCES BibleBook (BookID)
);
```


## BibleNote:

Some verses have translation notes.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
NoteID | INTEGER | NO | Primary | 0 | AUTOINCREMENT | (0-18446744073709551615) RowID
VerseID | TEXT | NO | Foreign | NULL |    | (64-Character) Verse to which the note refers (Links to BibleVerse)
NoteText | TEXT | YES | NO | NULL |    | (65535-Character) Text of the note

```sqlite
CREATE TABLE BibleNote (
NoteID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
VerseID TEXT NOT NULL,
NoteText TEXT DEFAULT NULL,
FOREIGN KEY (VerseID) REFERENCES BibleVerse (VerseID)
);
```


## Song:

Songs.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
SongTitle | TEXT | NO | Primary | NULL |    | (255-Character) Title of the song
Author | TEXT | YES | NO | NULL |    | (255-Character) Author(s) of the song
Copyright | TEXT | YES | NO | NULL |    | (65535-Character) Copyright information of the song
SongText | TEXT | YES | NO | NULL |    | (4294967295-Character) Text of the song with markup
Year | TEXT | YES | NO | NULL |    | (8-Character) Year the song was published

```sqlite
CREATE TABLE Song (
SongTitle TEXT NOT NULL PRIMARY KEY,
Author TEXT DEFAULT NULL,
Copyright TEXT DEFAULT NULL,
SongText TEXT DEFAULT NULL,
Year TEXT DEFAULT NULL
);
```


## FLIndex:

Index for searching first lines of parts of a song.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
FLID | INTEGER | NO | Primary | 0 | AUTOINCREMENT| (0-18446744073709551615) RowID
SongTitle | TEXT | NO | Foreign | NULL |    | (255-Character) Links to Song
FirstLine | TEXT | NO | NO | NULL | INDEX | (255-Character) Searchable first line index as FLILineIndex

```sqlite
CREATE TABLE FLIndex (
FLID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
SongTitle VARCHAR(255) NOT NULL,
FirstLine VARCHAR(255) NOT NULL,
FOREIGN KEY (SongTitle) REFERENCES Song(SongTitle)
);

CREATE INDEX FLILineIndex ON FLIndex (FirstLine);
```

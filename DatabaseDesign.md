# Open Trebuchet Database Design

The Database is to be called "**open_trebuchet**".  Two users will have access to it: "**trebuchet_admin**" (with full access to the database) and "**trebuchet_user**" (with Insert, Update, Select and Delete).

## BibleTrans:

The Bible translations stored.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
BibID | VARCHAR(16) | NO | Primary | NULL |   | Translation identifier (E.G. WEB)
Name | VARCHAR(255) | NO | NO | NULL |    | Name of the translation (E.G. World English Bible)
Copyright | TEXT | YES | NO | NULL |    | Copyright information for the Bible translation
Year | VARCHAR(8) | YES | NO | NULL |    | Year of publication
Notes | TEXT | YES | NO | NULL |    | Any extra information the publisher wishes to store

```mysql
CREATE TABLE BibleTrans (
BibID VARCHAR(16) NOT NULL PRIMARY KEY,
Name VARCHAR(255) NOT NULL,
Copyright TEXT DEFAULT NULL,
Year VARCHAR(8) DEFAULT NULL,
Notes TEXT DEFAULT NULL
);
```


## BibleBook:

The Names of the Books.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
BookID | VARCHAR(16) | NO | Primary | NULL |    | Short name of Book (E.G. 2CHRON)
Name | VARCHAR(255) | NO | NO | NULL |    | Name of Book (E.G. 2 Chronicles)

```mysql
CREATE TABLE BibleBook (
BookID VARCHAR(16) NOT NULL PRIMARY KEY,
Name VARCHAR(255) NOT NULL
);
```


## BibleVerse:

The actual verses of the Bible.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
VerseID | VARCHAR(64) | NO | Primary | NULL |    | Format: {BibID}{BookID}HEX({ChapterNum})HEX({VerseNum})
BibID | VARCHAR(16) | NO | Foreign | NULL |    | Links to BibleTrans
BookID | VARCHAR(16) | NO | Foreign | NULL |    | Links to BibleBook
ChapterNum | UNSIGNED SMALLINT | NO | NO | 0 |    | Chapter
VerseNum | UNSIGNED SMALLINT | NO | NO | 0 |    | Verse
VerseText | TEXT | YES | NO | NULL |    | Text of the verse

```mysql
CREATE TABLE BibleVerse (
VerseID VARCHAR(64) NOT NULL PRIMARY KEY,
BibID VARCHAR(16) NOT NULL,
BookID VARCHAR(16) NOT NULL,
ChapterNum SMALLINT UNSIGNED NOT NULL DEFAULT 0,
VerseNum SMALLINT UNSIGNED NOT NULL DEFAULT 0,
VerseText TEXT DEFAULT NULL
);

ALTER TABLE BibleVerse
ADD FOREIGN KEY (BibID)
REFERENCES BibleTrans(BibID);

ALTER TABLE BibleVerse
ADD FOREIGN KEY (BookID)
REFERENCES BibleBook(BookID);
```


## BibleNote:

Some verses have translation notes.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
NoteID | UNSIGNED BIGINT | NO | Primary | 0 | AUTO_INCREMENT | 
VerseID | VARCHAR(64) | NO | Foreign | NULL |    | Verse to which the note refers (Links to BibleVerse)
NoteText | TEXT | YES | NO | NULL |    | Text of the note

```mysql
CREATE TABLE BibleNote (
NoteID BIGINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,
VerseID VARCHAR(64) NOT NULL,
NoteText TEXT DEFAULT NULL
);

ALTER TABLE BibleNote
ADD FOREIGN KEY (VerseID)
REFERENCES BibleVerse(VerseID);
```


## Song:

Songs.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
SongTitle | VARCHAR(255)| NO | Primary | NULL |    | Title of the song
Author | VARCHAR(255) | YES | NO | NULL |    | Author(s) of the song
Copyright | TEXT | YES | NO | NULL |    | Copyright information of the song
SongText | LONGTEXT | YES | NO | NULL |    | Text of the song with markup

```mysql
CREATE TABLE Song (
SongTitle VARCHAR(255) NOT NULL PRIMARY KEY,
Author VARCHAR(255) DEFAULT NULL,
Copyright TEXT DEFAULT NULL,
SongText LONGTEXT DEFAULT NULL
);
```


## FLIndex:

Index for searching first lines of parts of a song.

Field Name | Data Type | Allow Null | Key | Default | Extra | Notes
---------- | --------- | ---------- | --- | ------- | ----- | -----
FLID | UNSIGNED BIGINT | NO | Primary | 0 | AUTO_INCREMENT| 
SongTitle | VARCHAR(255) | NO | Foreign | NULL |    | Links to Song
FirstLine | VARCHAR(255) | NO | NO | NULL | INDEX | Searchable first line index FLILineIndex

```mysql
CREATE TABLE FLIndex (
FLID BIGINT UNSIGNED NOT NULL PRIMARY KEY AUTOINCREMENT,
SongTitle VARCHAR(255) NOT NULL,
FirstLine VARCHAR(255) NOT NULL
);

ALTER TABLE FLIndex
ADD FOREIGN KEY (SongTitle)
REFERENCES Song(SongTitle);

CREATE INDEX FLILineIndex
ON FLIndex (FirstLine);
```

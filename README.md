# Open-Trebuchet
Open-source church projection software for Windows.

## Files

### Documentation

* "README.md" is this file.
* "DatabaseDesign.md" contains the design of the database.
* "DatabaseDesignForMySQL.md" contains the old design of the database from when it was going to be implemented in MySQL.
* "MySQLVsSQLite.md" contains the reasons for the switch to SQLite.
* "TrebuchetMarkup.md" contains the design for the markup language to be used to format songs.

### Source Code

* "dbfuncs.c" is where the functions are for preparing data to be exchanged with the database.
* "dbfuncs.h" is the header file for "dbfuncs.c".
* "dbinsert.c" is where the functions are for inserting data from the database into the various listboxes etc in the main program.
* "dbinsert.h" is the header file for "dbinsert.c".
* "errmsgs.c" contains the routines for producing and defining error messages.
* "errmsgs.h" contains definitions of some error messages and is the header file for "errormsgs.c".
* "langspec.h" is where you specify what (human) language you want the finished program to use.  Edit this file if your target is in a non-English speaking region.  If you are developing for the project, do not #include this file!  Use "langdef.h" instead.
* "langdef.h" is where the languages are defined.
* "main.c" is the main body of code.  It's where WinMain is, along with the functions for running the windows.
* "main.h" is the header file for "main.c".
* "Makefile" is the Makefile for the project.
* "menus.rc" is the resource file where the menu system for the windows is defined.
* "proginfo.rc" is the resource file where the program information is defined.
* "resource.h" holds the resource definitions for the RC files and the controls.
* "safetyfuncs.c" is essentially a bunch of functions that replace the "strsafe.h" functions needed by the program.  "strsafe.h" is not available for MinGW so this file is needed.
* "safetyfuncs.h" is the header file for "safetyfuncs.c".
* "scrnfuns.c" is where the functions for managing the Screen Window are held.
* "scrnfuns.h" is the header file for "scrnfuns.c".

### Final files

* The "otdb.db" file contains the database in SQLite3 format.

### Subfolders

* The "Database Backups" folder contains various backups of the database.
* The "RdBF" folder contains the program for adding the contents of the Bible Files from Trebuchet 3.0 to the database.
* The "WEB" folder within the "RdBF" folder contains the Trebuchet 3.0 Bible Files for the World English Bible translation.

---

## Background
Many years ago, I was playing about in Delphi and created some simple projection software.  I called it "_Trebuchet_", because it was for "_flinging stuff at the screen_".  It was rubbish - it used the filesystem to store data and was 16-bit, but it was a good start.  Later I thought a bit and built a far better version that used a database backend.  It never got finished but I christened it "_Trebuchet 2_" and moved onto other projects.  Many years later, in 2013-ish, I visited my Uncle's church, where they didn't have any projection software and decided to resurrect the idea for the project in "_Trebuchet 3_" and give it to them free.  Trebuchet 3 was built in VB6 with a Microsoft Access backend and while I made a good deal of progress on the project I got sidetracked by other things and never quite completed it.  Now however, I've decided to do things properly...

Open Trebuchet will essentially be "_Trebuchet 4_".  It will be fully Open-Source, 32-bit for maximum compatibility, written in C and designed for Windows Vista onwards (I'd love to include XP users but most of the useful bits of the Windows API are Vista minimum) - crucially, it must run on Windows 10.  The database backend was originally going to be MySQL (to ensure both the open-source nature and the compatibility with all (recent) Windows versions), however owing to licensing difficulties and programming incompatibilities, SQLite will be used instead.  SQLite is public-domain (ensuring Open Trebuchet can be any form of open-source licence appropriate to the project) and still runs on most Windows variations, so there should be no conflicts with the original vision.

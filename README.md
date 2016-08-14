# Open-Trebuchet
Open-source church projection software for Windows.

---

## Background
Many years ago, I was playing about in Delphi and created some simple projection software.  I called it "_Trebuchet_", because it was for "_flinging stuff at the screen_".  It was rubbish - it used the filesystem to store data and was 16-bit, but it was a good start.  Later I thought a bit and built a far better version that used a database backend.  It never got finished but I christened it "_Trebuchet 2_" and moved onto other projects.  Many years later, in 2013-ish, I visited my Uncle's church, where they didn't have any projection software and decided to resurrect the idea for the project in "_Trebuchet 3_" and give it to them free.  Trebuchet 3 was built in VB6 with a Microsoft Access backend and while I made a good deal of progress on the project I got sidetracked by other things and never quite completed it.  Now however, I've decided to do things properly...

Open Trebuchet will essentially be "_Trebuchet 4_".  It will be fully Open-Source, 32-bit for maximum compatibility, written in C and designed for Windows Vista onwards (I'd love to include XP users but most of the useful bits of the Windows API are Vista minimum) - crucially, it must run on Windows 10.  The database backend was originally going to be MySQL (to ensure both the open-source nature and the compatibility with all (recent) Windows versions), however owing to licensing difficulties and programming incompatibilities, SQLite will be used instead.  SQLite is public-domain (ensuring Open Trebuchet can be any form of open-source licence appropriate to the project) and still runs on most Windows variations, so there should be no conflicts with the original vision.

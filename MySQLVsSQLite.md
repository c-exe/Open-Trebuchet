# MySQL Vs SQLite

The Open Trebuchet project was originally going to use MySQL as its database backend, however I have decided to move to SQLite.  This document seeks to explain the reasons for the decision.

## Advantages of MySQL over SQLite

This was not an easy decision to make, by any means, so let's start with what MySQL has going for it:

* MySQL is a Client/Server RDBMS, so the database could be stored elsewhere.  This could have been useful for combined parishes to have a single database for all their songs, without having to copy files around all the time (and protecting against the Lost Update Problem).
* MySQL is strongly typed, so text fields are efficiently managed.
* MySQL automatically sizes the database without need of a command to clean up or defragmant the database.
* MySQL databases can be spread over multiple hard-drives, reducing the strain on each.
* MySQL can store massive amounts of data, so there would be no limit on songs and even images could be stored in the database.  (It is worth noting on this last point, however, that while SQLite databases are limited to 128TB each, this is still substantially larger than most computer hard-drives.  Indeed, if your church is generating this much data, you can probably afford to buy something like MediaShout instead of relying on an open-source projection program).

## Advantages of SQLite over MySQL

Now we come to the reasons for choosing SQLite:

* SQLite is integrated into the program, so separate installation is not necessary.
* SQLite is entirely Public Domain and Licence Free, meaning Open Trebuchet can take on whichever open-source licence seems appropriate for its use.
* SQLite easily integrates into a large number of languages, including (crucially) C with any compiler (MySQL is limited to gcc on Linux and Visual C++ on Windows - you can't even use MinGW or CygWin to compile using gcc on Windows with it!).
* SQLite will run on basically any platform, from standard desktop systems (most versions of Windows, Linux and Mac OS/X), through mobile platforms (Android, iOS and SymbianOS) to even legacy systems like DOS!  This should ensure that it will continue to be appropriate for Open Trebuchet wherever the project heads in the future.

## Conclusion

While there appear to be more individual points in favour of MySQL above, the final point in its list is moot for most users, leaving a pretty even list.  In the end, though, while most of the MySQL advantages are merely "nice-to-have", the ability of SQLite to work with any C compiler and any licensing system is critical to the project.  Thus, I have concluded that Open Trebuchet must use SQLite as a back-end database.

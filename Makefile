# Makefile for Open Trebuchet

CC = gcc
WINDRES = windres.exe
ALLOBJS = main.o safetyfuncs.o dbfuncs.o proginfores.o menusres.o
LIBSPECS = -luser32 -lkernel32

ot4.exe: ${ALLOBJS}
	${CC} ${ALLOBJS} ${LIBSPECS} -o ot4.exe

main.o: main.c main.h resource.h langdef.h
	${CC} -c main.c

safetyfuncs.o: safetyfuncs.c safetyfuncs.h
	${CC} -c safetyfuncs.c

dbfuncs.o: dbfuncs.c dbfuncs.h safetyfuncs.h
	${CC} -c dbfuncs.c

proginfores.o: proginfo.rc resource.h langdef.h
	${WINDRES} -O coff proginfo.rc proginfores.o

menusres.o: menus.rc resource.h langdef.h
	${WINDRES} -O coff menus.rc menusres.o
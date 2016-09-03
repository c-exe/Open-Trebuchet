# Makefile for Open Trebuchet

CC = gcc
WINDRES = windres.exe
ALLOBJS = safetyfuncs.o dbfuncs.o proginfores.o menusres.o

ot4.exe: ${ALLOBJS}
	${CC} ${ALLOBJS} -luser32 -lkernel32 -o ot4.exe

safetyfuncs.o: safetyfuncs.c safetyfuncs.h
	${CC} -c safetyfuncs.c

dbfuncs.o: dbfuncs.c dbfuncs.h safetyfuncs.h
	${CC} -c dbfuncs.c

proginfores.o: proginfo.rc resource.h langdef.h
	${WINDRES} -O coff proginfo.rc proginfores.o

menusres.o: menus.rc resource.h langdef.h
	${WINDRES} -O coff menus.rc menusres.o
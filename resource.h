/* Resources Header */

#ifndef __RESOURCE_H__

#define __RESOURCE_H__ 1

#include "langdef.h"
#ifndef INFOLANG
#define INFOLANG _LANG_ENGLISH
#endif

#ifndef VS_VERSION_INFO
#define VS_VERSION_INFO 1
#endif
#ifndef VS_FF_DEBUG
#define VS_FF_DEBUG 0x1L
#endif
#ifndef VS_VERSION_INFO
#define VS_VERSION_INFO 0x1L
#endif
#ifndef VS_FFI_FILEFLAGSMASK
#define VS_FFI_FILEFLAGSMASK 0x17L
#endif
#ifndef VER_PRIVATEBUILD
#define VER_PRIVATEBUILD 0x0L
#endif
#ifndef VER_PRERELEASE
#define VER_PRERELEASE 0x0L
#endif
#ifndef VOS__WINDOWS32
#define VOS__WINDOWS32 0x4L
#endif
#ifndef VOS_NT_WINDOWS32
#define VOS_NT_WINDOWS32 0x00040004L
#endif
#ifndef VFT_DLL
#define VFT_DLL 0x2L
#endif
#ifndef VFT2_UNKNOWN
#define VFT2_UNKNOWN 0x0L
#endif

#define VER_FILEVERSION 1,0,1,0
#define VER_FILEVERSION_STR "1.00.01\0"
#define VER_PRODUCTVERSION 4,0,0,1
#define VER_PRODUCTVERSION_STR "4.00.00 ALPHA\0"
#if INFOLANG == _LANG_ENGLISH
#define VER_COMPANYNAME_STR "The Open Trebuchet Project\0"
#define VER_PRODUCTNAME_STR "Open Trebuchet\0"
#define VER_FILEDESCRIPTION_STR "Open Trebuchet - Open-source Windows projection software.\0"
#define VER_INTERNALNAME_STR "ot4\0"
#define VER_ORIGINALFILENAME_STR "ot4.exe\0"
#define VER_LEGALCOPYRIGHT_STR "2016 The Open Trebuchet Project\0"
#elif INFOLANG == _LANG_FRANCAIS
#define VER_COMPANYNAME_STR "Le projet Open Trebuchet\0"
#define VER_PRODUCTNAME_STR "Open Trebuchet\0"
#define VER_FILEDESCRIPTION_STR "Open Trebuchet - Open-source du logiciel de projection de Windows.\0"
#define VER_INTERNALNAME_STR "ot4\0"
#define VER_ORIGINALFILENAME_STR "ot4\0"
#define VER_LEGALCOPYRIGHT_STR "2016 Le projet Open Trebuchet\0"
#elif INFOLANG == _LANG_CYMRAIG
#define VER_COMPANYNAME_STR "Mae'r Prosiect Open Trebuchet\0"
#define VER_PRODUCTNAME_STR "Open Trebuchet\0"
#define VER_FILEDESCRIPTION_STR "Open Trebuchet - Meddalwedd taflunio ffynhonnell agored ar gyfer Windows.\0"
#define VER_INTERNALNAME_STR "ot4\0"
#define VER_ORIGINALFILENAME_STR "ot4\0"
#define VER_LEGALCOPYRIGHT_STR "2016 Mae'r Prosiect Open Trebuchet\0"
#endif


#define IDR_MAINMENU 101

#define ID_FILE_EXIT 9001
#define ID_SHOW_SCREEN 9002
#define ID_SHOW_PROJECT 9003
#define ID_SHOW_BLACK 9004
#define ID_SHOW_CLEAR 9005
#define ID_SHOW_SETTINGS 9006
#define ID_SONG_NEW 9007
#define ID_SONG_EDITDB 9008
#define ID_SONG_EDITSCHED 9009


#endif

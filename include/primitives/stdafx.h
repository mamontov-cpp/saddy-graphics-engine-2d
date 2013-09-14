// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

//#define __VERSION__ "0.01r52"

//Determines linkage,waygrid,other checking fuctions are being used
//#define LINKAGETEST
//#define WAYGRIDTEST
//#define ITYPETEST

#ifdef _CRTMEMORYTEST
	  #include <windows.h>
      #include <crtdbg.h>
      #define _CRT_INIT_MEMTEST  HANDLE hTLogFile=CreateFileA("dump.txt",GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);_CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_FILE);_CrtSetReportFile(_CRT_WARN,hTLogFile);
      #define _CRT_END_MEMTEST   _CrtDumpMemoryLeaks();CloseHandle(hTLogFile);
      #define CKPERFTEST( X )    printf(X,GetTickCount()-ck);ck=GetTickCount();
      #define CKINITPERFTEST     int  ck=GetTickCount();int  ckfirst=ck;
      #define CKTOTALELAPSED     printf("Current time elapsed :%ld\n",GetTickCount()-ckfirst);
#else
	 #define _CRT_INIT_MEMTEST
	 #define _CRT_END_MEMTEST
     #define CKPERFTEST( X )
	 #define CKINITPERFTEST
	 #define CKTOTALELAPSED
#endif

//#include "targetver.h"

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_NO_WARNINGS 1

#define H_USE_LOW_LEVEL_IO          //HINT: use low level	IO Functions for hString
#ifndef __WIN32
    #define __WIN32                 //HINT: use win32 hcio
#ifndef _MSC_VER
    #define __UBUNTU				//HINT: use <unistd.h> instead of <io.h>
#endif
#endif
//#define __HAS_A_ITOA_CONVERSION     //HINT: define if compiler supports itoa()

#include <stdio.h>
#ifdef _MSC_VER
	#include <tchar.h>
	#include <conio.h>
#else
	#define _tmain main
	#define _TCHAR char
	#define _getch
	#define _getcwd getcwd
	#define _putenv putenv
    #define _spawnl spawnl
#ifdef __UBUNTU
	#define _read read
	#define _close close
    #define _write write
	#define _open open
	#define _O_RDONLY O_RDONLY
	#define _O_WRONLY O_WRONLY
	#define _O_TRUNC  O_TRUNC
	#define _O_TEXT   1
	#define _O_CREAT  O_CREAT
	#define _S_IREAD  S_IREAD
	#define _S_IWRITE S_IWRITE
#endif
#endif

/*
* libtcod 1.5.1
* Copyright (c) 2008,2009,2010,2012 Jice & Mingos
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice or Mingos may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE AND MINGOS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE OR MINGOS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <string.h>

#if defined (__linux) && !defined (__ANDROID__) || defined (__FreeBSD__)
/* X11 stuff for clipboard support */
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#elif defined (__APPLE__) && defined (__MACH__)
#include <ApplicationServices/ApplicationServices.h>
#endif

#include "Doryen/libtcod_int.h"

#ifdef TCOD_WINDOWS
#include <windows.h>
#else

#include <dirent.h>
#endif

#if defined(TCOD_WINDOWS)
char *strcasestr (const char *haystack, const char *needle) {
	const char *p, *startn = 0, *np = 0;

	for (p = haystack; *p; p++) {
		if (np) {
			if (toupper(*p) == toupper(*np)) {
				if (!*++np)
					return (char *)startn;
			} else
				np = 0;
		} else if (toupper(*p) == toupper(*needle)) {
			np = needle + 1;
			startn = p;
		}
	}

	return 0;
}
#endif


static bool filename_match(const char *name, const char *pattern) {
	char *ptr;
	if ( pattern == NULL || pattern[0] == 0 ) return true;
	ptr=strchr(pattern,'*');
	if ( ! ptr ) return strcmp(name,pattern) == 0;
	if ( ptr != name && strncmp(name,pattern, ptr - pattern) != 0 ) return false;
	return  strcmp( name + strlen(name) - strlen(ptr+1), ptr+1) == 0;
}

TCOD_list_t TCOD_sys_get_directory_content(const char *path, const char *pattern) {
    TCOD_list_t list=TCOD_list_new();
#ifdef TCOD_WINDOWS
    WIN32_FIND_DATA FileData;
    HANDLE          hList;
	char dname[ 512 ];
	sprintf(dname, "%s\\*",path);
    hList = FindFirstFile(dname, &FileData);
    if (hList == INVALID_HANDLE_VALUE)
    {
        return list;
    }
	do
	{
		if ( ! (strcmp(FileData.cFileName,".") == 0 || strcmp(FileData.cFileName,"..") == 0 ) )
		{
			if ( filename_match(FileData.cFileName,pattern) )
				TCOD_list_push(list,TCOD_strdup(FileData.cFileName));
		}

	} while ( FindNextFile(hList, &FileData) );
    FindClose(hList);
#else
    DIR *dir = opendir(path);
    struct dirent *dirent = NULL;
    if ( ! dir ) return list;
    while ( ( dirent = readdir(dir) ) )
    {
		if ( ! (strcmp(dirent->d_name,".") == 0 || strcmp(dirent->d_name,"..") == 0 ) )
		{
			if ( filename_match(dirent->d_name,pattern) )
				TCOD_list_push(list,strdup(dirent->d_name));
		}
	}
	closedir(dir);
#endif
	return list;
}

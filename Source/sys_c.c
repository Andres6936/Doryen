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

/* thread stuff */
#ifdef TCOD_WINDOWS
/* Helper function to count set bits in the processor mask. */
static DWORD CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR)*8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
    DWORD i;

    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest)?1:0);
        bitTest/=2;
    }

    return bitSetCount;
}
#endif

#ifdef TCOD_WINDOWS
/* poor win32 api has no thread conditions */
typedef struct {
	int nbSignals;
	int nbWaiting;
	TCOD_mutex_t mutex;
	TCOD_semaphore_t waiting;
	TCOD_semaphore_t waitDone;
} cond_t;
#endif

/*clipboard stuff */
#ifdef TCOD_WINDOWS
void TCOD_sys_clipboard_set(const char *value)
{
    HGLOBAL clipbuffer;
    char * buffer;
	if (!OpenClipboard(0) || ! value) return;
    EmptyClipboard();
    clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(value)+1);
    buffer = (char*)GlobalLock(clipbuffer);
	strcpy(buffer, value);
    GlobalUnlock(clipbuffer);
    SetClipboardData(CF_TEXT, clipbuffer);
    CloseClipboard();
}

char *TCOD_sys_clipboard_get()
{
    char * buffer = NULL;
    HANDLE hData;
	if (!OpenClipboard(NULL)) return 0;
    hData = GetClipboardData( CF_TEXT );
    buffer = (char*)GlobalLock( hData );
    GlobalUnlock( hData );
    CloseClipboard();
    return buffer;
}
#elif defined(TCOD_MACOSX)
void TCOD_sys_clipboard_set(const char *value)
{
	PasteboardRef clipboard;
  if (PasteboardCreate(kPasteboardClipboard, &clipboard) != noErr) return;
  if (PasteboardClear(clipboard) != noErr) {
      CFRelease(clipboard);
      return;
  }
	size_t len = strlen(value);
  CFDataRef data = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault,
																					(const UInt8 *)value,
                                          len, kCFAllocatorNull);
  if (data == NULL) {
      CFRelease(clipboard);
      return;
  }
  OSStatus err;
  err = PasteboardPutItemFlavor(clipboard, NULL, kUTTypePlainText, data, 0);
  CFRelease(clipboard);
  CFRelease(data);
}

char clipboardText[256];

char *TCOD_sys_clipboard_get()
{
	PasteboardSyncFlags syncFlags;
	ItemCount itemCount;
	PasteboardRef clipboard;
	UInt32 itemIndex;
	if (PasteboardCreate(kPasteboardClipboard, &clipboard) != noErr) return NULL;
	syncFlags = PasteboardSynchronize(clipboard);
	if (PasteboardGetItemCount(clipboard, &itemCount) != noErr) return NULL;
	if (itemCount == 0) return NULL;
	for(itemIndex = 1; itemIndex <= itemCount; itemIndex++) {
		PasteboardItemID itemID;
		CFArrayRef flavorTypeArray;
		CFIndex flavorCount;
		CFIndex flavorIndex;
		if (PasteboardGetItemIdentifier(clipboard, itemIndex, &itemID ) != noErr) return NULL;
		if (PasteboardCopyItemFlavors(clipboard, itemID, &flavorTypeArray) != noErr) return NULL;
		flavorCount = CFArrayGetCount(flavorTypeArray);
		for(flavorIndex = 0; flavorIndex < flavorCount; flavorIndex++) {
        CFStringRef flavorType;
        CFDataRef flavorData;
        CFIndex flavorDataSize;
				flavorType = (CFStringRef)CFArrayGetValueAtIndex(flavorTypeArray, flavorIndex);
				if (UTTypeConformsTo(flavorType, CFSTR("public.plain-text"))) {
						if (PasteboardCopyItemFlavorData(clipboard, itemID, flavorType, &flavorData) != noErr) {
							CFRelease(flavorData);
							return NULL;
						}
						flavorDataSize = CFDataGetLength( flavorData );
						flavorDataSize = (flavorDataSize<254) ? flavorDataSize : 254;
						short dataIndex;
	          for(dataIndex = 0; dataIndex <= flavorDataSize; dataIndex++) {
	             clipboardText[dataIndex] = *(CFDataGetBytePtr(flavorData) + dataIndex);
	          }
	          clipboardText[flavorDataSize] = '\0';
	          clipboardText[flavorDataSize+1] = '\n';
						CFRelease (flavorData);
	     }
		}
	}
	return clipboardText;
}
#elif defined(TCOD_HAIKU) || defined(__ANDROID__)
/* TODO */
void TCOD_sys_clipboard_set(const char *value)
{
}
char *TCOD_sys_clipboard_get()
{
	return "";
}
#else
static Display *dpy=NULL;
void TCOD_sys_clipboard_set(const char *value)
{
	if ( ! value ) return;
	if (!dpy ) dpy = XOpenDisplay(NULL);
	XStoreBytes(dpy,value,strlen(value)+1);
	/* doesn't seem to work without this... */
	int len;
	char *xbuf = XFetchBytes(dpy,&len);
	XFree(xbuf);
}

char *TCOD_sys_clipboard_get()
{
	int len;
	if (!dpy ) dpy = XOpenDisplay(NULL);
	char *xbuf = XFetchBytes(dpy,&len);
	if (! xbuf ) return NULL;
	char *ret=strdup(xbuf);
	XFree(xbuf);
	return ret;
}
#endif


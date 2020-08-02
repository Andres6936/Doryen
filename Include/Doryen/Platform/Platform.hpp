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

#ifndef _TCOD_SYS_HPP
#define _TCOD_SYS_HPP

#include "Doryen/libtcod.h"

namespace Doryen
{

    /**
     * This toolkit contains some system specific miscellaneous utilities.
     *
     * @note Use them is you want your code to be easily portable.
     */
    class Platform
	{

	public :

		/**
		 * Returns the number of seconds since the program has started.
		 *
		 * @return number of seconds since the program has started.
		 */
		static float getElapsedSeconds();

		/**
        @PageName system_screenshots
        @PageFather system
        @PageTitle Easy screenshots
        @FuncDesc This function allows you to save the current game screen in a png file, or possibly a bmp file if you provide a filename ending with .bmp.
        @Cpp static void TCODSystem::saveScreenshot(const char *filename)
        @C void TCOD_sys_save_screenshot(const char *filename)
        @Py sys_save_screenshot(filename)
        @C# static void TCODSystem::saveScreenshot(string filename);
        @Lua tcod.system.saveScreenshot(filename)
        @Param filename Name of the file. If NULL, a filename is automatically generated with the form "./screenshotNNN.png", NNN being the first free number (if a file named screenshot000.png already exist, screenshot001.png will be used, and so on...).
        */
		static void saveScreenshot(const char* filename);

		/**
		@PageName system_filesystem
		@FuncTitle List files in a directory
		@FuncDesc To get the list of entries in a directory (including sub-directories, except . and ..).
			The returned list is allocated by the function and must be deleted by you. All the const char * inside must be also freed with TCODList::clearAndDelete.
		@Cpp static TCODList TCODSystem::getDirectoryContent(const char *path, const char *pattern)
		@C TCOD_list_t TCOD_sys_get_directory_content(const char *path)
		@Param path a directory
		@Param pattern If NULL or empty, returns all directory entries. Else returns only entries matching the pattern. The pattern is NOT a regular expression. It can only handle one '*' wildcard. Examples : *.png, saveGame*, font*.png
		*/
		static TCOD_list_t getDirectoryContent(const char* path, const char* pattern);

	};
}

#endif

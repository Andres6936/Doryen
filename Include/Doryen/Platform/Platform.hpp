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
#include "Doryen/Doryen.hpp"

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
         * @brief  Allows you to limit the number of frames per second.
         *
         * If a frame is rendered faster than expected, the TCOD_console_flush
         * function will wait so that the frame rate never exceed this value.
         *
         * @note You can call this function during your game initialization.
         * @note You can dynamically change the frame rate. Just call this function
         * once again.
         * @note You should always limit the frame rate, except during benchmarks,
         * else your game will use 100% of the CPU power
         *
         * @param val Maximum number of frames per second. 0 means unlimited frame rate.
         */
        static void setFps( int val );

        /**
         * @brief Get the number of frames rendered during the last second.
         *
         * @note The value returned by this function is updated every second.
         *
         * @return Number of frames rendered during the las second.
         */
        static int getFps( );

        /**
         * @brief Get the duration of the last frame.
         *
         * This function returns the length in seconds of the last rendered frame.
         *
         * @note You can use this value to update every time dependent object in the world.
         *
         * @return
         */
		static float getLastFrameLength();

		/**
		 * @brief Get global timer in milliseconds.
		 *
		 * Returns the number of milliseconds since the program has started.
		 *
		 * @return number of milliseconds since the program has started.
		 */
		static uint32 getElapsedMilli();

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

		/**
		@PageName system_sdlcbk
		@FuncTitle Managing screen redraw
		@FuncDesc libtcod is not aware of the part of the screen your SDL renderer has updated. If no change occured in the console, it won't redraw them except if you tell him to do so with this function
		@Cpp void TCODConsole::setDirty(int x, int y, int w, int h)
		@C void TCOD_console_set_dirty(int x, int y, int w, int h)
		@Py TCOD_console_set_dirty(x, y, w, h)
		@Param x,y,w,h Part of the root console you want to redraw even if nothing has changed in the console back/fore/char.
		*/

        /**
        @PageName system_misc
        @PageFather system
        @PageTitle Miscellaneous utilities
        @FuncTitle Using a custom resolution for the fullscreen mode
        @FuncDesc This function allows you to force the use of a specific resolution in fullscreen mode.
            The default resolution depends on the root console size and the font character size.
        @Cpp static void TCODSystem::forceFullscreenResolution(int width, int height)
        @C void TCOD_sys_force_fullscreen_resolution(int width, int height)
        @Py sys_force_fullscreen_resolution(width, height)
        @C# static void TCODSystem::forceFullscreenResolution(int width, int height);
        @Lua tcod.system.forceFullscreenResolution(width,height)
        @Param width,height Resolution to use when switching to fullscreen.
            Will use the smallest available resolution so that :
            resolution width >= width and resolution width >= root console width * font char width
            resolution width >= height and resolution height >= root console height * font char height
        @CppEx
            TCODSystem::forceFullscreenResolution(800,600); // use 800x600 in fullscreen instead of 640x400
            TCODConsole::initRoot(80,50,"",true); // 80x50 console with 8x8 char => 640x400 default resolution
        @CEx
            TCOD_sys_force_fullscreen_resolution(800,600);
            TCOD_console_init_root(80,50,"",true);
        @PyEx
            libtcod.sys_force_fullscreen_resolution(800,600)
            libtcod.console_init_root(80,50,"",True)
        @LuaEx
            tcod.system.forceFullscreenResolution(800,600) -- use 800x600 in fullscreen instead of 640x400
            tcod.console.initRoot(80,50,"",true) -- 80x50 console with 8x8 char => 640x400 default resolution
        */
		static void forceFullscreenResolution(int width, int height);

		/**
		@PageName system_misc
		@FuncTitle Get the font size
		@FuncDesc You can get the size of the characters in the font
		@Cpp static void TCODSystem::getCharSize(int *width, int *height)
		@C void TCOD_sys_get_char_size(int *width, int *height)
		@Py sys_get_char_size() # returns w,h
		@C# static void TCODSystem::getCharSize(out int w, out int h);
		@Param width,height contains a character size when the function returns
		*/
        static void getCharSize( int *w, int *h );

        /**
        @PageName system_misc
        @FuncTitle Dynamically updating the font bitmap
        @FuncDesc You can dynamically change the bitmap of a character in the font. All cells using this ascii code will be updated at next flush call.
        @Cpp static void TCODSystem::updateChar(int asciiCode, int fontx, int fonty,const TCODImage *img,int x,int y)
        @C void TCOD_sys_update_char(int asciiCode, int fontx, int fonty, TCOD_image_t img, int x, int y)
        @Py sys_update_char(asciiCode,fontx,fonty,img,x,y)
        @Param asciiCode ascii code corresponding to the character to update
        @Param fontx,fonty coordinate of the character in the bitmap font (in characters, not pixels)
        @Param img image containing the new character bitmap
        @Param x,y position in pixels of the top-left corner of the character in the image
        */
		static void updateChar(int asciiCode, int fontx, int fonty, const Image* img, int x, int y);

        /**
        @PageName system_misc
        @FuncTitle Dynamically change libtcod's internal renderer
        @FuncDesc As of 1.5.1, libtcod contains 3 different renderers :
            * SDL : historic libtcod renderer. Should work and be pretty fast everywhere
            * OpenGL : requires OpenGL compatible video card. Might be much faster or much slower than SDL, depending on the drivers
            * GLSDL : requires OpenGL 1.4 compatible video card with GL_ARB_shader_objects extension. Blazing fast if you have the proper hardware and drivers.
            This function switches the current renderer dynamically.
        @Cpp static void TCODSystem::setRenderer(TCOD_renderer_t renderer)
        @C void TCOD_sys_set_renderer(TCOD_renderer_t renderer)
        @Py sys_set_renderer(renderer)
        @C# static void TCODSystem::setRenderer(TCODRendererType renderer);
        @Param renderer Either TCOD_RENDERER_GLSL, TCOD_RENDERER_OPENGL or TCOD_RENDERER_SDL
        */
        static void setRenderer( TCOD_renderer_t renderer );

		/**
		@PageName system_misc
		@FuncTitle Get the current internal renderer
		@Cpp static TCOD_renderer_t TCODSystem::getRenderer()
		@C TCOD_renderer_t TCOD_sys_get_renderer()
		@Py sys_get_renderer()
		@C# static TCODRendererType TCODSystem::getRenderer();
		*/
		static TCOD_renderer_t getRenderer();

	};
}

#endif

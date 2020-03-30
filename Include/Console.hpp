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

#ifndef _TCOD_CONSOLE_HPP
#define _TCOD_CONSOLE_HPP

#include <string>
#include <vector>
#include <array>

#include "Renderer/Renderer.hpp"
#include "Renderer/Sdl.hpp"
#include "Color.hpp"

#include "libtcod.h"
#include "console_types.h"

namespace Doryen
{
	/**
	 * The console emulator handles the rendering of the game screen and the
	 * keyboard input.
	 *
	 * The instaces of Console class are called offscreen consoles, this you
	 * allow draw on secondary consoles as you would do with the
	 * root console. You can then blit those secondary consoles on the root
	 * console. This allows you to use local coordinate space while rendering
	 * a portion of the final screen, and easily move components of the screen
	 * without modifying the rendering functions.
	 *
	 * You can create as many off-screen consoles as you want by using
	 * instances. You can draw on them as you would do with the root console,
	 * but you cannot flush them to the screen. Else, you can blit them on other
	 * consoles, including the root console.
	 */
	class Console
	{

	private:

		static Renderer* renderer;

		bool windowClose = false;

		/**
		 * Only exist a root console during all execute of program.
		 */
		bool isConsoleRoot = false;

		/**
		 * Console width (in characters,not pixels).
		 */
		unsigned width = 0;

		/**
		 *  Console heigth (in characters,not pixels).
		 */
		unsigned height = 0;

		/**
		 * Foreground text.
		 */
		Color foreground = Color::white;

		/**
		 * Background text.
		 */
		Color background = Color::black;

		Color controlBackground[TCOD_COLCTRL_NUMBER];

		Color controlForeground[TCOD_COLCTRL_NUMBER];

		/**
		 * Current console.
		 */
		std::vector <Char> buffer;

		/**
		 * Console for last frame.
		 */
		std::vector <Char> oldBuffer;

	public :

		TCOD_console_t data;

		static Console* root;

		explicit Console(TCOD_console_t con) : data(con)
		{
		}

		/**
		 * Create an offscreen console.
		 *
		 * @note The size of offscreen console is 80x25 default.
		 */
		Console();


		/**
		 * Create an offscreen console.
		 *
		 * @param w width of console. w > 0.
		 * @param h height of console. h > 0.
		 */
        Console( int w, int h );

        /**
         * Create an offscreen console from a .asc or .apf file.
         *
         * You can create an offscreen console from a file
         * created with Dorpy with this constructor.
         *
         * @param filename path to the .asc or .apf file created with Dorpy.
         */
        Console( const char *filename );

        /**
         * Destroy an offscreen console and release any resources allocated.
         *
         * @note Don't use it on the root console.
         */
        virtual ~Console( );

        /**
         * Creating the game window.
         *
         * The default font in libtcod (./Terminal.png) uses 8x8 pixels characters.
         *
         * You can change the font by calling TCODConsole::setCustomFont before
         * calling initRoot.
         *
         * @param w width size of the console (in characters).
         * @param h height size of the console (in characters).
         * @param title title of the window. It's not visible when you are in fullscreen.
         * @param _fullscreen wether you start in windowed or fullscreen mode.
         * @param _renderer renderer to use. Possible values are :
         *
         *  1- TCOD_RENDERER_GLSL : works only on video cards with pixel shaders. <br>
         *  2- TCOD_RENDERER_OPENGL : works on all video cards supporting OpenGL 1.4 <br>
         *  3- TCOD_RENDERER_SDL : should work everywhere!. <br>
         *
         * @note If you select a renderer that is not supported by the player's machine,
         * libtcod scan the lower renderers until it finds a working one.
         *
         * @note On recent video cards, GLSL results in up to 900% increase of
         * framerates in the true color sample compared to SDL renderer.
         *
         * @note Whatever renderer you use, it can always be overriden by the player
         * through the libtcod.cfg file.
         */
		void
		initRoot(int w, int h, const char* title, bool _fullscreen = false,
				TCOD_renderer_t _renderer = TCOD_RENDERER_SDL);

		/**
		 * This function allows you to use a bitmap font (png or bmp) with custom
		 * character size or layout.
		 *
		 * It should be called before initializing the root console with initRoot.
		 * Once this function is called, you can define your own custom mappings
		 * using mapping functions.
		 *
		 * @param fontFile Name of a .bmp or .png file containing the font.
		 * @param flags Used to define the characters layout in the bitmap and the font type :
		 *
		 * 1- TCOD_FONT_LAYOUT_ASCII_INCOL : characters in ASCII order, code 0-15 in the first column. <br>
		 * 2- TCOD_FONT_LAYOUT_ASCII_INROW : characters in ASCII order, code 0-15 in the first row <br>
		 * 3- TCOD_FONT_LAYOUT_TCOD : simplified layout. <br>
		 * 4- TCOD_FONT_TYPE_GREYSCALE : create an anti-aliased font from a greyscale bitmap. <br>
		 *
		 * @param nbCharHoriz Number of characters in the font.
		 * @param nbCharVertic Number of characters in the font.
		 *
		 * @note If set to 0 nbCharHoriz or nbCharVertic, there are deduced from
		 * the font layout flag.
		 */
		void setCustomFont(const char* fontFile, int flags = TCOD_FONT_LAYOUT_ASCII_INCOL, int nbCharHoriz = 0,
				int nbCharVertic = 0);

        /**
         * @return returns true if the current mode is fullscreen.
         */
        static bool isFullscreen( );

        /**
         * Switches the root console to fullscreen or windowed mode.
         *
         * @note Note that there is no predefined key combination to
         * switch to/from fullscreen. You have to do this in your own code.
         *
         * @param fullscreen true to switch to fullscreen mode, false to
         * switch to windowed mode.
         */
		static void setWindowInFullscreen();

		/**
		 * Dynamically changes the title of the game window.
		 *
		 * @note Note that the window title is not visible while in fullscreen.
		 *
		 * @param _title New title of the game window.
		 */
		static void setWindowTitle(const std::string& _title);

        /**
         * When you start the program, this returns false. Once a "close window"
         * event has been sent by the window manager, it will allways return
         * true. You're supposed to exit cleanly the game.
         *
         * @return true if the user want exit the app, false otherwise.
         */
        bool isWindowClosed( );

        /**
         * Display credits, as seen in the samples.
         *
         * You can print a "Powered by libtcod x.y.z" screen during your game
         * startup simply by calling this function after initRoot.
         *
         * @note The credits screen can be skipped by pressing any key.
         */
        static void credits( );

        /**
         * You can also print the credits on one of your game screens (your
         * main menu for example) by calling this function in your main loop.
         *
         * This function returns true when the credits screen is finished,
         * indicating that you no longer need to call it.
         *
         * @param x coordiante in x of the credits text in your root console.
         * @param y coordiante in y of the credits text in your root console.
         * @param alpha if true, credits are transparently added on top of
         * the existing screen.
         *
         * @note For this to work, this function must be placed between
         * your screen rendering code and the console flush.
         *
         * @return true when the credits screen is finished, false otherwise.
         */
        static bool renderCredits( int x, int y, bool alpha );

        /**
         * @brief Restart the credits animation.
         *
         * When using rederCredits, you can restart the credits animation
         * from the begining before it's finished by calling this function.
         */
        static void resetCredits( );

        /**
         * @brief Setting the default background color.
         *
         * Changes the default background color for a console. The default
         * background color is used by several drawing functions like
         * clear, putChar, ...
         *
         * @param back the new default background color for this console.
         */
		void setDefaultBackground(const Color& back);

		/**
		 * Changes the default foreground color for a console.
		 *
		 * The default foreground color is used by several drawing function
		 * like clear, putChar, ...
		 *
		 * @param fore the new default foreground color for this console.
		 */
		void setDefaultForeground(const Color& fore);

        /**
         * Modifies all cells of a console :
         *
         * 1- Set the cell's background color to the console default background color.
         * 2- Set the cell's foreground color to the console default foreground color.
         * 3- Set the cell's ASCII code to 32 (space).
         */
        void clear( );

        /**
         * @brief Setting the background color of a cell.
         *
         * Modifies the background color of a cell, leaving other properties
         * (foreground color and ASCII code) unchanged.
         *
         * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
         * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
         * @param col the background color to use. You can use color constants.
         * @param flag this flag defines how the cell's background color is modified.
         *
         * @see TCOD_bkgnd_flag_t
         */
		void setCharBackground(int x, int y, const Doryen::Color& col, BackgroundFlag flag = BackgroundFlag::SET);

		/**
		@PageName console_draw_basic
		@FuncTitle Setting the foreground color of a cell
		@FuncDesc This function modifies the foreground color of a cell, leaving other properties (background color and ASCII code) unchanged.
		@Cpp void TCODConsole::setCharForeground(int x, int y, const Doryen::TCODColor &col)
		@C void TCOD_console_set_char_foreground(TCOD_console_t con,int x, int y, TCOD_color_t col)
		@Py console_set_char_foreground(con, x, y, col)
		@C# void TCODConsole::setCharForeground(int x, int y, Doryen::TCODColor col)
		@Lua Console:setCharForeground(x, y, col)
		@Param con in the C and Python versions, the offscreen console handler or NULL for the root console
		@Param x,y coordinates of the cell in the console.
			0 <= x < console width
			0 <= y < console height
		@Param col the foreground color to use. You can use color constants
		*/
        void setCharForeground( int x, int y, const Doryen::Color &col );

        /**
        @PageName console_draw_basic
        @FuncTitle Setting the ASCII code of a cell
        @FuncDesc This function modifies the ASCII code of a cell, leaving other properties (background and foreground colors) unchanged.
            Note that since a clear console has both background and foreground colors set to black for every cell, using setchar will produce black characters on black background. Use putchar instead.
        @Cpp void TCODConsole::setChar(int x, int y, int c)
        @C void TCOD_console_set_char(TCOD_console_t con,int x, int y, int c)
        @Py console_set_char(con, x,  y, c)
        @C# void TCODConsole::setChar(int x, int y, int c)
        @Lua Console:setChar(x, y, c)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param x,y coordinates of the cell in the console.
            0 <= x < console width
            0 <= y < console height
        @Param c the new ASCII code for the cell. You can use ASCII constants
        */
        void setChar( int x, int y, int c );

        /**
        @PageName console_draw_basic
        @FuncTitle Setting every property of a cell using default colors
        @FuncDesc This function modifies every property of a cell :
            * update the cell's background color according to the console default background color (see TCOD_bkgnd_flag_t).
            * set the cell's foreground color to the console default foreground color
            * set the cell's ASCII code to c
        @Cpp void TCODConsole::putChar(int x, int y, int c, TCOD_bkgnd_flag_t flag = TCOD_BKGND_DEFAULT)
        @C void TCOD_console_put_char(TCOD_console_t con,int x, int y, int c, TCOD_bkgnd_flag_t flag)
        @Py console_put_char( con, x,  y, c, flag=BKGND_DEFAULT)
        @C#
            void TCODConsole::putChar(int x, int y, int c)
            void TCODConsole::putChar(int x, int y, int c, TCODBackgroundFlag flag)
        @Lua
            Console:putChar(x, y, c)
            Console:putChar(x, y, c, flag)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param x,y coordinates of the cell in the console.
            0 <= x < console width
            0 <= y < console height
        @Param c the new ASCII code for the cell. You can use ASCII constants
        @Param flag this flag defines how the cell's background color is modified. See TCOD_bkgnd_flag_t
        */
		void putChar(int x, int y, int c, BackgroundFlag flag = BackgroundFlag::NONE);

        /**
        @PageName console_draw_basic
        @FuncTitle Setting every property of a cell using specific colors
        @FuncDesc This function modifies every property of a cell :
            * set the cell's background color to back.
            * set the cell's foreground color to fore.
            * set the cell's ASCII code to c.
        @Cpp void TCODConsole::putCharEx(int x, int y, int c, const Doryen::TCODColor & fore, const Doryen::TCODColor & back)
        @C void TCOD_console_put_char_ex(TCOD_console_t con,int x, int y, int c, TCOD_color_t fore, TCOD_color_t back)
        @Py console_put_char_ex( con, x,  y, c, fore, back)
        @C# void TCODConsole::putCharEx(int x, int y, int c, Doryen::TCODColor fore, Doryen::TCODColor back)
        @Lua Console:putCharEx(x, y, c, fore, back)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param x,y coordinates of the cell in the console.
            0 <= x < console width
            0 <= y < console height
        @Param c the new ASCII code for the cell. You can use ASCII constants
        @Param fore,back new foreground and background colors for this cell
        */
        void putCharEx( int x, int y, int c, const Doryen::Color &fore, const Doryen::Color &back );

        /**
        @PageName console_bkgnd_flag_t
        @PageTitle Background effect flags
        @PageFather console_draw
        @PageDesc This flag is used by most functions that modify a cell background color. It defines how the console's current background color is used to modify the cell's existing background color :
            TCOD_BKGND_NONE : the cell's background color is not modified.
            TCOD_BKGND_SET : the cell's background color is replaced by the console's default background color : newbk = curbk.
            TCOD_BKGND_MULTIPLY : the cell's background color is multiplied by the console's default background color : newbk = oldbk * curbk
            TCOD_BKGND_LIGHTEN : newbk = MAX(oldbk,curbk)
            TCOD_BKGND_DARKEN : newbk = MIN(oldbk,curbk)
            TCOD_BKGND_SCREEN : newbk = white - (white - oldbk) * (white - curbk) // inverse of multiply : (1-newbk) = (1-oldbk)*(1-curbk)
            TCOD_BKGND_COLOR_DODGE : newbk = curbk / (white - oldbk)
            TCOD_BKGND_COLOR_BURN : newbk = white - (white - oldbk) / curbk
            TCOD_BKGND_ADD : newbk = oldbk + curbk
            TCOD_BKGND_ADDALPHA(alpha) : newbk = oldbk + alpha*curbk
            TCOD_BKGND_BURN : newbk = oldbk + curbk - white
            TCOD_BKGND_OVERLAY : newbk = curbk.x <= 0.5 ? 2*curbk*oldbk : white - 2*(white-curbk)*(white-oldbk)
            TCOD_BKGND_ALPHA(alpha) : newbk = (1.0f-alpha)*oldbk + alpha*(curbk-oldbk)
            TCOD_BKGND_DEFAULT : use the console's default background flag
            Note that TCOD_BKGND_ALPHA and TCOD_BKGND_ADDALPHA are MACROS that needs a float parameter between (0.0 and 1.0). TCOD_BKGND_ALPH and TCOD_BKGND_ADDA should not be used directly (else they will have the same effect as TCOD_BKGND_NONE).
            For python, remove TCOD_ : libtcod.BKGND_NONE
            For C# : None, Set, Multiply, Lighten, Darken, Screen, ColodDodge, ColorBurn, Add, Burn Overlay, Default
            With lua, use tcod.None, ..., tcod.Default, BUT tcod.console.Alpha(value) and tcod.console.AddAlpha(value)
        */

		/**
        @PageName console_print
        @FuncTitle Setting the default alignment
        @FuncDesc This function defines the default alignment (see TCOD_alignment_t) for the console.
            This default alignment is used by several functions (print, printRect, ...).
            Values for alignment : TCOD_LEFT, TCOD_CENTER, TCOD_RIGHT (in python, remove TCOD_ : libtcod.LEFT).
            For C# and Lua : LeftAlignment, RightAlignment, CenterAlignment
        @Cpp void TCODConsole::setAlignment(TCOD_alignment_t alignment)
        @C void TCOD_console_set_alignment(TCOD_console_t con,TCOD_bkgnd_flag_t alignment)
        @Py console_set_alignment(con, alignment)
        @C# void TCODConsole::setAlignment(TCODAlignment alignment)
        @Lua Console:setAlignment(alignment)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param alignment defines how the strings are printed on screen.
        */
		void setAlignment(TCOD_alignment_t alignment);

		/**
		@PageName console_print
		@FuncTitle Printing a string with default parameters
		@FuncDesc This function print a string at a specific position using current default alignment, background flag, foreground and background colors.
		@Cpp void TCODConsole::print(int x, int y, const char *fmt, ...)
		@C void TCOD_console_print(TCOD_console_t con,int x, int y, const char *fmt, ...)
		@Py console_print(con, x, y, fmt)
		@C# void TCODConsole::print(int x, int y, string fmt)
		@Lua Console:print(x, y, fmt)
		@Param con in the C and Python versions, the offscreen console handler or NULL for the root console
		@Param x,y coordinate of the character in the console, depending on the default alignment for this console :
			* TCOD_LEFT : leftmost character of the string
			* TCOD_CENTER : center character of the string
			* TCOD_RIGHT : rightmost character of the string
		@Param fmt printf-like format string, eventually followed by parameters. You can use control codes to change the colors inside the string, except in C#.
		*/
		void print(int x, int y, const std::string& fmt);

		/**
		@PageName console_print
		@FuncTitle Printing a string with specific alignment and background mode
		@FuncDesc This function print a string at a specific position using specific alignment and background flag, but default foreground and background colors.
		@Cpp void TCODConsole::printEx(int x, int y, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const char *fmt, ...)
		@C void TCOD_console_print_ex(TCOD_console_t con,int x, int y, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const char *fmt, ...)
		@Py console_print_ex(con, x, y, flag, alignment, fmt)
		@C# void TCODConsole::printEx(int x, int y, TCODBackgroundFlag flag, TCODAlignment alignment, string fmt)
		@Lua Console::printEx(x, y, flag, alignment, fmt)
		@Param con in the C and Python versions, the offscreen console handler or NULL for the root console
		@Param x,y coordinate of the character in the console, depending on the alignment :
			* TCOD_LEFT : leftmost character of the string
			* TCOD_CENTER : center character of the string
			* TCOD_RIGHT : rightmost character of the string
		@Param flag this flag defines how the cell's background color is modified. See TCOD_bkgnd_flag_t
		@Param alignment defines how the strings are printed on screen.
		@Param fmt printf-like format string, eventually followed by parameters. You can use control codes to change the colors inside the string, except in C#.
		*/
        void printEx( int x, int y, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const char *fmt, ... );

        /**
        @PageName console_print
        @FuncTitle Printing a string with default parameters and autowrap
        @FuncDesc This function draws a string in a rectangle inside the console, using default colors, alignment and background mode.
            If the string reaches the borders of the rectangle, carriage returns are inserted.
            If h > 0 and the bottom of the rectangle is reached, the string is truncated. If h = 0, the string is only truncated if it reaches the bottom of the console.
            The function returns the height (number of console lines) of the printed string.
        @Cpp int TCODConsole::printRect(int x, int y, int w, int h, const char *fmt, ...)
        @C int TCOD_console_print_rect(TCOD_console_t con,int x, int y, int w, int h, const char *fmt, ...)
        @Py console_print_rect(con, x, y, w, h, fmt)
        @C# int TCODConsole::printRect(int x, int y, int w, int h, string fmt)
        @Lua Console:printRect(x, y, w, h, fmt)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param x,y coordinate of the character in the console, depending on the alignment :
            * TCOD_LEFT : leftmost character of the string
            * TCOD_CENTER : center character of the string
            * TCOD_RIGHT : rightmost character of the string
        @Param w,h size of the rectangle
            x <= x+w < console width
            y <= y+h < console height
        @Param fmt printf-like format string, eventually followed by parameters. You can use control codes to change the colors inside the string, except in C#.
        */
        int printRect( int x, int y, int w, int h, const char *fmt, ... );

        /**
        @PageName console_print
        @FuncTitle Printing a string with specific alignment and background mode and autowrap
        @FuncDesc This function draws a string in a rectangle inside the console, using default colors, but specific alignment and background mode.
            If the string reaches the borders of the rectangle, carriage returns are inserted.
            If h > 0 and the bottom of the rectangle is reached, the string is truncated. If h = 0, the string is only truncated if it reaches the bottom of the console.
            The function returns the height (number of console lines) of the printed string.
        @Cpp int TCODConsole::printRectEx(int x, int y, int w, int h, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const char *fmt, ...)
        @C int TCOD_console_print_rect_ex(TCOD_console_t con,int x, int y, int w, int h, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const char *fmt, ...)
        @Py console_print_rect_ex(con, x, y, w, h, flag, alignment, fmt)
        @C# int TCODConsole::printRectEx(int x, int y, int w, int h, TCODBackgroundFlag flag, TCODAlignment alignment, string fmt)
        @Lua Console:printRectEx(x, y, w, h, flag, alignment, fmt)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param x,y coordinate of the character in the console, depending on the alignment :
            * TCOD_LEFT : leftmost character of the string
            * TCOD_CENTER : center character of the string
            * TCOD_RIGHT : rightmost character of the string
        @Param w,h size of the rectangle
            x <= x+w < console width
            y <= y+h < console height
        @Param flag this flag defines how the cell's background color is modified. See TCOD_bkgnd_flag_t
        @Param alignment defines how the strings are printed on screen.
        @Param fmt printf-like format string, eventually followed by parameters. You can use control codes to change the colors inside the string, except in C#.
        */
        int
        printRectEx( int x, int y, int w, int h, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const char *fmt,
                     ... );

        /**
        @PageName console_print
        @FuncTitle Compute the height of an autowrapped string
        @FuncDesc This function returns the expected height of an autowrapped string without actually printing the string with printRect or printRectEx
        @Cpp int TCODConsole::getHeightRect(int x, int y, int w, int h, const char *fmt, ...)

        @C int TCOD_console_get_height_rect(TCOD_console_t con,int x, int y, int w, int h, const char *fmt, ...)
        @Py console_get_height_rect(con, x, y, w, h, fmt)
        @C# int TCODConsole::getHeightRect(int x, int y, int w, int h, string fmt)
        @Lua Console:getHeightRect(x, y, w, h, fmt)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param x,y coordinate of the rectangle upper-left corner in the console
        @Param w,h size of the rectangle
            x <= x+w < console width
            y <= y+h < console height
        @Param fmt printf-like format string, eventually followed by parameters. You can use control codes to change the colors inside the string, except in C#.
        */
		int getHeightRect(int x, int y, int w, int h, const char* fmt, ...);


		/**
		@PageName console_advanced
		@PageFather console_draw
		@PageTitle Advanced printing functions
		@FuncTitle Filling a rectangle with the background color
		@FuncDesc Fill a rectangle inside a console. For each cell in the rectangle :
			* set the cell's background color to the console default background color
			* if clear is true, set the cell's ASCII code to 32 (space)
		@Cpp void TCODConsole::rect(int x, int y, int w, int h, bool clear, TCOD_bkgnd_flag_t flag = TCOD_BKGND_DEFAULT)
		@C void TCOD_console_rect(TCOD_console_t con,int x, int y, int w, int h, bool clear, TCOD_bkgnd_flag_t flag)
		@Py console_rect(con,x,  y,  w, h, clear, flag=BKGND_DEFAULT)
		@C#
			void TCODConsole::rect(int x, int y, int w, int h, bool clear)
			void TCODConsole::rect(int x, int y, int w, int h, bool clear, TCODBackgroundFlag flag)
		@Lua
			Console:rect(x, y, w, h, clear)
			Console:rect(x, y, w, h, clear, flag)
		@Param con in the C and Python versions, the offscreen console handler or NULL for the root console
		@Param x,y coordinates of rectangle upper-left corner in the console.
			0 <= x < console width
			0 <= y < console height
		@Param w,h size of the rectangle in the console.
			x <= x+w < console width
			y <= y+h < console height
		@Param clear if true, all characters inside the rectangle are set to ASCII code 32 (space).
			If false, only the background color is modified
		@Param flag this flag defines how the cell's background color is modified. See TCOD_bkgnd_flag_t
		*/
		void rect(int x, int y, int rw, int rh, bool clear, BackgroundFlag flag = BackgroundFlag::DEFAULT);

		/**
		@PageName console_advanced
		@FuncTitle Drawing an horizontal line
		@FuncDesc Draws an horizontal line in the console, using ASCII code TCOD_CHAR_HLINE (196), and the console's default background/foreground colors.
		@Cpp void TCODConsole::hline(int x,int y, int l, TCOD_bkgnd_flag_t flag = TCOD_BKGND_DEFAULT)
		@C void TCOD_console_hline(TCOD_console_t con,int x,int y, int l, TCOD_bkgnd_flag_t flag)
		@Py console_hline(con,x,y,l,flag=BKGND_DEFAULT)
		@C#
			void TCODConsole::hline(int x,int y, int l)
			void TCODConsole::hline(int x,int y, int l, TCODBackgroundFlag flag)
		@Lua
			Console:hline(x,y, l)
			Console:hline(x,y, l, flag)
		@Param con in the C and Python versions, the offscreen console handler or NULL for the root console
		@Param x,y Coordinates of the line's left end in the console.
			0 <= x < console width
			0 <= y < console height
		@Param l The length of the line in cells 1 <= l <= console width - x
		@Param flag this flag defines how the cell's background color is modified. See TCOD_bkgnd_flag_t
		*/
		void hline(int x, int y, int l, BackgroundFlag flag = BackgroundFlag::DEFAULT);

        /**
        @PageName console_advanced
        @FuncTitle Drawing an vertical line
        @FuncDesc Draws an vertical line in the console, using ASCII code TCOD_CHAR_VLINE (179), and the console's default background/foreground colors.
        @Cpp void TCODConsole::vline(int x,int y, int l, TCOD_bkgnd_flag_t flag = TCOD_BKGND_DEFAULT)
        @C void TCOD_console_vline(TCOD_console_t con,int x,int y, int l, TCOD_bkgnd_flag_t flag)
        @Py console_vline(con,x,y,l,flag=BKGND_DEFAULT)
        @C#
            void TCODConsole::vline(int x,int y, int l)
            void TCODConsole::vline(int x,int y, int l, TCODBackgroundFlag flag)
        @Lua
            Console:vline(x,y, l)
            Console:vline(x,y, l, flag)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param x,y Coordinates of the line's upper end in the console.
            0 <= x < console width
            0 <= y < console height
        @Param l The length of the line in cells 1 <= l <= console height - y
        @Param flag this flag defines how the cell's background color is modified. See TCOD_bkgnd_flag_t
        */
		void vline(int x, int y, int l, BackgroundFlag flag = BackgroundFlag::DEFAULT);

		/**
		@PageName console_advanced
		@FuncTitle Drawing a window frame
		@FuncDesc This function calls the rect function using the supplied background mode flag, then draws a rectangle with the console's default foreground color. If fmt is not NULL, it is printed on the top of the rectangle, using inverted colors.
		@Cpp void TCODConsole::printFrame(int x,int y,int w,int h, bool clear=true, TCOD_bkgnd_flag_t flag = TCOD_BKGND_DEFAULT, const char *fmt=NULL, ...)
		@C void TCOD_console_print_frame(TCOD_console_t con,int x,int y,int w,int h, bool clear, TCOD_bkgnd_flag_t flag, const char *fmt, ...)
		@Py console_print_frame(con,x, y, w, h, clear=True, flag=BKGND_DEFAULT, fmt=0)
		@C#
			void TCODConsole::printFrame(int x,int y, int w,int h)
			void TCODConsole::printFrame(int x,int y, int w,int h, bool clear)
			void TCODConsole::printFrame(int x,int y, int w,int h, bool clear, TCODBackgroundFlag flag)
			void TCODConsole::printFrame(int x,int y, int w,int h, bool clear, TCODBackgroundFlag flag, string fmt)
		@Lua
			Console:printFrame(x,y, w,h)
			Console:printFrame(x,y, w,h, clear)
			Console:printFrame(x,y, w,h, clear, flag)
			Console:printFrame(x,y, w,h, clear, flag, fmt)
		@Param con in the C and Python versions, the offscreen console handler or NULL for the root console
		@Param x,y Coordinates of the rectangle's upper-left corner in the console.
			0 <= x < console width
			0 <= y < console height
		@Param w,h size of the rectangle in the console.
			x <= x+w < console width
			y <= y+h < console height
		@Param clear if true, all characters inside the rectangle are set to ASCII code 32 (space).
			If false, only the background color is modified
		@Param flag this flag defines how the cell's background color is modified. See TCOD_bkgnd_flag_t
		@Param fmt if NULL, the funtion only draws a rectangle.
			Else, printf-like format string, eventually followed by parameters. You can use control codes to change the colors inside the string.
		*/
		void printFrame(int x, int y, int w, int h, bool clear = true, BackgroundFlag flag = BackgroundFlag::DEFAULT,
				const std::string& name = "");

        /**
        @PageName console_read
        @PageTitle Reading the content of the console
        @PageFather console_draw
        @FuncTitle Get the console's width
        @FuncDesc This function returns the width of a console (either the root console or an offscreen console)
        @Cpp int TCODConsole::getWidth() const
        @C int TCOD_console_get_width(TCOD_console_t con)
        @Py console_get_width(con)
        @C# int TCODConsole::getWidth()
        @Lua Console:getWidth()
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        */
		unsigned int getWidth() const;

		/**
		@PageName console_read
		@FuncTitle Get the console's height
		@FuncDesc This function returns the height of a console (either the root console or an offscreen console)
		@Cpp int TCODConsole::getHeight() const
		@C int TCOD_console_get_height(TCOD_console_t con)
		@Py console_get_height(con)
		@C# int TCODConsole::getHeight()
		@Lua Console:getHeight()
		@Param con in the C and Python versions, the offscreen console handler or NULL for the root console
		*/
		unsigned int getHeight() const;

		/**
		@PageName console_read
		@FuncTitle Reading the default background color
		@FuncDesc This function returns the default background color of a console.
		@Cpp Doryen::TCODColor TCODConsole::getDefaultBackground() const
		@C TCOD_color_t TCOD_console_get_default_background(TCOD_console_t con)
		@Py console_get_default_background(con)
		@C# Doryen::TCODColor TCODConsole::getBackgroundColor()
		@Lua Console:getBackgroundColor()
		@Param con in the C and Python versions, the offscreen console handler or NULL for the root console
		*/
		const Color& getDefaultBackground() const;

		/**
		@PageName console_read
		@FuncTitle Reading the default foreground color
		@FuncDesc This function returns the default foreground color of a console.
		@Cpp Doryen::TCODColor TCODConsole::getDefaultForeground() const
		@C TCOD_color_t TCOD_console_get_default_foreground(TCOD_console_t con)
		@Py console_get_default_foreground(con)
		@C# Doryen::TCODColor TCODConsole::getForegroundColor()
		@Lua Console:getForegroundColor()
		@Param con in the C and Python versions, the offscreen console handler or NULL for the root console
		*/
		const Color& getDefaultForeground() const;

        /**
        @PageName console_read
        @FuncTitle Reading the background color of a cell
        @FuncDesc This function returns the background color of a cell.
        @Cpp Doryen::TCODColor TCODConsole::getCharBackground(int x, int y) const
        @C TCOD_color_t TCOD_console_get_char_background(TCOD_console_t con,int x, int y)
        @Py console_get_char_background(con,x,y)
        @C# Doryen::TCODColor TCODConsole::getCharBackground(int x, int y)
        @Lua Console::getCharBackground(x, y)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param x,y coordinates of the cell in the console.
            0 <= x < console width
            0 <= y < console height
        */
        Doryen::Color getCharBackground( int x, int y ) const;

        /**
        @PageName console_read
        @FuncTitle Reading the foreground color of a cell
        @FuncDesc This function returns the foreground color of a cell.
        @Cpp Doryen::TCODColor TCODConsole::getCharForeground(int x, int y) const
        @C TCOD_color_t TCOD_console_get_char_foreground(TCOD_console_t con,int x, int y)
        @Py console_get_char_foreground(con,x,y)
        @C# Doryen::TCODColor TCODConsole::getCharForeground(int x, int y)
        @Lua Console::getCharForeground(x, y)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param x,y coordinates of the cell in the console.
            0 <= x < console width
            0 <= y < console height
        */
        Doryen::Color getCharForeground( int x, int y ) const;

        /**
        @PageName console_read
        @FuncTitle Reading the ASCII code of a cell
        @FuncDesc This function returns the ASCII code of a cell.
        @Cpp int TCODConsole::getChar(int x, int y) const
        @C int TCOD_console_get_char(TCOD_console_t con,int x, int y)
        @Py console_get_char(con,x,y)
        @C# int TCODConsole::getChar(int x, int y)
        @Lua Console::getChar(x, y)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param x,y coordinates of the cell in the console.
            0 <= x < console width
            0 <= y < console height
        */
        int getChar( int x, int y ) const;

        /**
        @PageName console_fading
        @PageTitle Screen fading functions
        @PageFather console_draw
        @PageDesc Use these functions to easily fade to/from a color
        @FuncTitle Changing the fading parameters
        @FuncDesc This function defines the fading parameters, allowing to easily fade the game screen to/from a color. Once they are defined, the fading parameters are valid for ever. You don't have to call setFade for each rendered frame (unless you change the fading parameters).
        @Cpp static void TCODConsole::setFade(uint8 fade, const Doryen::TCODColor &fadingColor)
        @C void TCOD_console_set_fade(uint8 fade, TCOD_color_t fadingColor)
        @Py console_set_fade(fade, fadingColor)
        @C# static void TCODConsole::setFade(byte fade, Doryen::TCODColor fadingColor)
        @Lua tcod.console.setFade(fade, fadingColor)
        @Param fade the fading amount. 0 => the screen is filled with the fading color. 255 => no fading effect
        @Param fadingColor the color to use during the console flushing operation
        @CppEx
            for (int fade=255; fade >= 0; fade --) {
                TCODConsole::setFade(fade,Doryen::TCODColor::black);
                TCODConsole::flush();
            }
        @CEx
            int fade;
            for (fade=255; fade >= 0; fade --) {
                TCOD_console_setFade(fade,TCOD_black);
                TCOD_console_flush();
            }
        @PyEx
            for fade in range(255,0) :
                libtcod.console_setFade(fade,libtcod.black)
                libtcod.console_flush()
        @LuaEx
            for fade=255,0,-1 do
                tcod.console.setFade(fade,tcod.color.black)
                tcod.console.flush()
            end
        */
		static void setFade(short val, const Doryen::Color& fade);

        /**
        @PageName console_fading
        @FuncTitle Reading the fade amount
        @FuncDesc This function returns the current fade amount, previously defined by setFade.
        @Cpp static uint8 TCODConsole::getFade()
        @C uint8 TCOD_console_get_fade()
        @Py console_get_fade()
        @C# static byte TCODConsole::getFade()
        @Lua tcod.console.getFade()
        */
		static short getFade();

        /**
        @PageName console_fading
        @FuncTitle Reading the fading color
        @FuncDesc This function returns the current fading color, previously defined by setFade.
        @Cpp static Doryen::TCODColor TCODConsole::getFadingColor()
        @C TCOD_color_t TCOD_console_get_fading_color()
        @Py console_get_fading_color()
        @C# static Doryen::TCODColor TCODConsole::getFadingColor()
        @Lua tcod.console.getFadingColor()
        */
        static Doryen::Color getFadingColor( );

        /**
        @PageName console_flush
        @PageFather console
        @PageTitle Flushing the root console
        @FuncDesc Once the root console is initialized, you can use one of the printing functions to change the background colors, the foreground colors or the ASCII characters on the console.
            Once you've finished rendering the root console, you have to actually apply the updates to the screen with this function.
        @Cpp static void TCODConsole::flush()
        @C void TCOD_console_flush()
        @Py console_flush()
        @C# static void TCODConsole::flush()
        @Lua tcod.console.flush()
        */
        static void flush( );

        /**
        @PageName console_ascii
        @PageTitle ASCII constants
        @PageFather console_draw
        @FuncDesc Some useful graphic characters in the terminal.bmp font. For the python version, remove TCOD_ from the constants. C# and Lua is in parenthesis :
            Single line walls:
            TCOD_CHAR_HLINE=196 (HorzLine)
            TCOD_CHAR_VLINE=179 (VertLine)
            TCOD_CHAR_NE=191 (NE)
            TCOD_CHAR_NW=218 (NW)
            TCOD_CHAR_SE=217 (SE)
            TCOD_CHAR_SW=192 (SW)

            Double lines walls:
            TCOD_CHAR_DHLINE=205 (DoubleHorzLine)
            TCOD_CHAR_DVLINE=186 (DoubleVertLine)
            TCOD_CHAR_DNE=187 (DoubleNE)
            TCOD_CHAR_DNW=201 (DoubleNW)
            TCOD_CHAR_DSE=188 (DoubleSE)
            TCOD_CHAR_DSW=200 (DoubleSW)

            Single line vertical/horizontal junctions (T junctions):
            TCOD_CHAR_TEEW=180 (TeeWest)
            TCOD_CHAR_TEEE=195 (TeeEast)
            TCOD_CHAR_TEEN=193 (TeeNorth)
            TCOD_CHAR_TEES=194 (TeeSouth)

            Double line vertical/horizontal junctions (T junctions):
            TCOD_CHAR_DTEEW=185 (DoubleTeeWest)
            TCOD_CHAR_DTEEE=204 (DoubleTeeEast)
            TCOD_CHAR_DTEEN=202 (DoubleTeeNorth)
            TCOD_CHAR_DTEES=203 (DoubleTeeSouth)

            Block characters:
            TCOD_CHAR_BLOCK1=176 (Block1)
            TCOD_CHAR_BLOCK2=177 (Block2)
            TCOD_CHAR_BLOCK3=178 (Block3)

            Cross-junction between two single line walls:
            TCOD_CHAR_CROSS=197 (Cross)

            Arrows:
            TCOD_CHAR_ARROW_N=24 (ArrowNorth)
            TCOD_CHAR_ARROW_S=25 (ArrowSouth)
            TCOD_CHAR_ARROW_E=26 (ArrowEast)
            TCOD_CHAR_ARROW_W=27 (ArrowWest)

            Arrows without tail:
            TCOD_CHAR_ARROW2_N=30 (ArrowNorthNoTail)
            TCOD_CHAR_ARROW2_S=31 (ArrowSouthNoTail)
            TCOD_CHAR_ARROW2_E=16 (ArrowEastNoTail)
            TCOD_CHAR_ARROW2_W=17 (ArrowWestNoTail)

            Double arrows:
            TCOD_CHAR_DARROW_H=29 (DoubleArrowHorz)
            TCOD_CHAR_ARROW_V=18 (DoubleArrowVert)

            GUI stuff:
            TCOD_CHAR_CHECKBOX_UNSET=224
            TCOD_CHAR_CHECKBOX_SET=225
            TCOD_CHAR_RADIO_UNSET=9
            TCOD_CHAR_RADIO_SET=10

            Sub-pixel resolution kit:
            TCOD_CHAR_SUBP_NW=226 (SubpixelNorthWest)
            TCOD_CHAR_SUBP_NE=227 (SubpixelNorthEast)
            TCOD_CHAR_SUBP_N=228 (SubpixelNorth)
            TCOD_CHAR_SUBP_SE=229 (SubpixelSouthEast)
            TCOD_CHAR_SUBP_DIAG=230 (SubpixelDiagonal)
            TCOD_CHAR_SUBP_E=231 (SubpixelEast)
            TCOD_CHAR_SUBP_SW=232 (SubpixelSouthWest)

            Miscellaneous characters:
            TCOD_CHAR_SMILY = 1 (Smilie)
            TCOD_CHAR_SMILY_INV = 2 (SmilieInv)
            TCOD_CHAR_HEART = 3 (Heart)
            TCOD_CHAR_DIAMOND = 4 (Diamond)
            TCOD_CHAR_CLUB = 5 (Club)
            TCOD_CHAR_SPADE = 6 (Spade)
            TCOD_CHAR_BULLET = 7 (Bullet)
            TCOD_CHAR_BULLET_INV = 8 (BulletInv)
            TCOD_CHAR_MALE = 11 (Male)
            TCOD_CHAR_FEMALE = 12 (Female)
            TCOD_CHAR_NOTE = 13 (Note)
            TCOD_CHAR_NOTE_DOUBLE = 14 (NoteDouble)
            TCOD_CHAR_LIGHT = 15 (Light)
            TCOD_CHAR_EXCLAM_DOUBLE = 19 (ExclamationDouble)
            TCOD_CHAR_PILCROW = 20 (Pilcrow)
            TCOD_CHAR_SECTION = 21 (Section)
            TCOD_CHAR_POUND = 156 (Pound)
            TCOD_CHAR_MULTIPLICATION = 158 (Multiplication)
            TCOD_CHAR_FUNCTION = 159 (Function)
            TCOD_CHAR_RESERVED = 169 (Reserved)
            TCOD_CHAR_HALF = 171 (Half)
            TCOD_CHAR_ONE_QUARTER = 172 (OneQuarter)
            TCOD_CHAR_COPYRIGHT = 184 (Copyright)
            TCOD_CHAR_CENT = 189 (Cent)
            TCOD_CHAR_YEN = 190 (Yen)
            TCOD_CHAR_CURRENCY = 207 (Currency)
            TCOD_CHAR_THREE_QUARTERS = 243 (ThreeQuarters)
            TCOD_CHAR_DIVISION = 246 (Division)
            TCOD_CHAR_GRADE = 248 (Grade)
            TCOD_CHAR_UMLAUT = 249 (Umlaut)
            TCOD_CHAR_POW1 = 251 (Pow1)
            TCOD_CHAR_POW3 = 252 (Pow2)
            TCOD_CHAR_POW2 = 253 (Pow3)
            TCOD_CHAR_BULLET_SQUARE = 254 (BulletSquare)
        */

        /**
        @PageName console_input
        @PageTitle Handling keyboard input
        @PageDesc The keyboard handling functions allow you to get keyboard input from the user, either for turn by turn games (the function wait until the user press a key), or real time games (non blocking function).
        <b>WARNING : for proper redraw event handling, the keyboard functions should always be called just after TCODConsole::flush !</b>
        @PageFather console
        */

/**
	@PageName console_blocking_input
	@PageTitle Blocking keyboard input
	@PageFather console_input
	@FuncDesc This function waits for the user to press a key. It returns the code of the key pressed as well as the corresponding character. See TCOD_key_t.
		If the flush parameter is true, every pending keypress event is discarded, then the function wait for a new keypress.
		If flush is false, the function waits only if there are no pending keypress events, else it returns the first event in the keyboard buffer.
	@Cpp static TCOD_key_t TCODConsole::waitForKeypress(bool flush)
	@C TCOD_key_t TCOD_console_wait_for_keypress(bool flush)
	@Py console_wait_for_keypress(flush)
	@C# static TCOD_key_t TCODConsole::waitForKeypress(bool flush)
	@Lua tcod.console.waitForKeypress(flush)
	@Param flush if true, all pending keypress events are flushed from the keyboard buffer. Else, return the first available event
	@CppEx
		TCOD_key_t key = TCODConsole::waitForKeypress(true);
		if ( key.c == 'i' ) { ... open inventory ... }
	@CEx
		TCOD_key_t key = TCOD_console_wait_for_keypress(true);
		if ( key.c == 'i' ) { ... open inventory ... }
	@PyEx
		key = libtcod.console_wait_for_keypress(True)
		if key.c == ord('i') : # ... open inventory ...
	@LuaEx
		key = tcod.console.waitForKeypress(true)
		if key.Character == 'i' then ... open inventory ... end
	*/
		static bool isKeyPressed(KeyCode _key);

        /**
        @PageName console_non_blocking_input
        @PageTitle Non blocking keyboard input
        @PageFather console_input
        @FuncDesc This function checks if the user has pressed a key. It returns the code of the key pressed as well as the corresponding character. See TCOD_key_t. If the user didn't press a key, this function returns the key code TCODK_NONE (NoKey for C# and Lua).
            <b>Note that key repeat only results in TCOD_KEY_PRESSED events.</b>
        @Cpp static TCOD_key_t TCODConsole::checkForKeypress(int flags=TCOD_KEY_RELEASED)
        @C TCOD_key_t TCOD_console_check_for_keypress(int flags)
        @Py console_check_for_keypress(flags=KEY_RELEASED)
        @C# static TCODKey TCODConsole::checkForKeypress(int flags) // Use TCODKeyStatus
        @Lua tcod.console.checkForKeypress(flags)
        @Param flags A filter for key events (C# and Lua in parenthesis):
            TCOD_KEY_PRESSED (KeyPressed) : only keypress events are returned
            TCOD_KEY_RELEASED (KeyReleased): only key release events are returnes
            TCOD_KEY_PRESSED|TCOD_KEY_RELEASED (KeyPressed+KeyReleased): events of both types are returned.
        @CppEx
            TCOD_key_t key = TCODConsole::checkForKeypress();
            if ( key.vk == TCODK_NONE ) return; // no key pressed
            if ( key.c == 'i' ) { ... open inventory ... }
        @C
            TCOD_key_t key = TCOD_console_check_for_keypress(TCOD_KEY_PRESSED);
            if ( key.vk == TCODK_NONE ) return; // no key pressed
            if ( key.c == 'i' ) { ... open inventory ... }
        @Py
            key = libtcod.console_check_for_keypress()
            if key.vk == libtcod.KEY_NONE return # no key pressed
            if key.c == ord('i') : # ... open inventory ...
        @LuaEx
            key = tcod.console.checkForKeypress()
            if key.KeyCode == tcod.NoKey then return end -- no key pressed
            if key.Character == 'i' then ... open inventory ... end
        */
		static Doryen::Key getKeyPressed();

        /**
        @PageName console_non_blocking_input
        @PageTitle Non blocking keyboard input
        @PageFather console_input
        @FuncDesc You can also get the status of any special key at any time with :
        @Cpp static bool TCODConsole::isKeyPressed(TCOD_keycode_t key)
        @C bool TCOD_console_is_key_pressed(TCOD_keycode_t key)
        @Py console_is_key_pressed(key)
        @C# static bool TCODConsole::isKeyPressed(TCODKeyCode key)
        @Lua tcod.console.isKeyPressed(key)
        @Param key Any key code defined in keycode_t except TCODK_CHAR (Char) and TCODK_NONE (NoKey)
        */
        static bool isKeyPressed( TCOD_keycode_t key );

        /**
        @PageName console_keyboard_repeat
        @PageTitle Changing keyboard repeat delay
        @PageFather console_input
        @FuncDesc This function changes the keyboard repeat times.
        @Cpp static void TCODConsole::setKeyboardRepeat(int initialDelay, int interval)
        @C void TCOD_console_set_keyboard_repeat(int initial_delay, int interval)
        @Py console_set_keyboard_repeat(initial_delay, interval)
        @C# static void TCODConsole::setKeyboardRepeat(int initialDelay, int interval)
        @Lua tcod.console.setKeyboardRepeat(initialDelay, interval)
        @Param initialDelay delay in millisecond between the time when a key is pressed, and keyboard repeat begins. If 0, keyboard repeat is disabled
        @Param interval interval in millisecond between keyboard repeat events
        */
        static void setKeyboardRepeat( int initialDelay, int interval );

        /**
        @PageName console_keyboard_repeat
        @FuncDesc You can also disable the keyboard repeat feature with this function (it's equivalent to setKeyboardRepeat(0,0) ).
        @Cpp static void TCODConsole::disableKeyboardRepeat()
        @C void TCOD_console_disable_keyboard_repeat()
        @Py console_disable_keyboard_repeat()
        @C# static void TCODConsole::disableKeyboardRepeat()
        @Lua tcod.console.disableKeyboardRepeat()
        */
        static void disableKeyboardRepeat( );

		/**
		@PageName console_keycode_t
		@PageTitle 	Key codes
		@PageFather console_input
		@PageDesc TCOD_keycode_t is a libtcod specific code representing a key on the keyboard.
			For python, replace TCODK by KEY: libtcod.KEY_NONE. C# and Lua, the value is in parenthesis. Possible values are :
			When no key was pressed (see checkForKeypress) : TCOD_NONE (NoKey)
			Special keys :
			TCODK_ESCAPE (Escape)
			TCODK_BACKSPACE (Backspace)
			TCODK_TAB (Tab)
			TCODK_ENTER (Enter)
			TCODK_SHIFT (Shift)
			TCODK_CONTROL (Control)
			TCODK_ALT (Alt)
			TCODK_PAUSE (Pause)
			TCODK_CAPSLOCK (CapsLock)
			TCODK_PAGEUP (PageUp)
			TCODK_PAGEDOWN (PageDown)
			TCODK_END (End)
			TCODK_HOME (Home)
			TCODK_UP (Up)
			TCODK_LEFT (Left)
			TCODK_RIGHT (Right)
			TCODK_DOWN (Down)
			TCODK_PRINTSCREEN (Printscreen)
			TCODK_INSERT (Insert)
			TCODK_DELETE (Delete)
			TCODK_LWIN (Lwin)
			TCODK_RWIN (Rwin)
			TCODK_APPS (Apps)
			TCODK_KPADD (KeypadAdd)
			TCODK_KPSUB (KeypadSubtract)
			TCODK_KPDIV (KeypadDivide)
			TCODK_KPMUL (KeypadMultiply)
			TCODK_KPDEC (KeypadDecimal)
			TCODK_KPENTER (KeypadEnter)
			TCODK_F1 (F1)
			TCODK_F2 (F2)
			TCODK_F3 (F3)
			TCODK_F4 (F4)
			TCODK_F5 (F5)
			TCODK_F6 (F6)
			TCODK_F7 (F7)
			TCODK_F8 (F8)
			TCODK_F9 (F9)
			TCODK_F10 (F10)
			TCODK_F11 (F11)
			TCODK_F12 (F12)
			TCODK_NUMLOCK (Numlock)
			TCODK_SCROLLLOCK (Scrolllock)
			TCODK_SPACE (Space)

			numeric keys :

			TCODK_0 (Zero)
			TCODK_1 (One)
			TCODK_2 (Two)
			TCODK_3 (Three)
			TCODK_4 (Four)
			TCODK_5 (Five)
			TCODK_6 (Six)
			TCODK_7 (Seven)
			TCODK_8 (Eight)
			TCODK_9 (Nine)
			TCODK_KP0 (KeypadZero)
			TCODK_KP1 (KeypadOne)
			TCODK_KP2 (KeypadTwo)
			TCODK_KP3 (KeypadThree)
			TCODK_KP4 (KeypadFour)
			TCODK_KP5 (KeypadFive)
			TCODK_KP6 (KeypadSix)
			TCODK_KP7 (KeypadSeven)
			TCODK_KP8 (KeypadEight)
			TCODK_KP9 (KeypadNine)

			Any other (printable) key :

			TCODK_CHAR (Char)

			Codes starting with TCODK_KP represents keys on the numeric keypad (if available).
		*/



		/**
        @PageName console_offscreen
        @FuncTitle Blitting a console on another one
        @FuncDesc This function allows you to blit a rectangular area of the source console at a specific position on a destination console. It can also simulate alpha transparency with the fade parameter.
        @Cpp static void blit(const TCODConsole *src,int xSrc, int ySrc, int wSrc, int hSrc, TCODConsole *dst, int xDst, int yDst, float foregroundAlpha=1.0f, float backgroundAlpha=1.0f)
        @C void TCOD_console_blit(TCOD_console_t src,int xSrc, int ySrc, int wSrc, int hSrc, TCOD_console_t dst, int xDst, int yDst, float foreground_alpha, float background_alpha)
        @Py console_blit(src,xSrc,ySrc,xSrc,hSrc,dst,xDst,yDst,foregroundAlpha=1.0,backgroundAlpha=1.0)
        @C#
            static void TCODConsole::blit(TCODConsole src, int xSrc, int ySrc, int wSrc, int hSrc, TCODConsole dst, int xDst, int yDst)
            static void TCODConsole::blit(TCODConsole src, int xSrc, int ySrc, int wSrc, int hSrc, TCODConsole dst, int xDst, int yDst, float foreground_alpha)
            static void TCODConsole::blit(TCODConsole src, int xSrc, int ySrc, int wSrc, int hSrc, TCODConsole dst, int xDst, int yDst, float foreground_alpha, float background_alpha)
        @Lua
            tcod.console.blit(src, xSrc, ySrc, wSrc, hSrc, dst, xDst, yDst)
            tcod.console.blit(src, xSrc, ySrc, wSrc, hSrc, dst, xDst, yDst, foreground_alpha)
            tcod.console.blit(src, xSrc, ySrc, wSrc, hSrc, dst, xDst, yDst, foreground_alpha, background_alpha)
        @Param src The source console that must be blitted on another one.
        @Param xSrc,ySrc,wSrc,hSrc The rectangular area of the source console that will be blitted. If wSrc and/or hSrc == 0, the source console width/height are used
        @Param dst The destination console.
        @Param xDst,yDst Where to blit the upper-left corner of the source area in the destination console.
        @Param foregroundAlpha,backgroundAlpha Alpha transparency of the blitted console.
            0.0 => The source console is completely transparent. This function does nothing.
            1.0 => The source console is opaque. Its cells replace the destination cells.
            0 < fade < 1.0 => The source console is partially blitted, simulating real transparency.
        @CppEx
            // Cross-fading between two offscreen consoles. We use two offscreen consoles with the same size as the root console. We render a different screen on each offscreen console. When the user hits a key, we do a cross-fading from the first screen to the second screen.
            TCODConsole *off1 = new TCODConsole(80,50);
            TCODConsole *off2 = new TCODConsole(80,50);
            ... print screen1 on off1
            ... print screen2 of off2
            // render screen1 in the game window
            TCODConsole::blit(off1,0,0,80,50,TCODConsole::root,0,0);
            TCODConsole::flush();
            // wait or a keypress
            TCODConsole::waitForKeypress(true);
            // do a cross-fading from off1 to off2
            for (int i=1; i <= 255; i++) {
                TCODConsole::blit(off1,0,0,80,50,TCODConsole::root,0,0); // renders the first screen (opaque)
                TCODConsole::blit(off2,0,0,80,50,TCODConsole::root,0,0,i/255.0,i/255.0); // renders the second screen (transparent)
                TCODConsole::flush();
            }
        @CEx
            TCOD_console_t off1 = TCOD_console_new(80,50);
            TCOD_console_t off2 = TCOD_console_new(80,50);
            int i;
            ... print screen1 on off1
            ... print screen2 of off2
            // render screen1 in the game window
            TCOD_console_blit(off1,0,0,80,50,NULL,0,0,1.0,1.0);
            TCOD_console_flush();
            // wait or a keypress
            TCOD_console_wait_for_keypress(true);
            // do a cross-fading from off1 to off2
            for (i=1; i <= 255; i++) {
                TCOD_console_blit(off1,0,0,80,50,NULL,0,0,1.0,1.0); // renders the first screen (opaque)
                TCOD_console_blit(off2,0,0,80,50,NULL,0,0,i/255.0,i/255.0); // renders the second screen (transparent)
                TCOD_console_flush();
            }
        @PyEx
            off1 = libtcod.console_new(80,50)
            off2 = libtcod.console_new(80,50)
            ... print screen1 on off1
            ... print screen2 of off2
            # render screen1 in the game window
            libtcod.console_blit(off1,0,0,80,50,0,0,0)
            libtcod.console_flush()
            # wait or a keypress
            libtcod.console_wait_for_keypress(True)
            # do a cross-fading from off1 to off2
            for i in range(1,256) :
                litbcod.console_blit(off1,0,0,80,50,0,0,0) # renders the first screen (opaque)
                litbcod.console_blit(off2,0,0,80,50,0,0,0,i/255.0,i/255.0) # renders the second screen (transparent)
                litbcod.console_flush()
        @LuaEx
            -- Cross-fading between two offscreen consoles. We use two offscreen consoles with the same size as the root console. We render a different screen on each offscreen console. When the user hits a key, we do a cross-fading from the first screen to the second screen.
            off1 = tcod.Console(80,50)
            off2 = tcod.Console(80,50)
            ... print screen1 on off1
            ... print screen2 of off2
            -- render screen1 in the game window
            root=libtcod.TCODConsole_root
            tcod.console.blit(off1,0,0,80,50,root,0,0)
            tcod.console.flush()
            -- wait or a keypress
            tcod.console.waitForKeypress(true)
            -- do a cross-fading from off1 to off2
            for i=1,255,1 do
                tcod.console.blit(off1,0,0,80,50,root,0,0) -- renders the first screen (opaque)
                tcod.console.blit(off2,0,0,80,50,root,0,0,i/255,i/255) -- renders the second screen (transparent)
                tcod.console.flush()
            end
        */
        static void
        blit( const Console *src, int xSrc, int ySrc, int wSrc, int hSrc, Console *dst, int xDst, int yDst,
              float foreground_alpha = 1.0f, float background_alpha = 1.0f );


        /**
        @PageName console_offscreen
        @FuncTitle Define a blit-transparent color
        @FuncDesc This function defines a transparent background color for an offscreen console. All cells with this background color are ignored by the blit operation. You can use it to blit only some parts of the console.
        @Cpp void TCODConsole::setKeyColor(const Doryen::TCODColor &col)
        @C void TCOD_console_set_key_color(TCOD_console_t con,TCOD_color_t col)
        @Py console_set_key_color(con,col)
        @C# void TCODConsole::setKeyColor(Doryen::TCODColor col)
        @Lua Console:setKeyColor(col)
        @Param con in the C and Python versions, the offscreen console handler or NULL for the root console
        @Param col the transparent background color
        */
        void setKeyColor( const Doryen::Color &col );

		/**
		@PageName console_key_t
		@PageTitle 	Keyboard event structure
		@PageFather console_input
		@PageDesc This structure contains information about a key pressed/released by the user.
		@C
			typedef struct {
				TCOD_keycode_t vk;
				char c;
				bool pressed;
				bool lalt;
				bool lctrl;
				bool ralt;
				bool rctrl;
				bool shift;
			} TCOD_key_t;
		@Lua
			key.KeyCode
			key.Character
			key.Pressed
			key.LeftAlt
			key.LeftControl
			key.RightAlt
			key.RightControl
			key.Shift
		@Param vk An arbitrary value representing the physical key on the keyboard. Possible values are stored in the TCOD_keycode_t enum. If no key was pressed, the value is TCODK_NONE
		@Param c If the key correspond to a printable character, the character is stored in this field. Else, this field contains 0.
		@Param pressed true if the event is a key pressed, or false for a key released.
		@Param lalt This field represents the status of the left Alt key : true => pressed, false => released.
		@Param lctrl This field represents the status of the left Control key : true => pressed, false => released.
		@Param ralt This field represents the status of the right Alt key : true => pressed, false => released.
		@Param rctrl This field represents the status of the right Control key : true => pressed, false => released.
		@Param shift This field represents the status of the shift key : true => pressed, false => released.
		*/

		void setDirty(int x, int y, int w, int h);

	private:

		void drawFrame(const Math::Point2D& start, const Math::Point2D& end, bool empty, BackgroundFlag flag);

	};
}


#endif

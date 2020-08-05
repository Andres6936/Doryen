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

#include <string_view>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <array>

#include "Doryen/Graphics/Color/Palette.hpp"
#include "Doryen/Graphics/Color/Color.hpp"
#include "Doryen/Renderer/Renderer.hpp"
#include "Doryen/Geometry/Size.hpp"
#include "Doryen/Renderer/Sdl.hpp"

#include "Doryen/libtcod.h"
#include "Doryen/console_types.h"

/**
        @PageName console_bkgnd_flag_t
        @PageTitle Background effect flags
        @PageFather console_draw
        @PageDesc

            Note that TCOD_BKGND_ALPHA and TCOD_BKGND_ADDALPHA are MACROS that needs a float parameter between (0.0 and 1.0). TCOD_BKGND_ALPH and TCOD_BKGND_ADDA should not be used directly (else they will have the same effect as TCOD_BKGND_NONE).
            For python, remove TCOD_ : libtcod.BKGND_NONE
            For C# : None, Set, Multiply, Lighten, Darken, Screen, ColodDodge, ColorBurn, Add, Burn Overlay, Default
            With lua, use tcod.None, ..., tcod.Default, BUT tcod.console.Alpha(value) and tcod.console.AddAlpha(value)
        */

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

		inline static std::unique_ptr<Renderer> renderer = std::make_unique<SDL>();

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
		Color foreground = Palette::GRAY_WARN_1;

		/**
		 * Background text.
		 */
		Color background = Palette::GRAY_WARN_90;

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
		initRoot(int w, int h, const char* title, bool _fullscreen = false);

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
		 * When you start the program, this returns true. Once a "close window"
		 * event has been sent by the window manager, it will allways return
		 * false. You're supposed to exit cleanly the game.
		 *
		 * @return false if the user not want exit the app, true otherwise.
		 */
		bool isRunning();

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
		void setCharForeground(int x, int y, const Doryen::Color& col);

		/**
		 * @brief Setting every property of a cell using default colors
		 *
		 * This function modifies every property of a cell:
		 *
		 * - Update the cell's background color according to the console
		 *   default background color.
		 *
		 * - Set the cell's foreground color to the console default
		 *   foreground color.
		 *
		 * - Set the cell's ASCII code to c
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
		 * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param c the new ASCII code for the cell. You can use ASCII constants.
		 * @param flag this flag defines how the cell's background color is modified.
		 */
		void writeChar(int x, int y, int c, BackgroundFlag flag = BackgroundFlag::NONE);

		/**
		 * @brief Setting every property of a cell using specific colors
		 *
		 * This function modifies every property of a cell:
		 *
		 * - Set the cell's background color to background color pass for parameter.
		 *
		 * - Set the cell's foreground color to foreground color pass for parameter.
		 *
		 * - Set the cell's ASCII code to c
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
		 * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param c the new ASCII code for the cell. You can use ASCII constants.
		 * @param foreground Foreground color for this cell.
		 * @param background Background color for this cell.
		 */
		void writeChar(int x, int y, int c, const Color& foreground, const Color& background);

		/**
		 * @brief Setting every property of a cell using specific colors
		 *
		 * This function modifies every property of a cell:
		 *
		 * - Set the cell's background color to background color pass for parameter.
		 *
		 * - Set the cell's foreground color to foreground color pass for parameter.
		 *
		 * - Set the cell's ASCII code to c
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
		 * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param c the new ASCII code for the cell. You can use ASCII constants.
		 * @param foreground Foreground color for this cell.
		 * @param background Background color for this cell.
		 * @param flag this flag defines how the cell's background color is modified.
		 */
		void writeChar(int x, int y, int c, const Color& foreground, const Color& background, BackgroundFlag flag);

		/**
		 * Write a string in the console. You can use the function format for formatter
		 * the string before of that it will be wrote in the console.
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
         * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param fmt String to write in the console.
		 */
		void write(int x, int y, const std::string& fmt);

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
		printRectEx(int x, int y, int w, int h, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const char* fmt,
				...);

		/**
		 * @brief Writing a string with default parameters and auto-wrap.
		 *
		 * This function draws a string in a rectangle inside the console,
		 * using default colors, alignment and background mode.
		 *
		 * If the string reaches the borders of the rectangle, carriage
		 * returns are inserted.
		 *
		 * If h > 0 and the bottom of the rectangle is reached, the string
		 * is truncated. If h = 0, the string is only truncated if it reaches
		 * the bottom of the console.
		 *
		 * @param coordinate Initial point upper-left of rectangle.
		 * @param size Size of rectangle, where the text is rendering.
		 * @param flag this flag defines how the cell's background color is modified.
		 * @param text Text to draw in the area of rectangle.
		 */
		void writeText(const Geometry::Point2D<>& coordinate, const Geometry::Size& size,
				BackgroundFlag flag, std::string_view text);


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
		static void draw();

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
		static Key getKeyPressed();

		static Mouse getMouseEvent();

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
		 * @brief Blitting a console on another one
		 *
		 * This function allows you to blit a rectangular area of the source console <br>
		 * at a specific position on a destination console. It can also simulate alpha <br>
		 * transparency with the fade parameter.
		 *
         * @param source The rectangular area of the source console that will be blitted.
         *  If wSrc and/or hSrc == 0, the source console width/height are used
         * @param destination The source console that must be blitted on another one.
         * @param dest Where to blit the upper-left corner of the source area in the destination console.
         * @param foregroundAlpha Alpha transparency of the blitted console.
         * @param backgroundAlpha Alpha transparency of the blitted console.
         *
         *  0.0 => The source console is completely transparent. This function does nothing. <br>
         *  1.0 => The source console is opaque. Its cells replace the destination cells. <br>
         *  0 < fade < 1.0 => The source console is partially blitted, simulating real transparency. <br>
         *
         *  @example
         *  @code
         *  // Cross-fading between two offscreen consoles.
         *  // We use two offscreen consoles with
         *  // the same size as the root console.
         *  // We render a different screen on each offscreen console.
         *  // When the user hits a key, we do a cross-fading from the
         *  // first screen to the second screen.
         *
         *  Console off1 {80, 50};
         *  Console off2 {20, 30};
         *
         *  ... print screen1 on off1
         *  ... print screen2 of off2
         *
         *  // render screen1 in the game window
         *  off1.blit({0, 0}, off2, {0,0});
         *  @endcode
         */
		void blit(const Geometry::Point2D<>& source, Console& destination,
				const Geometry::Point2D<>& dest, float foregroundAlpha = 1.0f, float backgroundAlpha = 1.0f);


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

		void showCursor(bool show);

		/**
         * @brief  Allows you to limit the number of frames per second.
         *
         * If a frame is rendered faster than expected, the Console::flush
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
		void setFramePerSeconds(std::uint8_t _fps);

		/**
         * @brief Get the duration of the last frame.
         *
         * This function returns the length in seconds of the last rendered frame.
         *
         * @note You can use this value to update every time dependent object in the world.
         *
         * @return
         */
		float getLastFrameLength() const;

		/**
		 * Returns the number of seconds since the program has started.
		 *
		 * @return number of seconds since the program has started.
		 */
		float getElapsedSeconds() const;

		/**
         * @brief Get the number of frames rendered during the last second.
         *
         * @note The value returned by this function is updated every second.
         *
         * @return Number of frames rendered during the las second.
         */
		unsigned int getFramePerSeconds() const;

		/**
		 * @brief Get global timer in milliseconds.
		 *
		 * Returns the number of milliseconds since the program has started.
		 *
		 * @return number of milliseconds since the program has started.
		 */
		std::uint32_t getElapsedMilliseconds() const;

		Geometry::Size getFontSize() const;

		void unregisterCallback() const;

		/**
		 * @brief Draw custom graphics on top of the root console
		 *
		 * You can register a callback that will be called after the Doryen
		 * rendering phase, but before the screen buffer is swapped.
		 *
		 * This callback receives the screen SDL_Surface reference.
		 *
		 * This makes it possible to use any SDL drawing functions (including
		 * OpenGL) on top of the Doryen console.
		 *
		 * @note To disable the custom renderer, call the same method with a
		 * NULL parameter. Note that to keep Doryen from requiring the SDL headers,
		 * the callback parameter is a void pointer.
		 *
		 * You have to include SDL headers and cast it to SDL_Surface in your code.
		 *
		 * @param _render
		 */
		void registerCallback(std::unique_ptr<CallbackRender> _render) const;

	private:

		void drawFrame(const Geometry::Point2D<>& start,
				const Geometry::Point2D<>& end, bool empty, BackgroundFlag flag);

		/**
		 * Draw the tittle for a frame.
		 *
		 * @note Preferably the length of title should be lesser than
		 *  length of frame.
		 *
		 * @param _title The title to draw.
		 * @param _start Coordinate in x and y of frame relative to console
		 */
		void drawFrameTitle(std::string_view _title, const Geometry::Point2D<>& _start,
				const std::uint16_t lengthFrame);

	};
}


#endif

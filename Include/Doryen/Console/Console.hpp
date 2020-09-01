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

#include "Doryen/Graphics/Color/Palette.hpp"
#include "Doryen/Graphics/Color/Color.hpp"
#include "Doryen/Renderer/Renderer.hpp"
#include "Doryen/Geometry/Size.hpp"
#include "Doryen/Renderer/Sdl.hpp"

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
	 *
	 * The keyboard handling functions allow you to get keyboard input from the
	 * user, either for turn by turn games (the function wait until the user
	 * press a key), or real time games (non blocking function).
	 *
	 * @note for proper redraw event handling, the keyboard functions should
	 *  always be called just after draw the console.
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
		 *  Console height (in characters,not pixels).
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

		/**
		 * Current console.
		 */
		std::vector<Char> buffer;

		/**
		 * Console for last frame.
		 */
		std::vector<Char> oldBuffer;

		// Methods

		/**
         * Creating the game window.
         *
         * The default font in Doryen (./Terminal.png) uses 8x8 pixels characters.
         */
		void setConsoleModeMain();

		/**
		 * Draw the rectangle into of area.
		 *
		 * @param start Point initial of rectangle.
		 * @param end Point end of rectangle.
		 * @param empty if true, all characters inside the rectangle are set to ASCII code 32 (space).
		 * @param flag this flag defines how the cell's background color is modified.
		 */
		void drawFrame(const Geometry::Point2D<>& start,
				const Geometry::Point2D<>& end, bool empty, BlendModes flag);

		/**
		 * @brief Drawing an horizontal line
		 *
		 * Draws an horizontal line in the console, using ASCII code
		 * TCOD_CHAR_HLINE (196), and the console's default background
		 * and foreground colors.
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
		 * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param l The length of the line in cells 1 <= l <= console width - x
		 * @param flag this flag defines how the cell's background color is modified.
		 */
		void drawHorizontalLine(int x, int y, int l, BlendModes flag = BlendModes::DEFAULT);

		/**
		 * @brief Drawing an vertical line
		 *
		 * Draws an vertical line in the console, using ASCII code
		 * TCOD_CHAR_VLINE (179), and the console's default background
		 * and foreground colors.
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
		 * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param l The length of the line in cells 1 <= l <= console width - x
		 * @param flag this flag defines how the cell's background color is modified.
		 */
		void drawVerticalLine(int x, int y, int l, BlendModes flag = BlendModes::DEFAULT);

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

	public :

		/**
		 * Create an console.
		 *
		 * @note The first console instanced is the main console.
		 * @note The size of offscreen console is 80 x 50 cells for default.
		 */
		Console();


		/**
		 * Create an console.
		 *
		 * @note The first console instanced is the main console.
		 *
		 * @param w width of console. w > 0.
		 * @param h height of console. h > 0.
		 */
        Console( int w, int h );

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
		void setCustomFont(const char* fontFile, int flags = 1, int nbCharHoriz = 0,
				int nbCharVertic = 0);

		/**
		 * When you start the program, this returns true. Once a "close window"
		 * event has been sent by the window manager, it will allways return
		 * false. You're supposed to exit cleanly the game.
		 *
		 * @return false if the user not want exit the app, true otherwise.
		 */
		bool isRunning();

        /**
         * Modifies all cells of a console :
         *
         * 1- Set the cell's background color to the console default background color.
         * 2- Set the cell's foreground color to the console default foreground color.
         * 3- Set the cell's ASCII code to 32 (space).
         */
        void clear( );

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
		void writeChar(int x, int y, int c, BlendModes flag = BlendModes::NONE);

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
		void writeChar(int x, int y, int c, const Color& foreground, const Color& background, BlendModes flag);

		/**
		 * Write a string in the console. You can use the function format for formatter
		 * the string before of that it will be wrote in the console.
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
         * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param fmt String to write in the console.
		 */
		void writeString(int x, int y, const std::string& fmt);

		/**
		 * @brief Printing a string with specific alignment and background
		 * mode and auto-wrap.
		 *
		 * This function draws a string in a rectangle inside the console,
		 * using default colors, but specific alignment and background mode.
		 *
		 * If the string reaches the borders of the rectangle, carriage
		 * returns are inserted.
		 *
		 * If h > 0 and the bottom of the rectangle is reached, the string
		 * is truncated. If h = 0, the string is only truncated if it
		 * reaches the bottom of the console.
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
         * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param w Width of rectangle in the console. x <= x + w < console width.
		 * @param h Height of rectangle in the console. y <= y + h < console height.
		 * @param flag this flag defines how the cell's background color is modified.
		 * @param alignment defines how the strings are printed on screen.
		 *
		 * @param fmt printf-like format string, eventually followed by parameters.
		 *  You can use control codes to change the colors inside the string
		 *
		 * @param ... Extra parameters
		 *
		 * @return The function returns the height (number of console lines) of
		 *  the printed string.
		 */
		int writeWrapText(int x, int y, int w, int h, BlendModes flag, int alignment, const char* fmt, ...);

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
		void writeWrapText(const Geometry::Point2D<>& coordinate, const Geometry::Size& size,
				BlendModes flag, std::string_view text);

		/**
		 * @brief Filling a rectangle with the background color
		 *
		 * Fill a rectangle inside a console. For each cell in the rectangle :
		 *
		 * @li set the cell's background color to the console default background color
		 * @li set the cell's background color to the console default background color
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
         * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param rw Width of rectangle in the console. x <= x + w < console width.
		 * @param rh Height of rectangle in the console. y <= y + h < console height
		 * @param clear if true, all characters inside the rectangle are set to ASCII code 32 (space).
		 * @param flag this flag defines how the cell's background color is modified.
		 */
		void drawFillRect(int x, int y, int rw, int rh, bool clear, BlendModes flag = BlendModes::DEFAULT);

		/**
		 * @brief Drawing a window frame
		 *
		 * This function calls the rect function using the supplied background
		 * mode flag, then draws a rectangle with the console's default
		 * foreground color. If fmt is not NULL, it is printed on the top of
		 * the rectangle, using inverted colors.
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
         * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param w Width of rectangle in the console. x <= x + w < console width.
		 * @param h Height of rectangle in the console. y <= y + h < console height.
		 *
		 * @param clear  if true, all characters inside the rectangle are set to ASCII code 32 (space).
		 *  If false, only the background color is modified
		 *
		 * @param flag this flag defines how the cell's background color is modified.
		 * @param name if empty, the function only draws a rectangle.
		 */
		void drawWindowFrame(int x, int y, int w, int h, bool clear = true, BlendModes flag = BlendModes::DEFAULT,
				const std::string& name = "");

		/**
		 * Once the root console is initialized, you can use one of the
		 * printing functions to change the background colors, the foreground
		 * colors or the ASCII characters on the console.
		 *
		 * Once you've finished rendering the root console, you have to
		 * actually apply the updates to the screen with this function.
		 */
		void draw();

		/**
		 * Waits for the user to press a key.
		 *
		 * @param _key Key that the user is expected to press.
		 * @return True if the user has been pressed the key, otherwise false.
		 */
		bool isKeyPressed(KeyCode _key) const;

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

		void showCursor(bool show);

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

		// Getters

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
		 * @return the current fade amount, previously defined by setFade.
		 */
		short getFade() const;

		/**
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
		 * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @return the ASCII code of a cell.
		 */
		int getCellCode(int x, int y) const;

		/**
         * @return the width of a console in cells.
         */
		std::uint32_t getWidth() const;

		/**
		 * @return height of a console in cells.
		 */
		std::uint32_t getHeight() const;

		/**
         * @brief Get the number of frames rendered during the last second.
         *
         * @note The value returned by this function is updated every second.
         *
         * @return Number of frames rendered during the las second.
         */
		std::uint32_t getFramePerSeconds() const;

		/**
		 * @brief Get global timer in milliseconds.
		 *
		 * Returns the number of milliseconds since the program has started.
		 *
		 * @return number of milliseconds since the program has started.
		 */
		std::uint32_t getElapsedMilliseconds() const;

		/**
		 * @return The Key pressed for the user, if not key pressed, return
		 *  Key generic set to None.
		 */
		const Key& getKeyPressed() const;

		/**
		 * @return The mouse event pressed for the user, if not mouse events,
		 *  return a generic Mouse set to None.
		 */
		const Mouse& getMouseEvent() const;

		/**
		 * @return The size in pixels of each glyph of font used.
		 */
		const Geometry::Size getFontSize() const;

		/**
		 * @return the current fading color, previously defined by setFade.
		 */
		const Color& getFadingColor() const;

		/**
		 * @return the default background color of a console.
		 */
		const Color& getBackgroundColor() const;

		/**
		 * @return the default foreground color of a console.
		 */
		const Color& getForegroundColor() const;

		/**
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
		 * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @return the background color of a cell.
		 */
		const Color& getCellBackgroundColor(int x, int y) const;

		/**
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
		 * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @return the foreground color of a cell.
		 */
		const Color& getCellForegroundColor(int x, int y) const;

		// Setters

		/**
		 * Force the re-draw of cells that is into of rectangle.
		 */
		void setDirty(int x, int y, int w, int h);

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
		 * Dynamically changes the title of the game window.
		 *
		 * @note Note that the window title is not visible while in fullscreen.
		 *
		 * @param _title New title of the game window.
		 */
		void setWindowTitle(const std::string& _title);

		/**
         * @brief Use these functions to easily fade to/from a color.
         *
         * This function defines the fading parameters, allowing to easily fade
         * the game screen to/from a color. Once they are defined, the fading
         * parameters are valid for ever. You don't have to call setFade for
         * each rendered frame (unless you change the fading parameters).
         *
         * @param val the fading amount. 0 => the screen is filled with the
         *  fading color. 255 => no fading effect.
         *
         * @param fade the color to use during the console flushing operation.
         *
         * @example
         * @code
         *  for (int fade = 255; fade >= 0; --fade) {
         *  	console.setFade(fade, Palette::Red);
         *  	console.draw();
         *  }
         *  @endcode
         */
		void setFade(short val, const Doryen::Color& fade);

		/**
		 * @brief Setting the default background color.
		 *
		 * Changes the default background color for a console. The default
		 * background color is used by several drawing functions like
		 * clear, putChar, ...
		 *
		 * @param back the new default background color for this console.
		 */
		void setBackgroundColor(const Color& back);

		/**
		 * Changes the default foreground color for a console.
		 *
		 * The default foreground color is used by several drawing function
		 * like clear, putChar, ...
		 *
		 * @param fore the new default foreground color for this console.
		 */
		void setForegroundColor(const Color& fore);

		/**
		 * @brief Setting the background color of a cell.
		 *
		 * This function modifies the foreground color of a cell, leaving other
		 * properties (background color and ASCII code) unchanged.
		 *
		 * @param x coordinates in x of the cell in the console. 0 <= x <= console width.
         * @param y coordinates in y of the cell in the console. 0 <= y <= console height.
		 * @param col the foreground color to use. You can use color constants.
		 */
		void setCellForegroundColor(int x, int y, const Doryen::Color& col);

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
		void setCellBackgroundColor(int x, int y, const Doryen::Color& col, BlendModes flag = BlendModes::SET);

	};
}


#endif

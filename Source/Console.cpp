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

#include <cstdio>
#include <cstring>
#include <iostream>

#include "Doryen/Console.hpp"
#include "Doryen/libtcod.hpp"
#include "Doryen/libtcod_int.h"

using namespace Doryen;

// Static Members

Doryen::Renderer* Doryen::Console::renderer = new SDL();

Doryen::Console* Doryen::Console::root = nullptr;

// Constructs

Doryen::Console::Console()
{
	width = 80;
	height = 25;

	buffer.resize(width * height);
	oldBuffer.resize(width * height);

	windowClose = false;

	for (int i = 0; i < TCOD_COLCTRL_NUMBER; i++)
	{
		controlBackground[i] = Doryen::Color(0, 0, 0); // Black
		controlForeground[i] = Doryen::Color(255, 255, 255); // White
	}
}

Doryen::Console::Console(int w, int h)
{
	if (w > 0 && h > 0)
	{
		width = w;
		height = h;

		buffer.resize(width * height);
		oldBuffer.resize(width * height);

		windowClose = false;

		for (int i = 0; i < TCOD_COLCTRL_NUMBER; i++)
		{
			controlBackground[i] = Doryen::Color(0, 0, 0); // Black
			controlForeground[i] = Doryen::Color(255, 255, 255); // White
		}
	}
	else
	{
		// Throw Error
	}
}

Doryen::Console::Console(const char* filename)
{
	if (filename == nullptr)
	{
		// Throw Error
	}

	FILE* file;

	file = fopen(filename, "rb");

	if (file == nullptr)
	{
		// Throw Error
	}

	float version;

	fscanf(file, "Dorpy Version: %g", &version);

	int width;
	int height;

	fscanf(file, "%i %i", &width, &height);

	if (width < 0 || height < 0)
	{
		// Throw Error
	}

	TCOD_console_data_t* console = new TCOD_console_data_t;

	console->w = width;
	console->h = height;

	console->fore = TCOD_white;
	console->back = TCOD_black;
	console->fade = 255;
	console->buf = new char_t[console->w * console->h];
	console->oldbuf = new char_t[console->w * console->h];
	console->bkgnd_flag = TCOD_BKGND_NONE;
	console->alignment = TCOD_LEFT;

	windowClose = false;

	for (int i = 0; i < TCOD_COLCTRL_NUMBER; i++)
	{
		controlBackground[i] = Doryen::Color(0, 0, 0); // Black
		controlForeground[i] = Doryen::Color(255, 255, 255); // White
	}

	if (strstr(filename, ".asc"))
	{
		while (fgetc(file) != '#')
		{
			for (int x = 0; x < width; x++)
			{
				for (int y = 0; y < height; y++)
				{
					TCOD_color_t foreground;
					TCOD_color_t background;

					int c = fgetc(file);

					foreground.r = fgetc(file);
					foreground.g = fgetc(file);
					foreground.b = fgetc(file);

					background.r = fgetc(file);
					background.g = fgetc(file);
					background.b = fgetc(file);

					// Skip solid/walkable info
					if (version >= 0.3f)
					{
						fgetc(file);
						fgetc(file);
					}

					if (c < 0 || c > TCOD_ctx.max_font_chars)
					{
						// Throw Error
					}

					int offset = y * console->w + x;

					console->buf[offset].c = c;
					console->buf[offset].cf = TCOD_ctx.ascii_to_tcod[c];
					console->buf[offset].fore = foreground;
					console->buf[offset].back = background;
				}
			}
		}
	}
	else
	{
		// Not implement.
	}

	data = console;

	fclose(file);
}

Doryen::Console::~Console()
{
	if (isConsoleRoot)
	{
		renderer->onExit();

		delete renderer;
	}
}

void Doryen::Console::initRoot(int w, int h, const char* title, bool _fullscreen, TCOD_renderer_t _renderer)
{
	if (w > 0 && h > 0)
	{
		// Only exits a console root
		// during all the life cycle program.
		isConsoleRoot = true;

		// The method not is static and as
		// future plan, is convert this method
		// as a construct for initialize the
		// first console with root.

		// Actually, the method blit is free of
		// bugs caused for initialize of console
		// with this method, but in the past, the
		// method blit try use the buffer and
		// oldBuffer without methods getters and
		// setters and it threw out_of_range
		// exceptions because the method this
		// method [initRoot] not reinitialized the
		// width and height of console.

		// Reinitialize the size of console
		width = w;
		height = h;

		// Reinitialize the size of buffers
		buffer.resize(width * height);
		oldBuffer.resize(width * height);

		renderer->setWidth(w);
		renderer->setHeigth(h);
		renderer->setFullscreen(_fullscreen);
		renderer->setFade(255);

		renderer->onRenderer();

		renderer->createBuffer();
		renderer->clearBuffer();
		renderer->setWindowTitle(title);
	}
	else
	{
		// Throw Error
	}
}

void Doryen::Console::setCustomFont(const char* fontFile, int flags, int nbCharHoriz, int nbCharVertic)
{
	renderer->setFontfile(fontFile);

	// if layout not defined, assume ASCII_INCOL
	if (flags == 0 || flags == TCOD_FONT_TYPE_GREYSCALE)
	{
		flags |= TCOD_FONT_LAYOUT_ASCII_INCOL;
	}

	renderer->setFontHasRowLayout(((flags & TCOD_FONT_LAYOUT_ASCII_INROW) != 0));
	renderer->setFontGrayscale((flags & TCOD_FONT_TYPE_GREYSCALE) != 0);
	renderer->setFontHasDoryenLayout((flags & TCOD_FONT_LAYOUT_TCOD) != 0);

	if (nbCharHoriz > 0)
	{
		renderer->setFontCharHorizontalSize(nbCharHoriz);
		renderer->setFontCharVerticalSize(nbCharVertic);
	}
	else
	{
		if ((flags & TCOD_FONT_LAYOUT_ASCII_INROW) || (flags & TCOD_FONT_LAYOUT_ASCII_INCOL))
		{
			renderer->setFontCharHorizontalSize(16);
			renderer->setFontCharVerticalSize(16);
		}
		else
		{
			renderer->setFontCharHorizontalSize(32);
			renderer->setFontCharVerticalSize(8);
		}
	}

	if (renderer->isFontHasDoryenLayout())
	{
		renderer->setFontHasRowLayout(true);
	}

	if (renderer->getFontCharHorizontalSize() * renderer->getFontCharVerticalSize() != renderer->getMaxFontChars())
	{
		renderer->setMaxFontChars(renderer->getFontCharHorizontalSize() * renderer->getFontCharVerticalSize());

		renderer->checkTableOfCharacteres();
	}
}

void Doryen::Console::setDirty(int x, int y, int w, int h)
{
//	TCOD_console_set_dirty(x, y, w, h);

	if (x < 0 || y < 0 || w < 0 || h < 0)
	{
		// Throw error
		return;
	}
	else
	{
		using Point = Geometry::Point2D<>;

		// Initial point
		Point start = Point(x, y);

		// End Point
		Point end = Point(w, h);

		renderer->setDirty(start, end);
	}
}

Doryen::Key Doryen::Console::getKeyPressed()
{
	return renderer->getKeyPressed();
}

bool Doryen::Console::isKeyPressed(KeyCode _key)
{
	return renderer->getKeyPressed().getKeyCode() == _key;
}

bool Doryen::Console::isRunning()
{
	return renderer->isRunning();
}

unsigned int Doryen::Console::getWidth() const
{
	if (isConsoleRoot)
	{
		return renderer->getWidth();
	}
	else
	{
		return width;
	}
}

unsigned int Doryen::Console::getHeight() const
{
	if (isConsoleRoot)
	{
		return renderer->getHeigth();
	}
	else
	{
		return height;
	}
}

const Doryen::Color& Doryen::Console::getDefaultForeground() const
{
	if (isConsoleRoot)
	{
		return renderer->getForeground();
	}
	else
	{
		return foreground;
	}
}

const Doryen::Color& Doryen::Console::getDefaultBackground() const
{
	if (isConsoleRoot)
	{
		return renderer->getBackground();
	}
	else
	{
		return background;
	}
}

void Doryen::Console::setDefaultBackground(const Color& back)
{
	if (isConsoleRoot)
	{
		renderer->setBackground(back);
	}
	else
	{
		this->background = back;
	}
}

void Doryen::Console::setDefaultForeground(const Color& fore)
{
	if (isConsoleRoot)
	{
		renderer->setForeground(fore);
	}
	else
	{
		this->foreground = fore;
	}
}

void Doryen::Console::setWindowTitle(const std::string& _title)
{
	renderer->setWindowTitle(_title);
}

void Doryen::Console::setWindowInFullscreen()
{
	renderer->setWindowInFullscreen();
}

bool Doryen::Console::isFullscreen()
{
	return renderer->isFullscreen();
}

void Doryen::Console::setAlignment(TCOD_alignment_t alignment)
{
	//TCOD_console_set_alignment(data, alignment);
}

void Doryen::Console::blit(const Doryen::Console* srcCon, int xSrc, int ySrc, int wSrc, int hSrc,
		Doryen::Console* dstCon, int xDst, int yDst, float foreground_alpha,
		float background_alpha)
{
	TCOD_console_blit(srcCon->data, xSrc, ySrc, wSrc, hSrc, dstCon->data, xDst, yDst, foreground_alpha,
			background_alpha);
}


void Doryen::Console::flush()
{
	renderer->draw();
}

void Doryen::Console::setFade(short val, const Doryen::Color& fade)
{
	renderer->setFade(val);
	renderer->setFadingColor(fade);
}

short Doryen::Console::getFade()
{
	return renderer->getFade();
}

Doryen::Color Doryen::Console::getFadingColor()
{
	return renderer->getFadingColor();
}

void Doryen::Console::putChar(int x, int y, int c, BackgroundFlag flag)
{
	// Asserts
	if (x < 0 || y < 0 || c < 0 || c > renderer->getMaxFontChars())
	{
		// Throw Error
		return;
	}

	if (isConsoleRoot)
	{
		// Asserts
		if (x > renderer->getWidth() || y > renderer->getHeigth())
		{
			// Throw Error
			return;
		}
		else
		{
			unsigned offset = y * renderer->getWidth() + x;

			Char _char = Char();

			_char.setC(c);
			_char.setCf(renderer->getCharacterInLayoutCharacteres(c));
			_char.setForeground(renderer->getForeground());

			Color b = renderer->getBackgroundOfCharacterInBufferAt(offset);
			b.trasformColor(renderer->getBackground(), flag);

			_char.setBackground(b);

			renderer->setCharacterInBufferAt(offset, _char);
		}
	}
	else
	{
		// Asserts
		if (x > width || y > height)
		{
			// Throw Error
			return;
		}
		else
		{
			unsigned offset = y * width + x;

			Char _char = Char();

			_char.setC(c);
			_char.setCf(renderer->getCharacterInLayoutCharacteres(c));
			_char.setForeground(foreground);

			Color b = buffer[offset].getBackground();
			b.trasformColor(background, flag);

			_char.setBackground(b);

			buffer[offset] = _char;
		}
	}
}

void Doryen::Console::putCharEx(int x, int y, int c, const Doryen::Color& fore, const Doryen::Color& back)
{
	TCOD_color_t f = { fore.r, fore.g, fore.b };
	TCOD_color_t b = { back.r, back.g, back.b };
	TCOD_console_put_char_ex(data, x, y, c, f, b);
}

void Doryen::Console::clear()
{
	if (isConsoleRoot)
	{
		renderer->clearBuffer();
	}
	else
	{
		for (Char& c: buffer)
		{
			c.setC(' ');
			c.setCf(renderer->getCharacterInLayoutCharacteres(' '));
			c.setForeground(foreground);
			c.setBackground(background);
			c.setDirt(false);
		}
	}
}

Doryen::Color Doryen::Console::getCharBackground(int x, int y) const
{
	// Asserts
	if (x < 0 || y < 0)
	{
		// Throw Error
		throw "ExceptionIllegalArgument";
	}

	if (isConsoleRoot)
	{
		unsigned index = x + renderer->getWidth() * y;

		return renderer->getBackgroundOfCharacterInBufferAt(index);
	}
	else
	{
		unsigned index = x + width * y;

		return buffer[index].getBackground();
	}
}

void Doryen::Console::setCharForeground(int x, int y, const Doryen::Color& col)
{
	// Asserts
	if (x < 0 || y < 0)
	{
		// Throw Error
		throw "ExceptionIllegalArgument";
	}

	if (isConsoleRoot)
	{
		unsigned index = x + renderer->getWidth() * y;

		renderer->setForegroundOfCharacterInBufferAt(index, col);
	}
	else
	{
		unsigned index = x + width * y;

		buffer[index].setForeground(col);
	}
}

Doryen::Color Doryen::Console::getCharForeground(int x, int y) const
{
	// Asserts
	if (x < 0 || y < 0)
	{
		// Throw Error
		throw "ExceptionIllegalArgument";
	}

	if (isConsoleRoot)
	{
		unsigned index = x + renderer->getWidth() * y;

		return renderer->getForegroundOfCharacterInBufferAt(index);
	}
	else
	{
		unsigned index = x + width * y;

		return buffer[index].getForeground();
	}
}

int Doryen::Console::getChar(int x, int y) const
{
	// Asserts
	if (x < 0 || y < 0)
	{
		// Throw Error
		throw "ExceptionIllegalArgument";
	}

	if (isConsoleRoot)
	{
		unsigned offset = x + renderer->getWidth() * y;

		return renderer->getCharOfCharacterInBufferAt(offset);
	}
	else
	{
		unsigned offset = x + width * y;

		return buffer[offset].getC();
	}
}

void Doryen::Console::setCharBackground(int x, int y, const Doryen::Color& col, Doryen::BackgroundFlag flag)
{
	// Asserts
	if (x < 0 || y < 0)
	{
		// Throw Error
		return;
	}

	if (isConsoleRoot)
	{
		// Asserts
		if (x > renderer->getWidth() || y > renderer->getHeigth())
		{
			// Throw Error
			return;
		}
		else
		{
			unsigned index = x + renderer->getWidth() * y;

			Color b = renderer->getBackgroundOfCharacterInBufferAt(index);

			b.trasformColor(col, flag);

			renderer->setBackgroundOfCharacterInBufferAt(index, b);
		}
	}
	else
	{
		// Asserts
		if (x > width || y > height)
		{
			// Throw Error
			return;
		}
		else
		{
			Color b = buffer[x + width * y].getBackground();

			b.trasformColor(col, flag);

			buffer[x + width * y].setBackground(b);
		}
	}
}

void Doryen::Console::setChar(int x, int y, int c)
{
	// Asserts
	if (x < 0 || y < 0)
	{
		// Throw Error
		throw "ExceptionIllegalArgument";
	}

	if (isConsoleRoot)
	{
		unsigned offset = x + renderer->getWidth() * y;

		renderer->setCharOfCharacterInBufferAt(offset, c);
	}
	else
	{
		unsigned offset = x + width * y;

		buffer[offset].setC(c);
		buffer[offset].setCf(renderer->getCharacterInLayoutCharacteres(c));
	}
}

using Point = Doryen::Geometry::Point2D<>;

// Private function
void consoleClamp(const Point& start, const Point& end, Point& first, Point& second)
{
	if (first.x + second.x > end.x)
	{
		second.x = end.x - first.x;
	}

	if (first.y + second.y > end.y)
	{
		second.y = end.y - first.y;
	}

	if (first.x < start.x)
	{
		second.x = second.x - start.x - first.x;
		first.x = start.x;
	}

	if (first.y < start.y)
	{
		second.y = second.y - start.y - first.y;
		first.y = start.y;
	}
}

void Doryen::Console::rect(int x, int y, int rw, int rh, bool clear, BackgroundFlag flag)
{
	// Asserts
	if (x < 0 || y < 0 || rw < 0 || rh < 0)
	{
		throw "ExceptionIllegalArgument";
	}

	if (isConsoleRoot)
	{
		// Asserts
		if (x > renderer->getWidth() || y > renderer->getHeigth() ||
			x + rw > renderer->getWidth() || y + rh > renderer->getHeigth())
		{
			throw "ExceptionIllegalArgument";
		}
		else
		{
			Point start = Point(0, 0);
			Point end = Point((int)renderer->getWidth(), (int)renderer->getHeigth());

			Point first = Point(x, y);
			Point second = Point(rw, rh);

			// Remember, pass for reference
			consoleClamp(start, end, first, second);

			for (int cx = first.x; cx < first.x + second.x; ++cx)
			{
				for (int cy = first.y; cy < first.y + second.y; ++cy)
				{
					setCharBackground(cx, cy, renderer->getBackground(), flag);

					if (clear)
					{
						unsigned index = cx + renderer->getWidth() * cy;

						renderer->setCharOfCharacterInBufferAt(index, ' ');
					}
				}
			}
		}
	}
	else
	{
		// Asserts
		if (x > width || y > height || x + rw > width || y + rh > height)
		{
			throw "ExceptionIllegalArgument";
		}
		else
		{
			Point start = Point(0, 0);
			Point end = Point((int)width, (int)height);

			Point first = Point(x, y);
			Point second = Point(rw, rh);

			// Remember, pass for reference
			consoleClamp(start, end, first, second);

			for (int cx = first.x; cx < first.x + second.x; ++cx)
			{
				for (int cy = first.y; cy < first.y + second.y; ++cy)
				{
					setCharBackground(cx, cy, background, flag);

					if (clear)
					{
						unsigned index = cx + width * cy;

						buffer[index].setC(' ');
						buffer[index].setCf(renderer->getCharacterInLayoutCharacteres(' '));
					}
				}
			}
		}
	}
}

void Doryen::Console::hline(int x, int y, int l, BackgroundFlag flag)
{
	for (int i = x; i < x + l; ++i)
	{
		// Character 196 ASCII, see table ASCII.
		putChar(i, y, 196, flag);
	}
}

void Doryen::Console::vline(int x, int y, int l, BackgroundFlag flag)
{
	for (int i = y; i < y + l; ++i)
	{
		// Character 179 ASCII, see table ASCII.
		putChar(x, i, 179, flag);
	}
}

void Doryen::Console::drawFrame(const Point& start, const Point& end, bool empty, BackgroundFlag flag)
{
	//Draw the edges
	putChar(start.x, start.y, 218, flag);
	putChar(start.x + end.x - 1, start.y, 191, flag);
	putChar(start.x, start.y + end.y - 1, 192, flag);
	putChar(start.x + end.x - 1, start.y + end.y - 1, 217, flag);

	// Draw the border of frame
	hline(start.x + 1, start.y, end.x - 2, flag);
	hline(start.x + 1, start.y + end.y - 1, end.x - 2, flag);

	if (end.y > 2)
	{
		// Draw the border of frame
		vline(start.x, start.y + 1, end.y - 2, flag);
		vline(start.x + end.x - 1, start.y + 1, end.y - 2, flag);

		if (empty)
		{
			rect(start.x + 1, start.y + 1, end.x - 2, end.y - 2, true, flag);
		}
	}
}

void Doryen::Console::drawFrameTitle(std::string_view _title,
		const Doryen::Geometry::Point2D<>& _start,
		const std::uint16_t lengthFrame)
{
	// Overflow caused for that the title is greater than length of frame
	if (_title.size() > lengthFrame)
	{
		print(_start.x, _start.y, _title.data());
		return;
	}

	// If not exit overflow, center the title
	std::uint16_t centerX = (lengthFrame - _title.size()) / 2;
	print(centerX, _start.y, _title.data());
}


void Doryen::Console::printFrame(int x, int y, int w, int h, bool clear,
		Doryen::BackgroundFlag flag, const std::string& name)
{
	// Curly braces for create a Point directly.
	drawFrame({ x, y }, { w, h }, clear, flag);
	drawFrameTitle(name, { x, y }, w);
}

void Doryen::Console::print(int x, int y, const std::string& fmt)
{
//	va_list ap;
//	TCOD_console_data_t* dat = (TCOD_console_data_t*)data;
//	TCOD_IFNOT (dat != NULL)
//	{ return; }
//	va_start(ap, fmt);
//	TCOD_console_print_internal(data, x, y, 0, 0, dat->bkgnd_flag, dat->alignment,
//			TCOD_console_vsprint(fmt, ap), false, false);
//	va_end(ap);

	for (int i = 0; i < fmt.size(); ++i)
	{
		putChar(x + i, y, fmt[i]);
	}
}

void Doryen::Console::printEx(int x, int y, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const char* fmt, ...)
{
//	va_list ap;
//	va_start(ap, fmt);
//	TCOD_console_print_internal(data, x, y, 0, 0, flag, alignment, TCOD_console_vsprint(fmt, ap), false, false);
//	va_end(ap);
}

int Doryen::Console::printRect(int x, int y, int w, int h, const char* fmt, ...)
{
	va_list ap;
	TCOD_console_data_t* dat = (TCOD_console_data_t*)data;
	TCOD_IFNOT (dat != NULL)
	{ return 0; }
	va_start(ap, fmt);
	int ret = TCOD_console_print_internal(data, x, y, w, h, dat->bkgnd_flag, dat->alignment,
			TCOD_console_vsprint(fmt, ap), true, false);
	va_end(ap);
	return ret;
}

int Doryen::Console::printRectEx(int x, int y, int w, int h, TCOD_bkgnd_flag_t flag,
		TCOD_alignment_t alignment, const char* fmt, ...)
{
//	va_list ap;
//	va_start(ap, fmt);
//	int ret = TCOD_console_print_internal(data, x, y, w, h, flag, alignment, TCOD_console_vsprint(fmt, ap), true,
//			false);
//	va_end(ap);
//	return ret;

	return 1;
}

std::vector<std::string> processCharacterNewLine(std::vector<std::string>&& _lines)
{
	std::vector<std::string> textFormatted;

	for (const std::string& line: _lines)
	{
		if (line[0] == '\n')
		{
			textFormatted.emplace_back("");
			textFormatted.emplace_back(line.substr(1));
		}
		else
		{
			textFormatted.emplace_back(line);
		}
	}

	return textFormatted;
}

std::vector<std::string> processEspecialCharacters(std::string&& _text)
{
	std::vector<std::string> lines;

	std::size_t lastCharacterProcessed = 0;
	std::size_t positionCharacterNewLine = _text.find('\n');

	while (positionCharacterNewLine not_eq std::string::npos)
	{
		std::string line = _text.substr(lastCharacterProcessed, positionCharacterNewLine);
		lines.emplace_back(line);
		// Skip the especial character (example: new line)
		lastCharacterProcessed = positionCharacterNewLine + 1;
		positionCharacterNewLine = _text.find('\n', lastCharacterProcessed + 1);
	}

	lines.emplace_back(_text.substr(lastCharacterProcessed));

	return processCharacterNewLine(std::move(lines));
}

/**
 * @param _text Reference to text (Warning, the parameter
 *  is modified in the function, pass for copy if no wanna
 *  that the information will be destroyed).
 *
 * @return List with all the word content in the text.
 */
std::vector<std::string> getAllWords(std::string&& _text)
{
	std::vector<std::string> words;

	std::size_t positionWhitespace;

	// The delimiter for word is a whitespace (Length of 1)
	while ((positionWhitespace = _text.find(' ')) not_eq std::string::npos)
	{
		words.emplace_back(_text.substr(0, positionWhitespace));
		// Deleted the word add to list more the delimiter (for it sum 1)
		_text.erase(0, positionWhitespace + 1);
	}

	return words;
}

std::vector<std::vector<std::string>> getAllWordsParagraph(std::vector<std::string>&& _lines)
{
	std::vector<std::vector<std::string>> paragraphs;

	std::vector<std::string> mergeLine;

	for (std::string& line : _lines)
	{
		if (line.empty())
		{
			paragraphs.emplace_back(mergeLine);
			mergeLine.clear();
		}
		else
		{
			std::vector<std::string> words = getAllWords(std::move(line));
			std::copy(words.begin(), words.end(), std::back_inserter(mergeLine));
		}
	}

	paragraphs.emplace_back(mergeLine);

	return paragraphs;
}

std::vector<std::string> wrapText(std::string_view _text, const std::uint16_t LINE_WIDTH)
{
	std::vector<std::string> textProcessed = processEspecialCharacters(std::string{ _text });

	std::vector<std::vector<std::string>> paragraphs = getAllWordsParagraph(std::move(textProcessed));

	// Pass a copy for parameters
	const std::vector<std::string> words = getAllWords(std::string{ _text });

	std::vector<std::string> wrapText{ 1 };

	// First iteration
	std::uint16_t spaceLeft = LINE_WIDTH;
	std::uint16_t currentIndex = 0;

	for (const std::string& word : words)
	{
		if (word.size() + 1 > spaceLeft)
		{
			// Insert a new line
			wrapText.emplace_back(word + " ");
			currentIndex += 1;
			spaceLeft = LINE_WIDTH - (word.size() + 1);
		}
		else
		{
			spaceLeft = spaceLeft - (word.size() + 1);
			wrapText[currentIndex] += word + " ";
		}
	}

	return wrapText;
}

void Doryen::Console::writeText(const Geometry::Point2D<>& coordinate,
		const Size& size, BackgroundFlag flag, std::string_view text)
{
	if (text.size() > size.w)
	{
		// Wrap the text
		const std::vector<std::string> lineTexts = wrapText(text, size.w);

		std::uint16_t currentY = coordinate.y;

		for (const std::string& _text : lineTexts)
		{
			print(coordinate.x, currentY, _text);
			currentY += 1;
		}
	}
	else
	{
		print(coordinate.x, coordinate.y, text.data());
	}
}

int Doryen::Console::getHeightRect(int x, int y, int w, int h, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int ret = TCOD_console_print_internal(data, x, y, w, h, TCOD_BKGND_NONE, TCOD_LEFT, TCOD_console_vsprint(fmt, ap),
			true, true);
	va_end(ap);
	return ret;
}

void Doryen::Console::setKeyboardRepeat(int initialDelay, int interval)
{
	TCOD_console_set_keyboard_repeat(initialDelay, interval);
}

void Doryen::Console::disableKeyboardRepeat()
{
	TCOD_console_disable_keyboard_repeat();
}

bool Doryen::Console::isKeyPressed(TCOD_keycode_t key)
{
	return TCOD_console_is_key_pressed(key) != 0;
}

void Doryen::Console::setKeyColor(const Doryen::Color& col)
{
	TCOD_color_t c = { col.r, col.g, col.b };
	TCOD_console_set_key_color(data, c);
}

void Doryen::Console::credits()
{
	TCOD_console_credits();
}

void Doryen::Console::resetCredits()
{
	TCOD_console_credits_reset();
}

bool Doryen::Console::renderCredits(int x, int y, bool alpha)
{
	//return TCOD_console_credits_render(x, y, alpha) != 0;

	return true;
}

float Doryen::Console::getLastFrameLength() const
{
	return renderer->getLastFrameLength();
}

Doryen::Mouse Doryen::Console::getMouseEvent()
{
	return renderer->getMouseEvent();
}

void
Doryen::Console::blit(const Doryen::Geometry::Point2D<>& source, Doryen::Console& destination,
		const Doryen::Geometry::Point2D<>& dest, float foregroundAlpha, float backgroundAlpha)
{
	for (int cx = source.x; cx < source.x + getWidth(); ++cx)
	{
		for (int cy = source.y; cy < source.y + getHeight(); ++cy)
		{
			if (cx >= getWidth() or cy >= getHeight()) continue;

			const Char srcChar = buffer[cy * width + cx];
			Char dstChar = srcChar;

			if (foregroundAlpha == 1.0f and backgroundAlpha == 1.0f)
			{
				dstChar = srcChar;
			}
			else
			{
				dstChar.setC(destination.getChar(dest.x, dest.y));
				dstChar.setBackground(destination.getCharBackground(dest.x, dest.y));
				dstChar.setForeground(destination.getCharForeground(dest.x, dest.y));

				dstChar.setBackground(Color::lerp(dstChar.getBackground(),
						srcChar.getBackground(), backgroundAlpha));

				if (srcChar.getC() == ' ')
				{
					dstChar.setForeground(Color::lerp(dstChar.getForeground(),
							srcChar.getBackground(), backgroundAlpha));
				}
				else if (dstChar.getC() == ' ')
				{
					dstChar.setC(srcChar.getC());
					dstChar.setCf(srcChar.getCf());
					dstChar.setForeground(Color::lerp(dstChar.getBackground(),
							srcChar.getForeground(), foregroundAlpha));
				}
				else if (dstChar.getC() == srcChar.getC())
				{
					dstChar.setForeground(Color::lerp(dstChar.getForeground(),
							srcChar.getForeground(), foregroundAlpha));
				}
				else
				{
					if (foregroundAlpha < 0.5f)
					{
						dstChar.setForeground(Color::lerp(dstChar.getForeground(),
								dstChar.getBackground(), foregroundAlpha * 2));
					}
					else
					{
						dstChar.setC(srcChar.getC());
						dstChar.setCf(srcChar.getCf());
						dstChar.setForeground(Color::lerp(dstChar.getBackground(),
								srcChar.getForeground(), (foregroundAlpha - 0.5f) * 2));
					}
				}
			}

			const int dx = cx - source.x + dest.x;
			const int dy = cy - source.y + dest.y;

			if (dx >= destination.getWidth() or dy >= destination.getHeight()) continue;

			// See the documentation internal of initRoot method for see details of an
			// buf produce for this method [blit]
			// Actually, this method [blit] is free of bugs
			destination.setChar(dx, dy, dstChar.getC());
			destination.setCharForeground(dx, dy, dstChar.getForeground());
			destination.setCharBackground(dx, dy, dstChar.getBackground());
		}
	}
}

unsigned int Doryen::Console::getFramePerSeconds() const
{
	return renderer->getFramePerSeconds();
}

void Doryen::Console::setFramePerSeconds(std::uint8_t _fps)
{
	// For avoid the division zero error
	if (_fps == 0)
	{
		renderer->setMinimumFrameLength(0);
		return;
	}

	renderer->setMinimumFrameLength(1000 / _fps);
}

void Doryen::Console::showCursor(bool show)
{
	renderer->showCursor(show);
}

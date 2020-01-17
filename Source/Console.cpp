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

#include "Console.hpp"
#include "libtcod.hpp"
#include "libtcod_int.h"

// Static Members

Doryen::Renderer* Doryen::Console::renderer = new SDL();

Doryen::Console* Doryen::Console::root = nullptr;

// Constructs

Doryen::Console::Console()
{
	TCOD_console_data_t* console = new TCOD_console_data_t;

	console->w = 80;
	console->h = 25;

	console->fore = TCOD_white;
	console->back = TCOD_black;
	console->fade = 255;
	console->buf = new char_t[console->w * console->h];
	console->oldbuf = new char_t[console->w * console->h];
	console->bkgnd_flag = TCOD_BKGND_NONE;
	console->alignment = TCOD_LEFT;

	for (int j = 0; j < console->w * console->h; j++)
	{
		console->buf[j].c = ' ';
		console->buf[j].cf = -1;
	}

	windowClose = false;

	for (int i = 0; i < TCOD_COLCTRL_NUMBER; i++)
	{
		controlBackground[i] = Doryen::Color(0, 0, 0); // Black
		controlForeground[i] = Doryen::Color(255, 255, 255); // White
	}

	data = console;
}

Doryen::Console::Console(int w, int h)
{
	if (w > 0 && h > 0)
	{
		TCOD_console_data_t* console = new TCOD_console_data_t;

		console->w = w;
		console->h = h;

		console->fore = TCOD_white;
		console->back = TCOD_black;
		console->fade = 255;
		console->buf = new char_t[console->w * console->h];
		console->oldbuf = new char_t[console->w * console->h];
		console->bkgnd_flag = TCOD_BKGND_NONE;
		console->alignment = TCOD_LEFT;

		for (int j = 0; j < console->w * console->h; j++)
		{
			console->buf[j].c = ' ';
			console->buf[j].cf = -1;
		}

		windowClose = false;

		for (int i = 0; i < TCOD_COLCTRL_NUMBER; i++)
		{
			controlBackground[i] = Doryen::Color(0, 0, 0); // Black
			controlForeground[i] = Doryen::Color(255, 255, 255); // White
		}

		data = console;
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
	Doryen::Console* con = new Doryen::Console();

	if (w > 0 && h > 0)
	{
		TCOD_console_data_t* console = new TCOD_console_data_t;

		// Only exits a console root
		// during all the life cycle program.
		isConsoleRoot = true;

		console->w = w;
		console->h = h;

		renderer->setWidth(w);
		renderer->setHeigth(h);
		renderer->setFullscreen(_fullscreen);

		TCOD_ctx.root = console;
		TCOD_ctx.renderer = _renderer;

		console->fore = TCOD_white;
		console->back = TCOD_black;
		console->fade = 255;
		console->buf = new char_t[console->w * console->h];
		console->oldbuf = new char_t[console->w * console->h];
		console->bkgnd_flag = TCOD_BKGND_NONE;
		console->alignment = TCOD_LEFT;

		for (int j = 0; j < console->w * console->h; j++)
		{
			console->buf[j].c = ' ';
			console->buf[j].cf = -1;
		}

//		renderer->onRenderer();

		if (!TCOD_sys_init(console->w, console->h, console->buf,
				console->oldbuf, _fullscreen))
		{
			// Throw Error
		}

		renderer->setWindowTitle(title);

		con->data = console;

		root = con;
	}
	else
	{
		// Throw Error
	}
}

void Doryen::Console::setCustomFont(const char* fontFile, int flags, int nbCharHoriz, int nbCharVertic)
{
	std::strcpy(TCOD_ctx.font_file, fontFile);

	// if layout not defined, assume ASCII_INCOL
	if (flags == 0 || flags == TCOD_FONT_TYPE_GREYSCALE)
	{
		flags |= TCOD_FONT_LAYOUT_ASCII_INCOL;
	}

	TCOD_ctx.font_in_row = ((flags & TCOD_FONT_LAYOUT_ASCII_INROW) != 0);
	TCOD_ctx.font_greyscale = ((flags & TCOD_FONT_TYPE_GREYSCALE) != 0);
	TCOD_ctx.font_tcod_layout = ((flags & TCOD_FONT_LAYOUT_TCOD) != 0);

	if (nbCharHoriz > 0)
	{
		TCOD_ctx.fontNbCharHoriz = nbCharHoriz;
		TCOD_ctx.fontNbCharVertic = nbCharVertic;
	}
	else
	{
		if ((flags & TCOD_FONT_LAYOUT_ASCII_INROW) || (flags & TCOD_FONT_LAYOUT_ASCII_INCOL))
		{
			TCOD_ctx.fontNbCharHoriz = 16;
			TCOD_ctx.fontNbCharVertic = 16;
		}
		else
		{
			TCOD_ctx.fontNbCharHoriz = 32;
			TCOD_ctx.fontNbCharVertic = 8;
		}
	}

	if (TCOD_ctx.font_tcod_layout)
	{
		TCOD_ctx.font_in_row = true;
	}

	if (TCOD_ctx.fontNbCharHoriz * TCOD_ctx.fontNbCharVertic != TCOD_ctx.max_font_chars)
	{
		TCOD_ctx.max_font_chars = TCOD_ctx.fontNbCharHoriz * TCOD_ctx.fontNbCharVertic;

		delete[] TCOD_ctx.ascii_to_tcod;
		TCOD_ctx.ascii_to_tcod = new int[TCOD_ctx.max_font_chars];

		for (int i = 0; i < Renderer::layoutAsciiCode.size(); ++i)
		{
			TCOD_ctx.ascii_to_tcod[i] = Renderer::layoutAsciiCode[i];
		}

		//renderer->createTablesOfCharacteres(TCOD_ctx.max_font_chars);
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
		using Point = Math::Point2D;

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

bool Doryen::Console::isWindowClosed()
{
	return TCOD_console_is_window_closed() != 0;
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
	TCOD_console_set_alignment(data, alignment);
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
	TCOD_color_t temp = TCOD_console_get_char_foreground(data, x, y);
	return Doryen::Color(temp.r, temp.g, temp.b);
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
			unsigned index = x + renderer->getWidth() + y;

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
	TCOD_console_set_char(data, x, y, c);
}

void Doryen::Console::rect(int x, int y, int rw, int rh, bool clear, TCOD_bkgnd_flag_t flag)
{
	TCOD_console_rect(data, x, y, rw, rh, clear, flag);
}

void Doryen::Console::hline(int x, int y, int l, TCOD_bkgnd_flag_t flag)
{
	TCOD_console_hline(data, x, y, l, flag);
}

void Doryen::Console::vline(int x, int y, int l, TCOD_bkgnd_flag_t flag)
{
	TCOD_console_vline(data, x, y, l, flag);
}

void Doryen::Console::printFrame(int x, int y, int w, int h, bool empty, TCOD_bkgnd_flag_t flag, const char* fmt, ...)
{
	if (fmt)
	{
		va_list ap;
		va_start(ap, fmt);
		TCOD_console_print_frame(data, x, y, w, h, empty, flag, TCOD_console_vsprint(fmt, ap));
		va_end(ap);
	}
	else
	{
		TCOD_console_print_frame(data, x, y, w, h, empty, flag, NULL);
	}
}

void Doryen::Console::print(int x, int y, const char* fmt, ...)
{
	va_list ap;
	TCOD_console_data_t* dat = (TCOD_console_data_t*)data;
	TCOD_IFNOT (dat != NULL)
	{ return; }
	va_start(ap, fmt);
	TCOD_console_print_internal(data, x, y, 0, 0, dat->bkgnd_flag, dat->alignment,
			TCOD_console_vsprint(fmt, ap), false, false);
	va_end(ap);
}

void Doryen::Console::printEx(int x, int y, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	TCOD_console_print_internal(data, x, y, 0, 0, flag, alignment, TCOD_console_vsprint(fmt, ap), false, false);
	va_end(ap);
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
	va_list ap;
	va_start(ap, fmt);
	int ret = TCOD_console_print_internal(data, x, y, w, h, flag, alignment, TCOD_console_vsprint(fmt, ap), true,
			false);
	va_end(ap);
	return ret;
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
	return TCOD_console_credits_render(x, y, alpha) != 0;
}

#ifndef NO_UNICODE

// ctrl = TCOD_COLCTRL_1...TCOD_COLCTRL_5 or TCOD_COLCTRL_STOP
#define NB_BUFFERS 10

#endif



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

#include "Doryen/Console/Console.hpp"

using namespace Doryen;

// Initialization of static variable

/*
 * Injection of dependency based in definition of macros.
 * My point of view on the use of macros is: minimize the use of macros where
 * it is possible.
 *
 * This macro-based definition offers several advantages that i have not
 * achieved otherwise, including preventing user who include the Console.hpp
 * header from accessing rendering-specific code, the ultimate goal of this
 * maneuver is to prevent the user from using platform-specific code (which is
 * not very portable).
 */
#if defined(DORYEN_USE_SDL_1)

#include <Doryen/Renderer/Sdl.hpp>

// Inject the dependency
std::unique_ptr<Renderer> Console::renderer = std::make_unique<SDL>();

#endif

// Constructs

Doryen::Console::Console() : Console(80, 50)
{
	// Delegate the construct of console to another construct
}

Doryen::Console::Console(int w, int h)
{
	// New feature of Doryen Library
	// Previously, the Libtcod Library required that the main (aka. root)
	// console will be called with the method initRoot, that marked the
	// console as the main console.
	// Actually, the Doryen Library allow created a main console without
	// the needed of call to method initRoot (that now not exist), with
	// this new feature, too exist a rule.
	// Rule: The first console that is instanced is the main console
	static bool consoleRootCreated = false;

	if (w > 0 && h > 0)
	{
		width = w;
		height = h;

		front.resize(width * height);
		oldBuffer.resize(width * height);

		// Only exist a main console durant the life cycle of program.
		if (not consoleRootCreated)
		{
			setConsoleModeMain();
			// The first console instanced is the main console
			// set the variable static to false for avoid that
			// other console instanced will be marked like main
			// console.
			consoleRootCreated = true;
		}
	}
	else
	{
		// Throw Error
	}
}

void Doryen::Console::setConsoleModeMain()
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

	// Reinitialize the size of buffers
	front.resize(width * height);
	oldBuffer.resize(width * height);

	renderer->setWidth(width);
	renderer->setHeight(height);
	renderer->setFade(255);

	renderer->onRenderer();
}

void Doryen::Console::setCustomFont(const char* fontFile, int flags, int nbCharHoriz, int nbCharVertic)
{
	const std::uint8_t FONT_LAYOUT_ASCII_INCOL = 1;
	const std::uint8_t FONT_LAYOUT_ASCII_INROW = 2;
	const std::uint8_t FONT_TYPE_GREYSCALE = 4;
	const std::uint8_t FONT_TYPE_GRAYSCALE = 4;
	const std::uint8_t FONT_LAYOUT_TCOD = 8;

	renderer->setFontfile(fontFile);

	// if layout not defined, assume ASCII_INCOL
	if (flags == 0 || flags == FONT_TYPE_GREYSCALE)
	{
		flags |= FONT_LAYOUT_ASCII_INCOL;
	}

	renderer->setFontHasRowLayout(((flags & FONT_LAYOUT_ASCII_INROW) != 0));
	renderer->setFontGrayscale((flags & FONT_TYPE_GREYSCALE) != 0);
	renderer->setFontHasDoryenLayout((flags & FONT_LAYOUT_TCOD) != 0);

	if (nbCharHoriz > 0)
	{
		renderer->setFontCharHorizontalSize(nbCharHoriz);
		renderer->setFontCharVerticalSize(nbCharVertic);
	}
	else
	{
		if ((flags & FONT_LAYOUT_ASCII_INROW) || (flags & FONT_LAYOUT_ASCII_INCOL))
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

	renderer->loadFont();
}

bool Doryen::Console::isKeyPressed(KeyCode _key) const
{
	return renderer->getKeyPressed().getKeyCode() == _key;
}

bool Doryen::Console::isRunning()
{
	return renderer->isRunning();
}

void Doryen::Console::draw()
{
	renderer->draw();
}

void Doryen::Console::writeChar(int x, int y, int c, BlendModes flag)
{
	if (isConsoleRoot)
	{
		writeChar(x, y, c, renderer->getForeground(), renderer->getBackground(), flag);
	}
	else
	{
		writeChar(x, y, c, foreground, background, flag);
	}
}

void Doryen::Console::writeChar(int x, int y, int c, const Color& _foreground, const Color& _background)
{
	writeChar(x, y, c, _foreground, _background, BlendModes::SET);
}

void Console::writeChar(int x, int y, int c, const Color& _foreground, const Color& _background, BlendModes flag)
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
		if (x > renderer->getWidth() || y > renderer->getHeight())
		{
			// Throw Error
			return;
		}
		else
		{
			unsigned offset = y * renderer->getWidth() + x;

			Char _char = Char();

			_char.setCharacter(c);
			_char.setCharacterFont(renderer->getCharacterInLayoutCharacteres(c));
			_char.setForeground(_foreground);

			Color b = renderer->getBackgroundOfCharacterInBufferAt(offset);
			b.trasformColor(_background, flag);

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

			_char.setCharacter(c);
			_char.setCharacterFont(renderer->getCharacterInLayoutCharacteres(c));
			_char.setForeground(_foreground);

			Color b = front[offset].getBackground();
			b.trasformColor(_background, flag);

			_char.setBackground(b);

			front[offset] = _char;
		}
	}
}

void Doryen::Console::clear()
{
	if (isConsoleRoot)
	{
		renderer->clearBuffer();
	}
	else
	{
		for (Char& c: front)
		{
			c.setCharacter(' ');
			c.setCharacterFont(renderer->getCharacterInLayoutCharacteres(' '));
			c.setForeground(foreground);
			c.setBackground(background);
			c.setDirt(false);
		}
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

void Doryen::Console::drawFillRect(int x, int y, int rw, int rh, bool clear, BlendModes flag)
{
	// Asserts
	if (x < 0 || y < 0 || rw < 0 || rh < 0)
	{
		throw "ExceptionIllegalArgument";
	}

	if (isConsoleRoot)
	{
		// Asserts
		if (x > renderer->getWidth() || y > renderer->getHeight() ||
			x + rw > renderer->getWidth() || y + rh > renderer->getHeight())
		{
			throw "ExceptionIllegalArgument";
		}
		else
		{
			Point start = Point(0, 0);
			Point end = Point((int)renderer->getWidth(), (int)renderer->getHeight());

			Point first = Point(x, y);
			Point second = Point(rw, rh);

			// Remember, pass for reference
			consoleClamp(start, end, first, second);

			for (int cx = first.x; cx < first.x + second.x; ++cx)
			{
				for (int cy = first.y; cy < first.y + second.y; ++cy)
				{
					setCellBackgroundColor(cx, cy, renderer->getBackground(), flag);

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
					setCellBackgroundColor(cx, cy, background, flag);

					if (clear)
					{
						unsigned index = cx + width * cy;

						front[index].setCharacter(' ');
						front[index].setCharacterFont(
								renderer->getCharacterInLayoutCharacteres(' '));
					}
				}
			}
		}
	}
}

void Doryen::Console::drawHorizontalLine(int x, int y, int l, BlendModes flag)
{
	for (int i = x; i < x + l; ++i)
	{
		// Character 196 ASCII, see table ASCII.
		writeChar(i, y, 196, flag);
	}
}

void Doryen::Console::drawVerticalLine(int x, int y, int l, BlendModes flag)
{
	for (int i = y; i < y + l; ++i)
	{
		// Character 179 ASCII, see table ASCII.
		writeChar(x, i, 179, flag);
	}
}

void Doryen::Console::drawFrame(const Point& start, const Point& end, bool empty, BlendModes flag)
{
	//Draw the edges
	writeChar(start.x, start.y, 218, flag);
	writeChar(start.x + end.x - 1, start.y, 191, flag);
	writeChar(start.x, start.y + end.y - 1, 192, flag);
	writeChar(start.x + end.x - 1, start.y + end.y - 1, 217, flag);

	// Draw the border of frame
	drawHorizontalLine(start.x + 1, start.y, end.x - 2, flag);
	drawHorizontalLine(start.x + 1, start.y + end.y - 1, end.x - 2, flag);

	if (end.y > 2)
	{
		// Draw the border of frame
		drawVerticalLine(start.x, start.y + 1, end.y - 2, flag);
		drawVerticalLine(start.x + end.x - 1, start.y + 1, end.y - 2, flag);

		if (empty)
		{
			drawFillRect(start.x + 1, start.y + 1, end.x - 2, end.y - 2, true, flag);
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
		writeString(_start.x, _start.y, _title.data());
		return;
	}

	// If not exit overflow, center the title
	std::uint16_t centerX = (lengthFrame - _title.size()) / 2;
	writeString(centerX, _start.y, _title.data());
}

void Doryen::Console::drawWindowFrame(int x, int y, int w, int h, bool clear,
		Doryen::BlendModes flag, const std::string& name)
{
	// Curly braces for create a Point directly.
	drawFrame({ x, y }, { w, h }, clear, flag);
	drawFrameTitle(name, { x, y }, w);
}


void Doryen::Console::writeString(int x, int y, const std::string& fmt)
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
		writeChar(x + i, y, fmt[i]);
	}
}

int Doryen::Console::writeWrapText(int x, int y, int w, int h, BlendModes flag,
		int alignment, const char* fmt, ...)
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
		std::string line = _text.substr(lastCharacterProcessed, positionCharacterNewLine - lastCharacterProcessed);
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

	std::vector<std::string> wrapText{ 1 };
	std::uint16_t currentIndex = 0;

	for (const std::vector<std::string>& paragraph: paragraphs)
	{
		// First iteration
		std::uint16_t spaceLeft = LINE_WIDTH;

		for (const std::string& word : paragraph)
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

		// Insert a space in blank
		wrapText.emplace_back("");
		// The new paragraph begin here
		wrapText.emplace_back("");
		// For write the lines from the
		// actual point
		currentIndex += 2;
	}

	return wrapText;
}

void Doryen::Console::writeWrapText(const Geometry::Point2D<>& coordinate,
		const Geometry::Size& size, BlendModes flag, std::string_view text)
{
	if (text.size() > size.w)
	{
		// Wrap the text
		const std::vector<std::string> lineTexts = wrapText(text, size.w);

		std::uint16_t currentY = coordinate.y;

		for (const std::string& _text : lineTexts)
		{
			if (_text.empty())
			{
				currentY += 1;
				continue;
			}

			writeString(coordinate.x, currentY, _text);
			currentY += 1;
		}
	}
	else
	{
		writeString(coordinate.x, coordinate.y, text.data());
	}
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

			const Char srcChar = front[cy * width + cx];
			Char dstChar = srcChar;

			if (foregroundAlpha == 1.0f and backgroundAlpha == 1.0f)
			{
				dstChar = srcChar;
			}
			else
			{
				dstChar.setCharacter(destination.getCellCode(dest.x, dest.y));
				dstChar.setBackground(destination.getCellBackgroundColor(dest.x, dest.y));
				dstChar.setForeground(destination.getCellForegroundColor(dest.x, dest.y));

				dstChar.setBackground(Color::lerp(dstChar.getBackground(),
						srcChar.getBackground(), backgroundAlpha));

				if (srcChar.getCharacter() == ' ')
				{
					dstChar.setForeground(Color::lerp(dstChar.getForeground(),
							srcChar.getBackground(), backgroundAlpha));
				}
				else if (dstChar.getCharacter() == ' ')
				{
					dstChar.setCharacter(srcChar.getCharacter());
					dstChar.setCharacterFont(srcChar.getCharacterFont());
					dstChar.setForeground(Color::lerp(dstChar.getBackground(),
							srcChar.getForeground(), foregroundAlpha));
				}
				else if (dstChar.getCharacter() == srcChar.getCharacter())
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
						dstChar.setCharacter(srcChar.getCharacter());
						dstChar.setCharacterFont(srcChar.getCharacterFont());
						dstChar.setForeground(Color::lerp(dstChar.getBackground(),
								srcChar.getForeground(), (foregroundAlpha - 0.5f) * 2));
					}
				}
			}

			const int dx = cx - source.x + dest.x;
			const int dy = cy - source.y + dest.y;

			if (dx >= destination.getWidth() or dy >= destination.getHeight()) continue;

			// See the documentation internal of initRoot method for see details of an
			// buffer produce for this method [blit]
			// Actually, this method [blit] is free of bugs
			destination.writeChar(dx, dy, dstChar.getCharacter(), dstChar.getForeground(), dstChar.getBackground());
		}
	}
}

void Doryen::Console::showCursor(bool show)
{
	renderer->showCursor(show);
}

void Console::registerCallback(std::unique_ptr<CallbackRender> _render) const
{
	renderer->registerCallback(std::move(_render));
}

void Console::unregisterCallback() const
{
	renderer->unregisterCallback();
}

// Getters

float Doryen::Console::getLastFrameLength() const
{
	return renderer->getLastFrameLength();
}

const Doryen::Mouse& Doryen::Console::getMouseEvent() const
{
	return renderer->getMouseEvent();
}

std::uint32_t Doryen::Console::getFramePerSeconds() const
{
	return renderer->getFramePerSeconds();
}

const Geometry::Size Console::getFontSize() const
{
	return { (int)renderer->getFontWidth(), (int)renderer->getFontHeight() };
}

std::uint32_t Console::getElapsedMilliseconds() const
{
	return renderer->getElapsedMilliseconds();
}

float Console::getElapsedSeconds() const
{
	return renderer->getElapsedSeconds();
}


const Doryen::Color& Doryen::Console::getCellForegroundColor(int x, int y) const
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

		return front[index].getForeground();
	}
}

int Doryen::Console::getCellCode(int x, int y) const
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

		return front[offset].getCharacter();
	}
}

const Doryen::Color& Doryen::Console::getCellBackgroundColor(int x, int y) const
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

		return front[index].getBackground();
	}
}

const Doryen::Color& Doryen::Console::getForegroundColor() const
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

const Doryen::Color& Doryen::Console::getBackgroundColor() const
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

const Doryen::Key& Doryen::Console::getKeyPressed() const
{
	return renderer->getKeyPressed();
}

std::uint32_t Doryen::Console::getWidth() const
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

std::uint32_t Doryen::Console::getHeight() const
{
	if (isConsoleRoot)
	{
		return renderer->getHeight();
	}
	else
	{
		return height;
	}
}

short Doryen::Console::getFade() const
{
	return renderer->getFade();
}

const Doryen::Color& Doryen::Console::getFadingColor() const
{
	return renderer->getFadingColor();
}

// Setters

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

void Doryen::Console::setCellBackgroundColor(int x, int y, const Doryen::Color& col, Doryen::BlendModes flag)
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
		if (x > renderer->getWidth() || y > renderer->getHeight())
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
			Color b = front[x + width * y].getBackground();

			b.trasformColor(col, flag);

			front[x + width * y].setBackground(b);
		}
	}
}

void Doryen::Console::setCellForegroundColor(int x, int y, const Doryen::Color& col)
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

		front[index].setForeground(col);
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
		// Initial point
		Point start = Point(x, y);

		// End Point
		Point end = Point(w, h);

		renderer->setDirty(start, end);
	}
}

void Doryen::Console::setBackgroundColor(const Color& back)
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

void Doryen::Console::setForegroundColor(const Color& fore)
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

void Doryen::Console::setFade(short val, const Doryen::Color& fade)
{
	renderer->setFade(val);
	renderer->setFadingColor(fade);
}

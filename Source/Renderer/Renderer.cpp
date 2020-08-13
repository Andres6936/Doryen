#include "Doryen/Renderer/Renderer.hpp"

// Static Members

const std::array<int, 256> Doryen::Renderer::layoutAsciiCode =

		{
				// ASCII 0 to 15
				0, 0, 0, 0, 0, 0, 0, 0, 0, 76, 77, 0, 0, 0, 0, 0,
				// ASCII 16 to 31
				71, 70, 72, 0, 0, 0, 0, 0, 64, 65, 67, 66, 0, 73, 68, 69,
				//  ASCII 32 to 47
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
				// ASCII 48 to 63
				16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
				// ASCII 64 to 79
				32, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
				// ASCII 80 to 95
				111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 33, 34, 35, 36, 37,
				// ASCII 96 to 111
				38, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
				// ASCII 112 to 127
				143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 39, 40, 41, 42, 0,
				// ASCII 128 to 143
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				// ASCII 144 to 159
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				// ASCII 160 to 175
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				// ASCII 176 to 191
				43, 44, 45, 46, 49, 0, 0, 0, 0, 81, 78, 87, 88, 0, 0, 55,
				// ASCII 192 to 207
				53, 50, 52, 51, 47, 48, 0, 0, 85, 86, 82, 84, 83, 79, 80, 0,
				// ASCII 208 to 223
				0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 54, 0, 0, 0, 0, 0,
				// ASCII 224 to 239
				74, 75, 57, 58, 59, 60, 61, 62, 63, 0, 0, 0, 0, 0, 0, 0,
				// ASCII 240 to 255
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		};

// Construct

Doryen::Renderer::Renderer() noexcept
{
	// The counter for the time elapsed start to count
	timeElapsed = std::chrono::high_resolution_clock::now();
}

// Methods

void Doryen::Renderer::createTablesOfCharacteres()
{
	if (getFontCharHorizontalSize() * getFontCharVerticalSize() != getMaxFontChars())
	{
		setMaxFontChars(getFontCharHorizontalSize() * getFontCharVerticalSize());
	}

	characterUpdated.clear();
	characterUpdated.resize(getMaxFontChars(), false);

	characterDrawed.clear();
	characterDrawed.resize(getMaxFontChars(), true);

	characterColor.clear();
	characterColor.resize(getMaxFontChars());

	layoutCharacteres.clear();
	layoutCharacteres.resize(getMaxFontChars(), 0);

	colored.clear();
	colored.resize(getMaxFontChars(), false);

	for (int i = 0; i < layoutCharacteres.size(); ++i)
	{
		layoutCharacteres[i] = layoutAsciiCode[i];
	}
}

std::uint32_t Doryen::Renderer::getFontCharHorizontalSize() const
{
	return fontCharHorizontalSize;
}

std::uint32_t Doryen::Renderer::getFontCharVerticalSize() const
{
	return fontCharVerticalSize;
}

void Doryen::Renderer::setFontWidth(std::uint32_t _fontWidth)
{
	fontWidth = _fontWidth;
}

void Doryen::Renderer::setFontHeigth(std::uint32_t _fontHeigth)
{
	fontHeigth = _fontHeigth;
}

std::uint32_t Doryen::Renderer::getMaxFontChars() const
{
	return maxFontChars;
}

void Doryen::Renderer::setMaxFontChars(std::uint32_t _maxFontChars)
{
	maxFontChars = _maxFontChars;
}

float Doryen::Renderer::getLastFrameLength() const
{
	return lastFrameLength;
}

bool Doryen::Renderer::isFontHasDoryenLayout() const
{
	return fontHasDoryenLayout;
}

std::uint32_t Doryen::Renderer::getFontWidth() const
{
	return fontWidth;
}

std::uint32_t Doryen::Renderer::getFontHeigth() const
{
	return fontHeigth;
}

bool Doryen::Renderer::isFontHasRowLayout() const
{
	return fontHasRowLayout;
}

void Doryen::Renderer::changeFontKeyColor(const Doryen::Color& _color)
{
	fontKeyColor.r = _color.r;
	fontKeyColor.g = _color.g;
	fontKeyColor.b = _color.b;
	fontKeyColor.a = _color.a;
}

bool Doryen::Renderer::isFontGrayscale() const
{
	return fontGrayscale;
}

const Doryen::Color& Doryen::Renderer::getFontKeyColor() const
{
	return fontKeyColor;
}

bool Doryen::Renderer::isCharacterColored(std::uint32_t index) const
{
	return colored[index];
}

void Doryen::Renderer::setCharacterColored(const std::uint32_t index, bool isColored)
{
	colored[index] = isColored;
}

void Doryen::Renderer::fillCharacterColorWith(const Doryen::Color& _color)
{
	for (Color& c : characterColor)
	{
		c = _color;
	}
}

void Doryen::Renderer::fillCharacterDrawedWith(const bool isDrawed)
{
	for (bool&& b : characterDrawed)
	{
		b = isDrawed;
	}
}

void Doryen::Renderer::checkTableOfCharacteres()
{
	if (getFontCharHorizontalSize() * getFontCharVerticalSize() != getMaxFontChars())
	{
		setMaxFontChars(getFontCharHorizontalSize() * getFontCharVerticalSize());

		characterUpdated.clear();
		characterUpdated.resize(getMaxFontChars(), false);

		characterDrawed.clear();
		characterDrawed.resize(getMaxFontChars(), true);

		characterColor.clear();
		characterColor.resize(getMaxFontChars());

		colored.clear();
		colored.resize(getMaxFontChars(), false);

		layoutCharacteres.clear();
		layoutCharacteres.resize(getMaxFontChars(), 0);

		for (int i = 0; i < layoutCharacteres.size(); ++i)
		{
			layoutCharacteres[i] = layoutAsciiCode[i];
		}
	}
}

void Doryen::Renderer::setLayoutCharacter(std::uint32_t index, std::uint32_t code)
{
	layoutCharacteres[index] = code;
}

bool Doryen::Renderer::isRunning() const
{
	return running;
}

bool Doryen::Renderer::isFullscreen() const
{
	return fullscreen;
}

void Doryen::Renderer::setWidth(std::uint32_t _width)
{
	width = _width;
}

void Doryen::Renderer::setHeigth(std::uint32_t _heigth)
{
	heigth = _heigth;
}

std::uint32_t Doryen::Renderer::getWidth() const
{
	return width;
}

std::uint32_t Doryen::Renderer::getHeigth() const
{
	return heigth;
}

void Doryen::Renderer::createBuffer()
{
	buffer.resize(getWidth() * getHeigth());

	oldBuffer.resize(getWidth() * getHeigth());
}

const std::string& Doryen::Renderer::getFontfile() const
{
	return fontfile;
}

void Doryen::Renderer::setDirty(const Geometry::Point2D<>& _start, Geometry::Point2D<>& _end)
{
	// Primera comprobación: ¿Es el primer punto mayor a las coordenadas
	// de la ventana principal?
	if (_start.x > getWidth() || _start.y > getHeigth())
	{
		return;
	}
	else
	{
		// Segunda comprobación: ¿La suma de las coordendas x & y de
		// ambos puntos (el ancho y alto) es mayor al ancho y alto de
		// la ventana principal?
		if (_start.x + _end.x > getWidth() ||
			_start.y + _end.y > getHeigth())
		{
			_end.x = (int)getWidth() - _start.x;
			_end.y = (int)getHeigth() - _start.y;
		}

		for (int x = _start.x; x < _start.x + _end.x; ++x)
		{
			for (int y = _start.y; y < _start.y + _end.y; ++y)
			{
				std::uint32_t offset = x + getWidth() * y;

				buffer[offset].setDirt(true);
			}
		}
	}
}

short Doryen::Renderer::getFade() const
{
	return fade;
}

const Doryen::Color& Doryen::Renderer::getFadingColor() const
{
	return fadingColor;
}

bool Doryen::Renderer::isCharacterDrawed(std::uint32_t index) const
{
	return characterDrawed[index];
}

const Doryen::Color& Doryen::Renderer::getColorInCharacterColorAt(std::uint32_t index) const
{
	return characterColor[index];
}

void Doryen::Renderer::setCharacterDrawed(std::uint32_t index, bool isDrawed)
{
	characterDrawed[index] = isDrawed;
}

std::uint32_t Doryen::Renderer::getRgbMask() const
{
	return RGBMask;
}

void Doryen::Renderer::setRgbMask(std::uint32_t _rgbMask)
{
	RGBMask = _rgbMask;
}

std::uint32_t Doryen::Renderer::getNrgbMask() const
{
	return nRGBMask;
}

void Doryen::Renderer::setNrgbMask(std::uint32_t _nRgbMask)
{
	nRGBMask = _nRgbMask;
}

void Doryen::Renderer::setColorInCharacterColorAt(std::uint32_t index, const Doryen::Color& _color)
{
	characterColor[index] = _color;
}

std::uint32_t Doryen::Renderer::getSdlKey() const
{
	return SDLKey;
}

void Doryen::Renderer::setSdlKey(std::uint32_t _sdlKey)
{
	SDLKey = _sdlKey;
}

bool Doryen::Renderer::isFontUpdated() const
{
	return fontUpdated;
}

void Doryen::Renderer::clearCharacterUpdate()
{
	characterUpdated.clear();
}

void Doryen::Renderer::resizeCharacterUpdate(std::uint32_t size)
{
	characterUpdated.resize(size, false);
}

std::uint32_t Doryen::Renderer::getCurrentFramePerSeconds() const
{
	return currentFramePerSeconds;
}

void Doryen::Renderer::setFramePerSeconds(std::uint32_t _framePerSeconds)
{
	framePerSeconds = _framePerSeconds;
}

void Doryen::Renderer::setCurrentFramePerSeconds(std::uint32_t _currentFramePerSeconds)
{
	currentFramePerSeconds = _currentFramePerSeconds;
}

void Doryen::Renderer::setLastFrameLength(float _lastFrameLength)
{
	lastFrameLength = _lastFrameLength;
}

std::uint32_t Doryen::Renderer::getMinimunFrameLength() const
{
	return minimunFrameLength;
}

void Doryen::Renderer::fillOldBuffer()
{
	for (int i = 0; i < buffer.size(); ++i)
	{
		oldBuffer[i] = buffer[i];
	}
}

int Doryen::Renderer::getCharacterInLayoutCharacteres(std::uint32_t index) const
{
	return layoutCharacteres[index];
}

short Doryen::Renderer::getOldFade() const
{
	return oldFade;
}

bool Doryen::Renderer::isCharacterUpdated(std::uint32_t index) const
{
	return characterUpdated[index];
}

void Doryen::Renderer::setOldFade(short _oldFade)
{
	oldFade = _oldFade;
}

bool Doryen::Renderer::isAnyCharacterUpdated() const
{
	return anyCharacterUpdated;
}

void Doryen::Renderer::setAnyCharacterUpdated(bool _anyCharacterUpdated)
{
	anyCharacterUpdated = _anyCharacterUpdated;
}

void Doryen::Renderer::setFade(short _fade)
{
	fade = _fade;
}

void Doryen::Renderer::setFadingColor(const Doryen::Color& _fadingColor)
{
	fadingColor = _fadingColor;
}

void Doryen::Renderer::setCharacterInBufferAt(std::uint32_t index, const Char& _char)
{
	buffer[index] = _char;
}

const Doryen::Color& Doryen::Renderer::getForeground() const
{
	return foreground;
}

const Doryen::Color& Doryen::Renderer::getBackground() const
{
	return background;
}

const Doryen::Color& Doryen::Renderer::getBackgroundOfCharacterInBufferAt(std::uint32_t index) const
{
	return buffer[index].getBackground();
}

void Doryen::Renderer::setBackgroundOfCharacterInBufferAt(std::uint32_t index, const Doryen::Color& _color)
{
	buffer[index].setBackground(_color);
}

int Doryen::Renderer::getCharOfCharacterInBufferAt(std::uint32_t index) const
{
	return buffer[index].getCharacter();
}

void Doryen::Renderer::setForeground(const Doryen::Color& _foreground)
{
	foreground = _foreground;
}

void Doryen::Renderer::setBackground(const Doryen::Color& _background)
{
	background = _background;
}

void Doryen::Renderer::clearBuffer()
{
	for (Char& c : buffer)
	{
		c.setCharacter(' ');
		c.setCharacterFont(getCharacterInLayoutCharacteres(' '));
		c.setForeground(foreground);
		c.setBackground(background);
		c.setDirt(false);
	}
}

void Doryen::Renderer::setForegroundOfCharacterInBufferAt(std::uint32_t index, const Doryen::Color& _color)
{
	buffer[index].setForeground(_color);
}

const Doryen::Color& Doryen::Renderer::getForegroundOfCharacterInBufferAt(std::uint32_t index) const
{
	return buffer[index].getForeground();
}

void Doryen::Renderer::setCharOfCharacterInBufferAt(std::uint32_t index, int _char)
{
	buffer[index].setCharacter(_char);
	buffer[index].setCharacterFont(getCharacterInLayoutCharacteres(_char));
}

void Doryen::Renderer::setFontfile(const std::string& _fontfile)
{
	fontfile = _fontfile;
}

void Doryen::Renderer::setFontHasDoryenLayout(bool _fontHasDoryenLayout)
{
	fontHasDoryenLayout = _fontHasDoryenLayout;
}

void Doryen::Renderer::setFontHasRowLayout(bool _fontHasRowLayout)
{
	fontHasRowLayout = _fontHasRowLayout;
}

void Doryen::Renderer::setFontGrayscale(bool _fontGrayscale)
{
	fontGrayscale = _fontGrayscale;
}

void Doryen::Renderer::setFontCharHorizontalSize(std::uint32_t _fontCharHorizontalSize)
{
	fontCharHorizontalSize = _fontCharHorizontalSize;
}

void Doryen::Renderer::setFontCharVerticalSize(std::uint32_t _fontCharVerticalSize)
{
	fontCharVerticalSize = _fontCharVerticalSize;
}

std::uint32_t Doryen::Renderer::getFramePerSeconds() const
{
	return framePerSeconds;
}

void Doryen::Renderer::setRunning(bool _running)
{
	running = _running;
}

void Doryen::Renderer::setMinimumFrameLength(std::uint32_t _frameLength)
{
	minimunFrameLength = _frameLength;
}

void Doryen::Renderer::unregisterCallback()
{
	// Set to null the render
	callbackRender.reset(nullptr);
}

float Doryen::Renderer::getElapsedSeconds() const
{
	return getElapsedMilliseconds() * (1.0f / 1000.0f);
}

std::uint32_t Doryen::Renderer::getElapsedMilliseconds() const
{
	const auto actualTime = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(actualTime - timeElapsed).count();
}

const Doryen::Key& Doryen::Renderer::getKeyPressed() const
{
	return keyPressed;
}

const Doryen::Mouse& Doryen::Renderer::getMouseEvent() const
{
	return mouse;
}

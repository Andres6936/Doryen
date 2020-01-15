#include <Platform/Platform.hpp>
#include "Renderer/Renderer.hpp"

// Static Members

bool Doryen::Renderer::hasInstanceActive = false;

const std::array <int, 256> Doryen::Renderer::layoutAsciiCode =

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

unsigned int Doryen::Renderer::getFontCharHorizontalSize() const
{
	return fontCharHorizontalSize;
}

unsigned int Doryen::Renderer::getFontCharVerticalSize() const
{
	return fontCharVerticalSize;
}

void Doryen::Renderer::setFontWidth(unsigned int _fontWidth)
{
	fontWidth = _fontWidth;
}

void Doryen::Renderer::setFontHeigth(unsigned int _fontHeigth)
{
	fontHeigth = _fontHeigth;
}

unsigned int Doryen::Renderer::getMaxFontChars() const
{
	return maxFontChars;
}

void Doryen::Renderer::setMaxFontChars(unsigned int _maxFontChars)
{
	maxFontChars = _maxFontChars;
}

bool Doryen::Renderer::isFontHasDoryenLayout() const
{
	return fontHasDoryenLayout;
}

unsigned int Doryen::Renderer::getFontWidth() const
{
	return fontWidth;
}

unsigned int Doryen::Renderer::getFontHeigth() const
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

bool Doryen::Renderer::isCharacterColored(unsigned index) const
{
	return colored[index];
}

void Doryen::Renderer::setCharacterColored(const unsigned index, bool isColored)
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

		layoutCharacteres.clear();
		layoutCharacteres.resize(getMaxFontChars(), 0);

		colored.clear();
		colored.resize(getMaxFontChars(), false);

		for (int i = 0; i < layoutCharacteres.size(); ++i)
		{
			layoutCharacteres[i] = layoutAsciiCode[i];
		}
	}
}

void Doryen::Renderer::setLayoutCharacter(unsigned index, unsigned code)
{
	layoutCharacteres[index] = code;
}

bool Doryen::Renderer::isFullscreen() const
{
	return fullscreen;
}

void Doryen::Renderer::setWidth(unsigned int _width)
{
	width = _width;
}

void Doryen::Renderer::setHeigth(unsigned int _heigth)
{
	heigth = _heigth;
}

unsigned int Doryen::Renderer::getWidth() const
{
	return width;
}

unsigned int Doryen::Renderer::getHeigth() const
{
	return heigth;
}

unsigned int Doryen::Renderer::getFullscreenWidth() const
{
	return fullscreenWidth;
}

unsigned int Doryen::Renderer::getFullscreenHeigth() const
{
	return fullscreenHeigth;
}

void Doryen::Renderer::setActualFullscreenWidth(unsigned int width)
{
	actualFullscreenWidth = width;
}

void Doryen::Renderer::setActualFullscreenHeigth(unsigned int heigth)
{
	actualFullscreenHeigth = heigth;
}

void Doryen::Renderer::setFullscreen(bool _fullscreen)
{
	fullscreen = _fullscreen;
}

unsigned int Doryen::Renderer::getActualFullscreenWidth() const
{
	return actualFullscreenWidth;
}

unsigned int Doryen::Renderer::getActualFullscreenHeigth() const
{
	return actualFullscreenHeigth;
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

void Doryen::Renderer::setDirty(const Math::Point2D& _start, Math::Point2D& _end)
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
				unsigned offset = x + getWidth() * y;

				buffer[offset].setDirt(true);
			}
		}
	}
}


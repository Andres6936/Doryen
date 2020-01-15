#ifndef LIBTCOD_RENDERER_HPP
#define LIBTCOD_RENDERER_HPP

#include <vector>
#include <array>

#include "Color.h"
#include "Color.hpp"

namespace Doryen
{
	class Renderer
	{

		// Private for common class, public for inheritance class
	protected:

		static bool hasInstanceActive;

		std::string fontfile = "Terminal.png";

	private:

		unsigned width = 0;

		unsigned heigth = 0;

		unsigned maxFontChars = 256;

		unsigned fontCharHorizontalSize = 16;

		unsigned fontCharVerticalSize = 16;

		unsigned fontWidth = 0;

		unsigned fontHeigth = 0;

		unsigned fullscreenWidth = 0;

		unsigned fullscreenHeigth = 0;

		unsigned actualFullscreenWidth = 0;

		unsigned actualFullscreenHeigth = 0;

		bool fontHasDoryenLayout = false;

		bool fontHasRowLayout = false;

		bool fontGrayscale = false;

		bool fullscreen = false;

		/**
		 * whether each character in the font is a colored tile
		 */
		std::vector <bool> colored;

		std::vector <bool> characterUpdated;

		std::vector <bool> characterDrawed;

		std::vector <int> layoutCharacteres;

		std::vector <Color> characterColor;

		Color fontKeyColor = Color(0, 0, 0);

	public:

		// Constructs

		Renderer() = default;

		virtual ~Renderer() = default;

		// Static members

		/**
		 * Convert ASCII code to Doryen layout position
		 */
		static const std::array <int, 256> layoutAsciiCode;

		// Methods Default

		void createTablesOfCharacteres();

		void checkTableOfCharacteres();

		void changeFontKeyColor(const Color& _color);

		void fillCharacterDrawedWith(const bool isDrawed);

		void fillCharacterColorWith(const Color& _color);

		// Getters

		bool isFontHasDoryenLayout() const;

		bool isFontHasRowLayout() const;

		bool isFontGrayscale() const;

		bool isCharacterColored(unsigned index) const;

		bool isFullscreen() const;

		unsigned int getWidth() const;

		unsigned int getHeigth() const;

		unsigned int getFullscreenWidth() const;

		unsigned int getFullscreenHeigth() const;

		unsigned int getMaxFontChars() const;

		unsigned int getFontCharHorizontalSize() const;

		unsigned int getFontCharVerticalSize() const;

		unsigned int getFontWidth() const;

		unsigned int getFontHeigth() const;

		const Color& getFontKeyColor() const;

		// Setters

		void setWidth(unsigned int _width);

		void setHeigth(unsigned int _heigth);

		void setActualFullscreenWidth(unsigned int _actualFullscreenWidth);

		void setActualFullscreenHeigth(unsigned int _actualFullscreenHeigth);

		void setMaxFontChars(unsigned int _maxFontChars);

		void setFontWidth(unsigned int _fontWidth);

		void setFontHeigth(unsigned int _fontHeigth);

		void setFontfile(const std::string& _fontfile);

		void setCharacterColored(unsigned index, bool isColored);

		void setLayoutCharacter(unsigned index, unsigned code);

		// Methods Pures

		virtual void onRenderer() = 0;

		virtual void loadFont() = 0;
	};
}

#endif //LIBTCOD_RENDERER_HPP

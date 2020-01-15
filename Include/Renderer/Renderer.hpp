#ifndef LIBTCOD_RENDERER_HPP
#define LIBTCOD_RENDERER_HPP

#include <vector>
#include <array>

#include "Color.h"
#include "Char.hpp"
#include "Color.hpp"
#include "Event/Key.hpp"
#include "Math/Point2D.hpp"

namespace Doryen
{
	class Renderer
	{

		// Private for common class, public for inheritance class
	protected:

		static bool hasInstanceActive;

		std::vector <Char> buffer;

		std::vector <Char> oldBuffer;

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

		std::string fontfile = "Terminal.png";

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

		void createBuffer();

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

		unsigned int getActualFullscreenWidth() const;

		unsigned int getActualFullscreenHeigth() const;

		unsigned int getFullscreenWidth() const;

		unsigned int getFullscreenHeigth() const;

		unsigned int getMaxFontChars() const;

		unsigned int getFontCharHorizontalSize() const;

		unsigned int getFontCharVerticalSize() const;

		unsigned int getFontWidth() const;

		unsigned int getFontHeigth() const;

		const std::string& getFontfile() const;

		const Color& getFontKeyColor() const;

		// Setters

		void setWidth(unsigned int _width);

		void setHeigth(unsigned int _heigth);

		void setFullscreen(bool _fullscreen);

		void setActualFullscreenWidth(unsigned int width);

		void setActualFullscreenHeigth(unsigned int heigth);

		void setMaxFontChars(unsigned int _maxFontChars);

		void setFontWidth(unsigned int _fontWidth);

		void setFontHeigth(unsigned int _fontHeigth);

		void setCharacterColored(unsigned index, bool isColored);

		void setLayoutCharacter(unsigned index, unsigned code);

		void setDirty(const Math::Point2D& _start, Math::Point2D& _end);

		// Methods Pures

		virtual void onRenderer() = 0;

		virtual void onExit() = 0;

		virtual void loadFont() = 0;

		virtual Key getKeyPressed() = 0;

		virtual void setWindowTitle(const std::string& _title) = 0;

		virtual void setWindowInFullscreen() = 0;
	};
}

#endif //LIBTCOD_RENDERER_HPP

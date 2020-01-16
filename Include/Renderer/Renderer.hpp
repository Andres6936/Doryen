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

		/**
		 *  Length of the last rendering loop
		 */
		float lastFrameLength = 0.0f;

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

		unsigned SDLKey = 0;

		unsigned RGBMask = 0;

		unsigned nRGBMask = 0;

		/**
		 * Number of frames in the last second
		 */
		unsigned framePerSeconds = 0;

		/**
		 * Current number of frames
		 */
		unsigned currentFramePerSeconds = 0;

		/**
		 * Minimum length for a frame (when fps are limited)
		 */
		unsigned minimunFrameLength = 0;

		/**
		 * Used as reference of frame length
		 */
		unsigned minimunFrameLengthBackup = 0;

		short fade = 255;

		bool fontHasDoryenLayout = false;

		bool fontHasRowLayout = false;

		bool fontGrayscale = false;

		bool fullscreen = false;

		bool fontUpdated = false;

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

		Color fadingColor = Color(128, 128, 128);

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

		void clearCharacterUpdate();

		void resizeCharacterUpdate(unsigned size);

		void changeFontKeyColor(const Color& _color);

		void fillCharacterDrawedWith(bool isDrawed);

		void fillCharacterColorWith(const Color& _color);

		/**
		 * The content of buffer will be moved to oldBuffer
		 */
		void fillOldBuffer();

		// Getters

		bool isFontHasDoryenLayout() const;

		bool isFontHasRowLayout() const;

		bool isFontGrayscale() const;

		bool isCharacterColored(unsigned index) const;

		bool isCharacterDrawed(unsigned index) const;

		bool isFullscreen() const;

		bool isFontUpdated() const;

		short getFade() const;

		int getCharacterInLayoutCharacteres(unsigned index) const;

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

		unsigned int getSdlKey() const;

		unsigned int getRgbMask() const;

		unsigned int getNrgbMask() const;

		unsigned int getCurrentFramePerSeconds() const;

		unsigned int getMinimunFrameLength() const;

		const std::string& getFontfile() const;

		const Color& getColorInCharacterColorAt(unsigned index) const;

		const Color& getFadingColor() const;

		const Color& getFontKeyColor() const;

		// Setters

		void setLastFrameLength(float _lastFrameLength);

		void setWidth(unsigned int _width);

		void setHeigth(unsigned int _heigth);

		void setFullscreen(bool _fullscreen);

		void setActualFullscreenWidth(unsigned int width);

		void setActualFullscreenHeigth(unsigned int heigth);

		void setMaxFontChars(unsigned int _maxFontChars);

		void setFontWidth(unsigned int _fontWidth);

		void setFontHeigth(unsigned int _fontHeigth);

		void setSdlKey(unsigned int _sdlKey);

		void setRgbMask(unsigned int _rgbMask);

		void setNrgbMask(unsigned int _nRgbMask);

		void setFramePerSeconds(unsigned int _framePerSeconds);

		void setCurrentFramePerSeconds(unsigned int _currentFramePerSeconds);

		void setCharacterColored(unsigned index, bool isColored);

		void setCharacterDrawed(unsigned index, bool isDrawed);

		void setLayoutCharacter(unsigned index, unsigned code);

		void setColorInCharacterColorAt(unsigned index, const Color& _color);

		void setDirty(const Math::Point2D& _start, Math::Point2D& _end);

		// Methods Pures

		virtual void onRenderer() = 0;

		virtual void onExit() = 0;

		virtual void draw() = 0;

		virtual void loadFont() = 0;

		virtual Key getKeyPressed() = 0;

		virtual void setWindowTitle(const std::string& _title) = 0;

		virtual void setWindowInFullscreen() = 0;
	};
}

#endif //LIBTCOD_RENDERER_HPP

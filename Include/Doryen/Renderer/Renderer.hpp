#ifndef LIBTCOD_RENDERER_HPP
#define LIBTCOD_RENDERER_HPP

#include <array>
#include <vector>
#include <chrono>
#include <memory>
#include <cstdint>

#include "Doryen/Event/Key.hpp"
#include "Doryen/Event/Mouse.hpp"
#include "Doryen/Console/Char.hpp"
#include "Doryen/Geometry/Point2D.hpp"
#include "Doryen/Graphics/Color/Color.hpp"
#include "Doryen/Graphics/Callback/Render.hpp"

namespace Doryen
{

	/**
	 * Defines the requirements for an object responsible for "rendering"
	 * (displaying) a value.
	 * <br><br>
	 *
	 * @note
	 *
	 * The methods that return a reference are used for avoid return an copy,
	 * is need take into account the life cycle of the variable that is returned.
	 * <br><br>
	 *
	 * If the reference returned by the method is to be used almost
	 * immediately, there will be no problem with the life cycle, if on the
	 * contrary the reference is stored and the life cycle of this reference
	 * ends, an unauthorized memory access will occur and the operating
	 * system will terminate the execution of the application.
	 * <br><br>
	 *
	 * If you need to store the data that the method returns, create a copy
	 * from the reference to avoid problems with the life cycle.
	 * <br><br>
	 *
	 * This interface use the double-buffering that uses two buffers, called
	 * front and back, which hold a array of characters that will be convert to
	 * a image. The front buffer is visible to the user; the back buffer is not.
	 * Successive frames of an animation are rendered into the back buffer while
	 * the previously rendered frame is displayed in the front buffer. When a
	 * new frame is ready, the back and front buffers swap roles, making the new
	 * frame visible. Ideally, this exchange appears to happen instantaneously
	 * to the user and with no visual artifacts. Thus, only completely rendered
	 * images are presented to the user, and they remain visible during the
	 * entire time it takes to render a new frame. The result is a flicker-free
	 * animation.
	 * <br><br>
	 *
	 * In general, both the front and back buffers are treated the same.
	 * particular, here are some important characteristics:
	 *
	 * - Only one buffer per window can be visible at a time (the front buffer).
	 * - Both buffers associated with a window have the same visual type, depth,
	 * 	 width, height, and shape as the window.
	 */
	class Renderer
	{

		// Private for common class, public for inheritance class
	protected:

		/**
		 * Mouse event
		 */
		Mouse mouse;

		/**
		 * Key event
		 */
		Key keyPressed;

		/**
		 * Main buffer used for the console. The content of this buffer will be
		 * drawn in the next call to method draw.
		 */
		std::vector<Char> front;

		/**
		 * Copy of main buffer of the console when is was drawn. It buffer is
		 * used for avoid the unnecessary calculation of rendering characters
		 * that are already drawn.
		 */
		std::vector<Char> back;

		std::unique_ptr<CallbackRender> callbackRender;

	private:

		short fade = 255;

		short oldFade = -1;

		/**
		 *  Length of the last rendering loop
		 */
		float lastFrameLength = 0.0f;

		std::uint32_t width = 0;

		std::uint32_t height = 0;

		std::uint32_t maxFontChars = 256;

		std::uint32_t fontCharHorizontalSize = 16;

		std::uint32_t fontCharVerticalSize = 16;

		std::uint32_t fontWidth = 0;

		std::uint32_t fontHeight = 0;

		std::uint32_t SDLKey = 0;

		std::uint32_t RGBMask = 0;

		std::uint32_t nRGBMask = 0;

		/**
		 * Number of frames in the last second
		 */
		std::uint32_t framePerSeconds = 0;

		/**
		 * Current number of frames
		 */
		std::uint32_t currentFramePerSeconds = 0;

		/**
		 * Minimum length for a frame (when fps are limited)
		 */
		std::uint32_t minimumFrameLength = 0;

		bool anyCharacterUpdated = false;

		bool fontHasDoryenLayout = false;

		bool fontHasRowLayout = false;

		bool fontGrayscale = false;

		bool fontUpdated = false;

		bool fullscreen = false;

		bool running = true;

		std::string fontfile = "Terminal.png";

		/**
		 * whether each character in the font is a colored tile
		 */
		std::vector<bool> colored;

		std::vector<bool> characterUpdated;

		std::vector<bool> characterDrawed;

		std::vector<int> layoutCharacters;

		std::vector<Color> characterColor;

		/**
		 * Counter that mark the total elapsed time from the start of application.
		 * This counter is initialized from the moment in that the first console
		 * called a instance of Renderer.
		 *
		 * @pre The construct for default for the class Renderer has been called.
		 */
		std::chrono::time_point<std::chrono::high_resolution_clock> timeElapsed;

		Color foreground = Color(255, 255, 255);

		Color background = Color(0, 0, 0);

		Color fontKeyColor = Color(0, 0, 0);

		Color fadingColor = Color(128, 128, 128);

	public:

		// Constructs

		/**
		 * @post The counter for the elapsed time from the start of application
		 *  has been initialized.
		 */
		Renderer() noexcept;

		virtual ~Renderer() = default;

		// Static members

		/**
		 * Convert ASCII code to Doryen layout position
		 */
		static const std::array<int, 256> layoutAsciiCode;

		// Methods Default

		void createTablesOfCharacters();

		void createBuffer();

		void checkTableOfCharacteres();

		void clearCharacterUpdate();

		void clearBuffer();

		void resizeCharacterUpdate(std::uint32_t size);

		void changeFontKeyColor(const Color& _color);

		void fillCharacterDrawedWith(bool isDrawed);

		void fillCharacterColorWith(const Color& _color);

		/**
		 * The content of buffer will be moved to oldBuffer
		 */
		void fillOldBuffer();

		void unregisterCallback();

		// Getters

		float getElapsedSeconds() const;

		float getLastFrameLength() const;

		bool isRunning() const;

		bool isFontUpdated() const;

		bool isFullscreen() const;

		bool isFontGrayscale() const;

		bool isFontHasRowLayout() const;

		bool isAnyCharacterUpdated() const;

		bool isFontHasDoryenLayout() const;

		bool isCharacterColored(std::uint32_t index) const;

		bool isCharacterDrawed(std::uint32_t index) const;

		bool isCharacterUpdated(std::uint32_t index) const;

		short getFade() const;

		short getOldFade() const;

		int getCharOfCharacterInBufferAt(std::uint32_t index) const;

		int getCharacterInLayoutCharacteres(std::uint32_t index) const;

		std::uint32_t getWidth() const;

		std::uint32_t getHeight() const;

		std::uint32_t getMaxFontChars() const;

		std::uint32_t getFontCharHorizontalSize() const;

		std::uint32_t getFontCharVerticalSize() const;

		std::uint32_t getFontWidth() const;

		std::uint32_t getFontHeight() const;

		std::uint32_t getSdlKey() const;

		std::uint32_t getRgbMask() const;

		std::uint32_t getNrgbMask() const;

		std::uint32_t getFramePerSeconds() const;

		std::uint32_t getMinimumFrameLength() const;

		std::uint32_t getElapsedMilliseconds() const;

		std::uint32_t getCurrentFramePerSeconds() const;

		const std::string& getFontfile() const;

		const Key& getKeyPressed() const;

		const Mouse& getMouseEvent() const;

		const Color& getForegroundOfCharacterInBufferAt(std::uint32_t index) const;

		const Color& getBackgroundOfCharacterInBufferAt(std::uint32_t index) const;

		const Color& getColorInCharacterColorAt(std::uint32_t index) const;

		const Color& getForeground() const;

		const Color& getBackground() const;

		const Color& getFadingColor() const;

		const Color& getFontKeyColor() const;

		// Setters

		void setFade(short _fade);

		void setLastFrameLength(float _lastFrameLength);

		void setOldFade(short _oldFade);

		void setWidth(std::uint32_t _width);

		void setHeight(std::uint32_t _height);

		void setRunning(bool _running);

		void setFontHasDoryenLayout(bool _fontHasDoryenLayout);

		void setFontHasRowLayout(bool _fontHasRowLayout);

		void setFontGrayscale(bool _fontGrayscale);

		void setMaxFontChars(std::uint32_t _maxFontChars);

		void setMinimumFrameLength(std::uint32_t _minFrameLength);

		void setFontCharHorizontalSize(std::uint32_t _fontCharHorizontalSize);

		void setFontCharVerticalSize(std::uint32_t _fontCharVerticalSize);

		void setFontWidth(std::uint32_t _fontWidth);

		void setFontHeight(std::uint32_t _fontHeight);

		void setAnyCharacterUpdated(bool _anyCharacterUpdated);

		void setSdlKey(std::uint32_t _sdlKey);

		void setRgbMask(std::uint32_t _rgbMask);

		void setNrgbMask(std::uint32_t _nRgbMask);

		void setFramePerSeconds(std::uint32_t _framePerSeconds);

		void setCurrentFramePerSeconds(std::uint32_t _currentFramePerSeconds);

		void setCharacterInBufferAt(std::uint32_t index, const Char& _char);

		void setCharacterColored(std::uint32_t index, bool isColored);

		void setCharacterDrawed(std::uint32_t index, bool isDrawed);

		void setLayoutCharacter(std::uint32_t index, std::uint32_t code);

		void setForeground(const Color& _foreground);

		void setBackground(const Color& _background);

		void setFadingColor(const Color& _fadingColor);

		void setCharOfCharacterInBufferAt(std::uint32_t index, int _char);

		void setForegroundOfCharacterInBufferAt(std::uint32_t index, const Color& _color);

		void setBackgroundOfCharacterInBufferAt(std::uint32_t index, const Color& _color);

		void setColorInCharacterColorAt(std::uint32_t index, const Color& _color);

		void setFontfile(const std::string& _fontfile);

		// Methods Pures

		virtual void onRenderer() = 0;

		virtual void draw() = 0;

		virtual void loadFont() = 0;

		virtual void showCursor(bool visible) = 0;

		virtual void setWindowTitle(const std::string& _title) = 0;

		virtual void registerCallback(std::unique_ptr<CallbackRender> _callback) = 0;
	};
}

#endif //LIBTCOD_RENDERER_HPP

#ifndef LIBTCOD_SDL_HPP
#define LIBTCOD_SDL_HPP

#include <SDL/SDL.h>

#include "Renderer/Renderer.hpp"

namespace Doryen
{
	class SDL : public Renderer
	{

	private:

		/**
		 * Minimum length for a frame (when fps are limited)
		 */
		int minimunFrameLength = 0;

		/**
		 * Minimum length for a frame (when fps are limited)
		 */
        int minimunFrameLengthBackup = 0;

        /**
         * Length of the last rendering loop.
         */
        float lastFrameLength = 0.0f;

		/**
		 * Number of frames in the last second.
		 */
		short framePerSecond = 0;

		/**
		 * Current number of frames.
		 */
		short currentFramePerSecond = 0;

		SDL_Surface* screen = nullptr;

		SDL_Surface* charmap = nullptr;

		// Methods

		void findResolution();

		static void convertToGenericEvent(SDL_Event& event, Key& key);

	public:

		// Construct

		SDL();

		~SDL() override;

		// Methods

		void onRenderer() override;

		void onExit() override;

		void draw() override;

		void loadFont() override;

		void sleepMilli(int milliseconds);

		// Setters

		void setFps(short value);

		void setWindowInFullscreen() override;

		void setWindowTitle(const std::string& _title) override;

		// Getters

		short getFps() const;

		int getElapsedMilli() const;

		unsigned getWidthInPixeles() const;

		unsigned getHeigthInPixeles() const;

		float getLasFrameLength() const;

		float getElapsedSeconds() const;

		Key getKeyPressed() override;

		Mouse getMouseEvent() override;
	};
}

#endif //LIBTCOD_SDL_HPP

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

		Key getKeyPressed() override;

		Mouse getMouseEvent() override;

		void loadFont() override;

		void setWindowTitle(const std::string& _title) override;

		void setWindowInFullscreen() override;

		// Setters

		void setFps(short value);

		// Getters

		unsigned getWidthInPixeles() const;

		unsigned getHeigthInPixeles() const;

		short getFps() const;

		float getLasFrameLength() const;

		void sleepMilli(int milliseconds);

		int getElapsedMilli() const;

		float getElapsedSeconds() const;
    };
}

#endif //LIBTCOD_SDL_HPP

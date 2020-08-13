#ifndef LIBTCOD_SDL_HPP
#define LIBTCOD_SDL_HPP

#include <SDL/SDL.h>
#include "Doryen/Renderer/Renderer.hpp"

namespace Doryen
{
	class SDL : public Renderer
	{

	private:

		bool eventPending = false;

		SDL_Event event;

		SDL_Surface* screen = nullptr;

		SDL_Surface* charmap = nullptr;

		// Methods

		void updateKeyEvents();

		void updateMouseEvents();

		void updateEventsQueue();

		void processEventsOfExit();

		static void convertToGenericEvent(SDL_Event& event, Key& key);

	public:

		// Construct

		/**
		 * @see Preconditions and post-conditions of class Renderer construct.
		 */
		SDL();

		~SDL() override;

		// Methods

		void onRenderer() override;

		void draw() override;

		void loadFont() override;

		// Setters

		void showCursor(bool visible) override;

		void setWindowTitle(const std::string& _title) override;

		void registerCallback(std::unique_ptr<CallbackRender> _callback) override;
	};
}

#endif //LIBTCOD_SDL_HPP

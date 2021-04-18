#ifndef LIBTCOD_SDL_HPP
#define LIBTCOD_SDL_HPP

#include <SDL/SDL.h>
#include <Doryen/Renderer/Renderer.hpp>
#include <Doryen/Renderer/Sdl/ImageSdl.hpp>
#include <Doryen/Renderer/Sdl/EventSdl.hpp>
#include <Doryen/Renderer/Sdl/MouseSdl.hpp>

namespace Doryen
{
	class SDL : public Renderer
	{

	private:

		bool eventPending = false;

		ImageSdl image{ 0, 0 };

		EventSdl eventSdl;

		MouseSdl mouseSdl;

		/**
		 * Default initialized to zero {0} for avoid warnings of Valgrind.
		 */
		SDL_Event event{ 0 };

		SDL_Surface* screen = nullptr;

		SDL_Surface* charmap = nullptr;

		// Methods

		void updateKeyEvents();

		void updateEventsQueue();

		void processEventsOfExit();

		void coloredCharacter(const SDL_Rect& sourceRect,
				const SDL_Surface* charmapBackup, Uint32* pix,
				const std::uint32_t hdelta);

		void fillOnlyNonKeyColorPixels(const SDL_Rect& sourceRect,
				const SDL_Surface* charmapBackup, const std::uint32_t hdelta,
				const std::uint32_t bpp, const std::uint32_t SDLFore,
				const Char& character);

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

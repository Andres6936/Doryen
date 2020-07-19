// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_SDL_HPP
#define DORYEN_SDL_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{

	class SDL : public ISample
	{

	private:

		bool renderCallback = false;

		const std::string text = "The SDL callback gives you access to the "
								 "screen surface so that you can alter the "
								 "pixels one by one using SDL API or any AP"
								 "I on top of SDL. SDL is used here to blur"
								 " the sample console.\n\nHit TAB to enable"
								 "/disable the callback. While enabled, it "
								 "will be active on other samples too.\n\nN"
								 "ote that the SDL callback only works with"
								 " SDL renderer.";

		void drawBackground() const;

		void drawText() const;

	public:

		SDL(const std::string _name, Console& _console);

		void render(KeyCode key, const Doryen::Mouse& mouse) override;

	};

}

#endif //DORYEN_SDL_HPP

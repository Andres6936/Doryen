// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_EVENTSDL_HPP
#define DORYEN_EVENTSDL_HPP

#include <SDL/SDL.h>
#include <Doryen/Event/Key.hpp>

namespace Doryen
{

	class EventSdl
	{

	public:

		/**
		 * Convert a SDL event to Generic event.
		 *
		 * @param event Event generated for SDL
		 * @param key Reference to generic event, in this parameter will be store the generic Event
		 */
		static void getGenericEvent(SDL_Event& event, Key& key) noexcept;

	};

}

#endif //DORYEN_EVENTSDL_HPP

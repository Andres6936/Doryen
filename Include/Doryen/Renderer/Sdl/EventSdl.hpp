// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_EVENTSDL_HPP
#define DORYEN_EVENTSDL_HPP

#include <SDL/SDL.h>
#include <Doryen/Event/Key.hpp>

namespace Doryen
{

	class EventSdl
	{

	protected:

		/**
		 * Reference to main listener event of SDL. Only-read
		 */
		const SDL_Event& event;

	public:

		/**
		 * Construct the reference to main listener event of SDL.
		 * @param event Refernece to main listener event of SDL.
		 */
		explicit EventSdl(SDL_Event& event) noexcept;

		/**
		 * Convert a SDL event to Generic event.
		 *
		 * @param event Event generated for SDL
		 * @param key Reference to generic event, in this parameter will be store the generic Event
		 */
		void getGenericEvent(Key& key) const noexcept;

	};

}

#endif //DORYEN_EVENTSDL_HPP

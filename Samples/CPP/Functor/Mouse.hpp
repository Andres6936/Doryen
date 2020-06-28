// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_MOUSE_HPP
#define DORYEN_MOUSE_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{

	class Mouse
	{

	private:

		Console sample;

	public:

		void operator()(KeyCode key, const Doryen::Mouse& mouse);

	};

}

#endif //DORYEN_MOUSE_HPP

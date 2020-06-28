// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_COLOR_HPP
#define DORYEN_COLOR_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{
	class Color
	{

	private:

		Console secondary;

	public:

		void operator()(KeyCode key, const Mouse& mouse);

	};
}

#endif //DORYEN_COLOR_HPP

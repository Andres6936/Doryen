// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_FOV_HPP
#define DORYEN_FOV_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{
	class FOV
	{

	private:

		Console sample;

	public:

		void operator()(KeyCode key, const Mouse& mouse);

	};
}

#endif //DORYEN_FOV_HPP

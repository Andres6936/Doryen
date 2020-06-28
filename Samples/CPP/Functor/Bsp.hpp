// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_BSP_HPP
#define DORYEN_BSP_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{
	class BSP
	{

	private:

		Console sample;

	public:

		void operator()(KeyCode key, const Mouse& mouse);

	};
}

#endif //DORYEN_BSP_HPP

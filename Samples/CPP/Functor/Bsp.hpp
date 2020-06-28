// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_BSP_HPP
#define DORYEN_BSP_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class BSP : private ISample
	{

	private:

		const Console sample;

	public:

		BSP(const std::string _name, const Console& _console);

		void operator()(KeyCode key, const Mouse& mouse);

	};
}

#endif //DORYEN_BSP_HPP

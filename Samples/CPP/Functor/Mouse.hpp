// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_MOUSE_HPP
#define DORYEN_MOUSE_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{

	class Mouse : public ISample
	{

	private:

		Console sample;

	public:

		Mouse(const std::string _name, const Console& _console);

		void operator()(KeyCode key, const Doryen::Mouse& mouse);

	};

}

#endif //DORYEN_MOUSE_HPP

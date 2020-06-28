// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_OFFSCREEN_HPP
#define DORYEN_OFFSCREEN_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class Offscreen : public ISample
	{

	private:

		Console sample;

	public:

		Offscreen(const std::string _name, const Console& _console);

		void operator()(KeyCode key, const Mouse& mouse);

	};
}

#endif //DORYEN_OFFSCREEN_HPP

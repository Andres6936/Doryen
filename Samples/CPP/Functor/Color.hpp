// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_COLOR_HPP
#define DORYEN_COLOR_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class Color : public ISample
	{

	private:

		const Console secondary;

	public:

		Color(const std::string _name, const Console& _console);

		void operator()(KeyCode key, const Mouse& mouse);

	};
}

#endif //DORYEN_COLOR_HPP

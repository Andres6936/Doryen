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

	public:

		Color(const std::string _name, std::reference_wrapper<Console> _console);

		void render(KeyCode key, const Mouse& mouse) override;

	};
}

#endif //DORYEN_COLOR_HPP

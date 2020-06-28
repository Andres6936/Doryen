// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_FOV_HPP
#define DORYEN_FOV_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class FOV : public ISample
	{

	public:

		FOV(const std::string _name, std::reference_wrapper<Console> _console);

		void render(KeyCode key, const Mouse& mouse) override;

	};
}

#endif //DORYEN_FOV_HPP

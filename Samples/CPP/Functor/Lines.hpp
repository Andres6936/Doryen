// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_LINES_HPP
#define DORYEN_LINES_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class Lines : public ISample
	{

	public:

		Lines(const std::string _name, std::reference_wrapper<Console> _console);

		BackgroundFlag switchBackgroundFlag(BackgroundFlag flag);

		void render(KeyCode key, const Mouse& mouse) override;

	};
}


#endif //DORYEN_LINES_HPP

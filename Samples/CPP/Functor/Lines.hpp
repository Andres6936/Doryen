// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_LINES_HPP
#define DORYEN_LINES_HPP

#include <Doryen/Doryen.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class Lines : public ISample
	{

	public:

		Lines(Console& _console);

		BlendModes switchBackgroundFlag(BlendModes flag);

		void render(KeyCode key, const Mouse& mouse) override;

	};
}


#endif //DORYEN_LINES_HPP

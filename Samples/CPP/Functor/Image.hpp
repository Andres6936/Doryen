// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_IMAGE_HPP
#define DORYEN_IMAGE_HPP

#include <Doryen/Doryen.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{

	class Image : public ISample
	{

	public:

		Image(Console& _console);

		void render(KeyCode key, const Mouse& mouse) override;

	};

}

#endif //DORYEN_IMAGE_HPP

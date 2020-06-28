// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_IMAGE_HPP
#define DORYEN_IMAGE_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{

	class Image
	{

	private:

		Console sample;

	public:

		void operator()(KeyCode key, const Mouse& mouse);

	};

}

#endif //DORYEN_IMAGE_HPP

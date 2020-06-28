// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_PATH_HPP
#define DORYEN_PATH_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{
	class Path
	{

	private:

		Console sample;

	public:

		void operator()(KeyCode key, const Mouse& mouse);

	};

}

#endif //DORYEN_PATH_HPP

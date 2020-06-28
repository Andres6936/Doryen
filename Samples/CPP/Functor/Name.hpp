// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_NAME_HPP
#define DORYEN_NAME_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{

	class Name
	{

	private:

		Console sample;

	public:

		void operator()(KeyCode key, const Mouse& mouse);

	};

}

#endif //DORYEN_NAME_HPP

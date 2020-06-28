// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_NAME_HPP
#define DORYEN_NAME_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{

	class Name : private ISample
	{

	private:

		Console sample;

	public:

		Name(const std::string _name, const Console& _console);

		void operator()(KeyCode key, const Mouse& mouse);

	};

}

#endif //DORYEN_NAME_HPP

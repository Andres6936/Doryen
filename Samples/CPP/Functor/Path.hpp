// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_PATH_HPP
#define DORYEN_PATH_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class Path : public ISample
	{

	private:

		Console sample;

	public:

		Path(const std::string _name, const Console& _console);

		void operator()(KeyCode key, const Mouse& mouse);

	};

}

#endif //DORYEN_PATH_HPP

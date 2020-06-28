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

	private:

		const Console sample;

	public:

		Lines(const std::string _name, const Console& _console);

		BackgroundFlag switchBackgroundFlag(BackgroundFlag flag);

		void operator()(KeyCode key, const Mouse& mouse);

	};
}


#endif //DORYEN_LINES_HPP

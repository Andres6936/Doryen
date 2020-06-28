// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_LINES_HPP
#define DORYEN_LINES_HPP

#include <Doryen/libtcod.hpp>

using namespace Doryen;

namespace Functor
{
	class Lines
	{

	private:

		Console sample;

	public:

		BackgroundFlag switchBackgroundFlag(BackgroundFlag flag);

		void operator()(KeyCode key, const Mouse& mouse);

	};
}


#endif //DORYEN_LINES_HPP

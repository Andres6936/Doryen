// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_COLOR_HPP
#define DORYEN_COLOR_HPP

#include <array>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class Color : public ISample
	{

	private:

		/**
		 * Random corner colors
		 */
		std::array<Doryen::Color, 4> cornerColors;

		// Methods Private

		void prepareRandomCornerColors();

	public:

		Color(const std::string _name, Console& _console);

		void render(KeyCode key, const Mouse& mouse) override;

	};
}

#endif //DORYEN_COLOR_HPP

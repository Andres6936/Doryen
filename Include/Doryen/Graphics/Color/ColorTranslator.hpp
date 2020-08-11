// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_COLORTRANSLATOR_HPP
#define DORYEN_COLORTRANSLATOR_HPP

#include <string_view>

namespace Doryen
{

	// Forward definition (Avoid compilation problems)
	class Color;

	class ColorTranslator
	{

	private:

		/**
		 * To return value of a char. For example, 2 is  returned for '2'.
		 * 10 is returned for 'A', 11 for 'B'.
		 *
		 * @param c Character to get the value in decimal.
		 * @return The value of char in base 10 (decimal).
		 */
		static std::int32_t valueOfChar(char c);

		/**
		 * Function that convert a number in base (b) to base 10 (decimal).
		 *
		 * @param string Representation of number in another base (different of 10).
		 * @param base Can be 2, 4, 8, 16
		 * @return The number in base 10 (decimal).
		 */
		static std::int32_t toInt32(std::string_view string, std::uint8_t base);

	public:

		static Color fromHTML(std::string_view htmlColor);

	};

}

#endif //DORYEN_COLORTRANSLATOR_HPP

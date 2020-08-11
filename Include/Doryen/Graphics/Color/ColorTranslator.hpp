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

		static std::int32_t valueOfChar(char c);

		static std::int32_t toInt32(std::string_view string, std::uint8_t base);

	public:

		static Color fromHTML(std::string_view htmlColor);

	};

}

#endif //DORYEN_COLORTRANSLATOR_HPP

// Joan Andrés (@Andres6936) Github.

#include <Doryen/Graphics/Color/Color.hpp>
#include "Doryen/Graphics/Color/ColorTranslator.hpp"

using namespace Doryen;

Color ColorTranslator::fromHTML(std::string_view htmlColor)
{
	// Return a empty color (0, 0, 0, 255)
	if (htmlColor.empty()) return {};

	// #RRGGBB or #RGB
	if (htmlColor[0] == '#' and (htmlColor.size() == 7 or htmlColor.size() == 4))
	{
		// Parse the format: #RRGGBB
		if (htmlColor.size() == 7)
		{
			std::int32_t r = toInt32(htmlColor.substr(1, 2), 16);
			std::int32_t g = toInt32(htmlColor.substr(3, 2), 16);
			std::int32_t b = toInt32(htmlColor.substr(5, 2), 16);

			return { r, g, b };
		}
			// Parse the format: #RGB
		else
		{
			const char r = htmlColor[1];
			const char g = htmlColor[2];
			const char b = htmlColor[3];

			std::int32_t cR = toInt32(std::string{ r } + std::string{ r }, 16);
			std::int32_t cG = toInt32(std::string{ g } + std::string{ g }, 16);
			std::int32_t cB = toInt32(std::string{ b } + std::string{ b }, 16);

			return { cR, cG, cB };
		}
	}

	// Return a empty color (0, 0, 0, 255)
	return {};
}

std::int32_t ColorTranslator::toInt32(std::string_view string, const std::uint8_t base)
{
	std::int32_t power = 1;
	std::int32_t number = 0;

	// Begin from last position (Rest 1, because the last
	// character is \0 [null terminated])
	for (std::int32_t i = string.size() - 1; i >= 0; --i)
	{
		number += valueOfChar(string[i]) * power;
		power = power * base;
	}

	return number;
}

std::int32_t ColorTranslator::valueOfChar(char c)
{
	if (c >= '0' and c <= '9')
	{
		return static_cast<std::int32_t>(c - '0');
	}
	else
	{
		return static_cast<std::int32_t>(c - 'A' + 10);
	}
}

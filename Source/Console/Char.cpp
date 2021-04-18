#include "Doryen/Console/Char.hpp"

// Constructor for Default

Doryen::Char::Char() noexcept
{
	setCharacter(' ');
}

// Getters

int Doryen::Char::getCharacter() const
{
	return static_cast<std::int32_t>(value >> 32);
}

void Doryen::Char::setCharacter(std::uint64_t character)
{
	value <<= 32;
	value >>= 32;
	character <<= 32;
	value ^= character;
}

int Doryen::Char::getCharacterFont() const
{
	std::uint64_t character = value << 32;
	character >>= 32;

	return static_cast<std::int32_t>(character);
}

void Doryen::Char::setCharacterFont(std::uint64_t characterFont)
{
	value >>= 32;
	value <<= 32;
	value ^= characterFont;
}

const Doryen::Color& Doryen::Char::getForeground() const
{
	return foreground;
}

void Doryen::Char::setForeground(const Doryen::Color& _foreground)
{
	foreground = _foreground;
}

const Doryen::Color& Doryen::Char::getBackground() const
{
	return background;
}

void Doryen::Char::setBackground(const Doryen::Color& _background)
{
	background = _background;
}

bool Doryen::Char::isDirt() const
{
	return dirt;
}

void Doryen::Char::setDirt(bool _dirt)
{
	dirt = _dirt;
}


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
	// Remember, the character is localized in the side left, the schema
	// is - C F - where C are the first 32 bits and F are the last 32 bits.
	// Need set the side left (aka the first 32 bits) of value variable with
	// the value of character parameter, so that first clear the bits of side
	// left and then set the value.
	value <<= 32;
	// The total of bits in the value variable is of 64 bits, the left side
	// have in total 32 bits, the bitwise fill with zeros this bits.
	value >>= 32;
	// Positing the bits of character parameter for that match with schema
	// of value variable. It is - C F -
	// The schema before of apply the bitwise for the character parameter is: - 0 C -
	// The schema after of apply the bitwise for the character parameter is: - C 0 -
	character <<= 32;
	// The operator "bitwise XOR (exclusive
	// OR)" allow interchange the values of
	// component C without affect the
	// another components (F).
	// See:
	// - C F - The schema of value
	// - C 0 - The schema of character parameter
	// Apply the operation xor the result is:
	// - C F -
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


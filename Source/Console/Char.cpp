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
	// Clear the bits of character (it is the first 32 bits).
	std::uint64_t character = value << 32;
	// Recover the schema for get the value of character in font
	// (it is the last 32 bits).
	character >>= 32;

	return static_cast<std::int32_t>(character);
}

void Doryen::Char::setCharacterFont(std::uint64_t characterFont)
{
	// Remember, the character in font is localized in the side right,
	// the schema is - C F - where C are the first 32 bits and F are the last 32 bits.
	// Need set the side right (aka the last 32 bits) of value variable with
	// the value of characterFont parameter, so that first clear the bits of side
	// right and then set the value.
	value >>= 32;
	// The total of bits in the value variable is of 64 bits, the right side
	// have in total 32 bits, the bitwise fill with zeros this bits.
	value <<= 32;
	// The operator "bitwise XOR (exclusive
	// OR)" allow interchange the values of
	// component F without affect the
	// another components (C).
	// See:
	// - C F - The schema of value
	// - 0 F - The schema of characterFont parameter
	// Apply the operation xor the result is:
	// - C F -
	value ^= characterFont;
}

const Doryen::Color& Doryen::Char::getForeground() const
{
	return foreground;
}

void Doryen::Char::setForeground(const Doryen::Color& _foreground)
{
	foreground.copy(_foreground);
}

const Doryen::Color& Doryen::Char::getBackground() const
{
	return background;
}

void Doryen::Char::setBackground(const Doryen::Color& _background)
{
	background.copy(_background);
}

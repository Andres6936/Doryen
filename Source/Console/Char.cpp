#include "Doryen/Console/Char.hpp"

int Doryen::Char::getCharacter() const
{
	return character;
}

void Doryen::Char::setCharacter(int _c)
{
	character = _c;
}

int Doryen::Char::getCharacterFont() const
{
	return characterFont;
}

void Doryen::Char::setCharacterFont(int _cf)
{
	characterFont = _cf;
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


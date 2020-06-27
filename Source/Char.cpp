#include "Doryen/Char.hpp"

int Doryen::Char::getC() const
{
	return c;
}

void Doryen::Char::setC(int _c)
{
	c = _c;
}

int Doryen::Char::getCf() const
{
	return cf;
}

void Doryen::Char::setCf(int _cf)
{
	cf = _cf;
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


// Joan Andrés (@Andres6936) Github.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest/doctest.h"
#include "Doryen/Doryen.hpp"

TEST_CASE("Verify the construction of character by default")
{
	Doryen::Char aChar{};

	CHECK(aChar.getCharacter() == ' ');
	CHECK(aChar.getBackground().equals({ 0, 0, 0 }));
	CHECK(aChar.getForeground().equals({ 255, 255, 255 }));
}

TEST_CASE("Verify the setter and getters of object")
{
	Doryen::Char aChar{};

	aChar.setCharacter('@');
	aChar.setCharacterFont(64);

	CHECK(aChar.getCharacter() == '@');
	CHECK(aChar.getCharacterFont() == 64);

	aChar.setCharacter('T');

	CHECK(aChar.getCharacter() == 'T');
	CHECK(aChar.getCharacterFont() == 64);
}
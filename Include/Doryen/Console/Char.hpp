#ifndef LIBTCOD_CHAR_HPP
#define LIBTCOD_CHAR_HPP

#include "Doryen/Graphics/Color/Color.hpp"

namespace Doryen
{

	/**
	 * A drawable character on the console.
	 */
	class Char
	{

	private:

		// Fields Members

		/**
		 * Store in the first 32 bits the character (ASCII code) and the
		 * last 32 bits store the character number in font.
		 *
		 * - 1 to 32 bit (Inclusive) = Character ASCII code
		 * - 33 to 64 bit (Inclusive) = Character number in font
		 */
		std::uint64_t value{ 0 };

		/**
		 * Foreground color
		 */
		Color foreground{ 255, 255, 255 };

		/**
		 * Background Color
		 */
		Color background{ 0, 0, 0 };

	public:

		// Construct

		Char() noexcept;

		// Getters

		int getCharacter() const;

		int getCharacterFont() const;

		const Color& getForeground() const;

		const Color& getBackground() const;

		// Setters

		void setCharacter(std::uint64_t character);

		void setCharacterFont(std::uint64_t characterFont);

		void setForeground(const Color& _foreground);

		void setBackground(const Color& _background);

	};
}


#endif //LIBTCOD_CHAR_HPP

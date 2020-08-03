#ifndef LIBTCOD_CHAR_HPP
#define LIBTCOD_CHAR_HPP

#include "Doryen/Graphics/Color/Color.hpp"

namespace Doryen
{

	/**
	 * A cell in the console
	 */
	class Char
	{

	private:

		// Fields Members

		/**
		 * Character ASCII code
		 */
		int character = ' ';

		/**
		 * Character number in font
		 */
		int characterFont = -1;

		/**
		 * Foreground color
		 */
		Color foreground{ 255, 255, 255 };

		/**
		 * Background Color
		 */
		Color background{ 0, 0, 0 };

		/**
		 * Cell modified since last flush?
		 */
		bool dirt = false;

	public:

		// Construct

		Char() noexcept = default;

		// Getters

		bool isDirt() const;

		int getCharacter() const;

		int getCharacterFont() const;

		const Color& getForeground() const;

		const Color& getBackground() const;

		// Setters

		void setCharacter(int _c);

		void setCharacterFont(int _cf);

		void setForeground(const Color& _foreground);

		void setBackground(const Color& _background);

		void setDirt(bool _dirt);

	};
}


#endif //LIBTCOD_CHAR_HPP

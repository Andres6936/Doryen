#ifndef LIBTCOD_CHAR_HPP
#define LIBTCOD_CHAR_HPP

#include "Color.hpp"

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
		int c = ' ';

		/**
		 * Character number in font
		 */
		int cf = -1;

		/**
		 * Foreground color
		 */
		Color foreground = Color(255, 255, 255);

		/**
		 * Background Color
		 */
		Color background = Color(0, 0, 0);

		/**
		 * Cell modified since last flush?
		 */
		bool dirt = false;

	public:

		// Construct

		Char() = default;

		// Getters

		bool isDirt() const;

		int getC() const;

		int getCf() const;

		const Color& getForeground() const;

		const Color& getBackground() const;

		// Setters

		void setC(int _c);

		void setCf(int _cf);

		void setForeground(const Color& _foreground);

		void setBackground(const Color& _background);

		void setDirt(bool _dirt);

	};
}


#endif //LIBTCOD_CHAR_HPP

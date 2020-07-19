/*
* libtcod 1.5.1
* Copyright (c) 2008,2009,2010,2012 Jice & Mingos
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice or Mingos may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE AND MINGOS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE OR MINGOS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _TCOD_COLOR_HPP
#define _TCOD_COLOR_HPP

#define MAX( a, b ) ((a)<(b)?(b):(a))
#define MIN( a, b ) ((a)>(b)?(b):(a))
#define ABS( a ) ((a)<0?-(a):(a))
#define CLAMP( a, b, x )        ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))
#define LERP( a, b, x ) ( a + x * (b - a) )

namespace Doryen
{
	enum class BackgroundFlag : short
	{
		NONE,
		SET,
		MULTIPLY,
		LIGHTEN,
		DARKEN,
		SCREEN,
		COLOR_DODGE,
		COLOR_BURN,
		ADD,
		ADDA,
		BURN,
		OVERLAY,
		ALPH,
		DEFAULT
	};

	/**
	 * The Color class is used to encapsulate colors in the default sRGB color space.
	 *
	 * Every color has an implicit alpha value of 1.0 or an explicit one provided in
	 * the constructor. The alpha value defines the transparency of a color and can
	 * be represented by a float value in the range 0.0 - 1.0 or 0 - 255.
	 *
	 * An alpha value of 1.0 or 255 means that the color is completely opaque and an
	 * alpha value of 0 or 0.0 means that the color is completely transparent.
	 *
	 * When constructing a Color with an explicit alpha or getting the color/alpha
	 * components of a Color, the color components are never premultiplied by the
	 * alpha component.
	 *
	 * The default color space for the Java 2D(tm) API is sRGB, a proposed standard
	 * RGB color space.
	 *
	 * For further information on sRGB,
	 * see http://www.w3.org/pub/WWW/Graphics/Color/sRGB.html
	 */
    class Color
    {

    public :

        short r;
        short g;
        short b;
		short a;

		/**
		 * Creates an opaque sRGB color with the component red, green, and blue
		 * set to 0.
		 *
		 * Alpha is defaulted to 255.
		 */
		Color();

		/**
		 * Constructor copy, create an color from exist color.
		 *
		 * @param _rhs Color to copy.
		 */
		Color(const Color& _rhs);

		/**
		 * Creates an opaque sRGB color with the specified red, green, and blue
		 * values in the range (0 - 255).
		 *
		 * The actual color used in rendering depends on finding the best match
		 * given the color space available for a given output device.
		 *
		 * Alpha is defaulted to 255.
		 *
		 * @param r The red component.
		 * @param g The green component.
		 * @param b The blue component.
		 */
        Color( short r, short g, short b );

        /**
         * Creates an sRGB color with the specified red, green, blue, and alpha
         * values in the range (0 - 255).
         *
         * @param r The red component.
         * @param g The green component.
         * @param b The blue component.
         * @param a The alpha component
         */
        Color( short r, short g, short b, short a );

		/**
		 * Determines whether another Color is equal to this Color.
		 *
		 * The result is true if and only if the argument is not null and is a
		 * Color object that has the same red, green, blue, and alpha values
		 * as this object.
		 *
		 * @param c The object to test for equality with this Color
		 * @return true if the objects are the same; false otherwise.
		 */
		bool equals(const Color& c) const;

		/**
		 * Transform the color as result of apply a operation.
		 *
		 * @param another Color to add
		 * @param flag Operation to make
		 */
		void trasformColor(const Color& another, BackgroundFlag flag);

		// Operators

		void operator=(const Color& _rhs);

		/**
		 * Multiply the color.
		 */
		void multiply(const Color& other);

		/**
		@PageName color
		@FuncTitle Multiply a color by a float
		@FuncDesc c1 = c2 * v =>
		c1.r = CLAMP(0, 255, c2.r * v)
		c1.g = CLAMP(0, 255, c2.g * v)
		c1.b = CLAMP(0, 255, c2.b * v)
		darkishRed = red * 0.5
	<table><tr><td style="background-color: rgb(128, 0, 0); width: 60px; height: 30px;"></td><td style="background-color: rgb(255, 0, 0); width: 60px;"></td><td style="width: 60px;"></td></tr></table>
	</tbody>
		@CppEx Doryen::TCODColor myDarkishRed = Doryen::TCODColor::lightRed * 0.5f;
		@CEx TCOD_color_t my_darkish_red = TCOD_color_multiply_scalar(TCOD_light_red, 0.5f);
		@PyEx myDarkishRed = litbcod.light_red * 0.5
		@C#Ex Doryen::TCODColor myDarkishRed = Doryen::TCODColor.lightRed.Multiply(0.5f);
		@LuaEx myDarkishRed = tcod.color.lightRed * 0.5
		*/
        Color operator*( float value ) const
        {
            int r = ( int ) ( this->r * value );
            int g = ( int ) ( this->g * value );
            int b = ( int ) ( this->b * value );

            r = CLAMP( 0, 255, r );
            g = CLAMP( 0, 255, g );
            b = CLAMP( 0, 255, b );

            return Color( r, g, b );
        }

		void add(const Color& other);

        /**
        @PageName color
        @FuncTitle Interpolate between two colors
        @FuncDesc   c1 = lerp (c2, c3, coef) => c1.r = c2.r  + (c3.r - c2.r ) * coef
                                  c1.g = c2.g  + (c3.g - c2.g ) * coef
                                  c1.b = c2.b  + (c3.b - c2.b ) * coef
    coef should be between 0.0 and 1.0 but you can as well use other values
    <table><tr><td style="background-color: rgb(96, 96, 96); color: rgb(255, 255, 255);" align="center">coef == 0.0f</td><td style="background-color: rgb(96, 96, 96); width: 60px;"></td><td style="background-color: rgb(255, 0, 0); width: 60px;"></td></tr>
        <tr><td style="background-color: rgb(135, 72, 72); color: rgb(255, 255, 255);" align="center">coef == 0.25f</td><td style="background-color: rgb(96, 96, 96); width: 60px;"></td><td style="background-color: rgb(255, 0, 0); width: 60px;"></td></tr>
        <tr><td style="background-color: rgb(175, 48, 48); color: rgb(255, 255, 255);" align="center">coef == 0.5f</td><td style="background-color: rgb(96, 96, 96); width: 60px;"></td><td style="background-color: rgb(255, 0, 0); width: 60px;"></td></tr>

        <tr><td style="background-color: rgb(215, 24, 24); color: rgb(255, 255, 255);" align="center">coef == 0.75f</td><td style="background-color: rgb(96, 96, 96); width: 60px;"></td><td style="background-color: rgb(255, 0, 0); width: 60px;"></td></tr>
        <tr><td style="background-color: rgb(255, 0, 0); color: rgb(255, 255, 255);" align="center">coef == 1.0f</td><td style="background-color: rgb(96, 96, 96); width: 60px;"></td><td style="background-color: rgb(255, 0, 0); width: 60px;"></td></tr></table>
        @CppEx Doryen::TCODColor myColor = Doryen::TCODColor::lerp ( Doryen::TCODColor::darkGrey, Doryen::TCODColor::lightRed,coef );
        @CEx TCOD_color_t my_color = TCOD_color_lerp ( TCOD_dark_grey, TCOD_light_red,coef);
        @PyEx my_color = libtcod.color_lerp ( libtcod.dark_grey, litbcod.light_red,coef)
        @C#Ex Doryen::TCODColor myColor = Doryen::TCODColor.Interpolate( Doryen::TCODColor.darkGrey, Doryen::TCODColor.lightRed, coef );
        @LuaEx myColor = tcod.color.Interpolate( tcod.color.darkGrey, tcod.color.lightRed, coef )
        */
        static Color lerp( const Color &a, const Color &b, float coef )
        {
            Color ret;
            ret.r = ( a.r + ( b.r - a.r ) * coef );
            ret.g = ( a.g + ( b.g - a.g ) * coef );
            ret.b = ( a.b + ( b.b - a.b ) * coef );
            return ret;
        }

		// Getters

		short getRed() const;

		short getGreen() const;

		short getBlue() const;

		short getAlpha() const;

		// Setters

		void setRed(short _r);

		void setGreen(short _g);

		void setBlue(short _b);

		void setAlpha(short _a);

		// Assets

		// grey levels
		static const Color GRAY_WARN_90;
		static const Color GRAY_WARN_30;
		static const Color GRAY_WARN_1;

		//sepia
		static const Color INK;

		// standard colors
		static const Color RED;
		static const Color GOLD;
		static const Color ORANGE;
		static const Color MINT;
		static const Color YELLOW;
		static const Color INDIGO;
		static const Color PRIMARY_LIGHTER;
		static const Color GREEN;
		static const Color PRIMARY;
		static const Color PRIMARY_VIVID;
		static const Color CYAN;
		static const Color PRIMARY_DARK;
		static const Color PRIMARY_DARKER;
		static const Color BLUE;
		static const Color SECONDARY_LIGHT;
		static const Color VIOLET;
		static const Color SECONDARY;
		static const Color BASE_LIGHTEST;
		static const Color MAGENTA;
		static const Color BASE_LIGHTER;
		static const Color BASE_LIGHT;

		// dark colors
		static const Color BASE;
		static const Color BASE_DARK;

		// darker colors
		static const Color BASE_DARKER;

		// darkest colors
		static const Color BASE_DARKEST;

		// light colors
		static const Color ACCENT_WARN_LIGHT;
		static const Color PRIMARY_LIGHT;

		//lighter colors
		static const Color SECONDARY_LIGHTER;

		// lightest colors
		static const Color SECONDARY_VIVID;
		static const Color SECONDARY_DARK;

		// metallic
		static const Color SECONDARY_DARKER;
		static const Color ACCENT_COOL_LIGHTER;
		static const Color ACCENT_COOL_LIGHT;
		static const Color ACCENT_COOL;

		// miscellaneous
		static const Color ACCENT_COOL_DARK;
		static const Color ACCENT_COOL_DARKER;
	};
}

Doryen::Color operator*( float value, const Doryen::Color &c );

#endif

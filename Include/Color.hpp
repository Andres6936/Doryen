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

// color constants uses to generate @ColorTable
/**
	@ColorCategory STANDARD COLORS
	@Color red 255,0,0
	@Color flame 255,63,0
	@Color orange 255,127,0
	@Color amber 255,191,0
	@Color yellow 255,255,0,
	@Color lime 191,255,0
	@Color chartreuse 127,255,0
	@Color green 0,255,0
	@Color sea 0,255,127
	@Color turquoise 0,255,191
	@Color cyan 0,255,255
	@Color sky 0,191,255
	@Color azure 0,127,255
	@Color blue 0,0,255
	@Color han 63,0,255
	@Color violet 127,0,255
	@Color purple 191,0,255
	@Color fuchsia 255,0,255
	@Color magenta 255,0,191
	@Color pink 255,0,127
	@Color crimson 255,0,63
	@ColorCategory METALLIC COLORS
	@Color brass 191,151,96
	@Color copper 196,136,124
	@Color gold 229,191,0
	@Color silver 203,203,203
	@ColorCategory MISCELLANEOUS COLORS
	@Color celadon 172,255,175
	@Color peach 255,159,127
	@ColorCategory GREYSCALE & SEPIA
	@Color grey 127,127,127
	@Color sepia 127,101,63
	@ColorCategory BLACK AND WHITE
	@Color black 0,0,0
	@Color white 255,255,255
*/

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
        Color( );

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

		/**
		@PageName color
		@FuncTitle Multiply two colors
		@FuncDesc c1 = c2 * c3 =>
		c1.r = c2.r * c3.r / 255
		c1.g = c2.g * c3.g / 255
		c1.b = c2.b * c3.b / 255
		darkishRed = darkGrey * red
	<table><tr><td style="background-color: rgb(96, 0, 0); width: 60px; height: 30px;"></td><td style="background-color: rgb(96, 96, 96); width: 60px;"></td><td style="background-color: rgb(255, 0, 0); width: 60px;"></td></tr></table>
		@CppEx Doryen::TCODColor myDarkishRed = Doryen::TCODColor::darkGrey * Doryen::TCODColor::lightRed;
		@CEx TCOD_color_t my_darkish_red = TCOD_color_multiply(TCOD_dark_grey, TCOD_light_red);
		@PyEx my_darkish_red = libtcod.dark_grey * libtcod.light_red
		@C#Ex Doryen::TCODColor myDarkishRed = Doryen::TCODColor.darkGrey.Multiply(Doryen::TCODColor.lightRed);
		@LuaEx myDarkishRed = tcod.color.darkGrey * tcod.color.lightRed
		*/
		Color operator*(const Color& a) const
		{
			Doryen::Color ret;
			ret.r = (((int)r) * a.r / 255);
			ret.g = (((int)g) * a.g / 255);
			ret.b = (((int)b) * a.b / 255);
			return ret;
		}

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

        /**
        @PageName color
        @FuncTitle Adding two colors
        @FuncDesc c1 = c1 + c2 => c1.r = MIN(255, c1.r + c2.r)
                      c1.g = MIN(255, c1.g + c2.g)
                      c1.b = MIN(255, c1.b + c2.b)
        lightishRed = red + darkGrey
    <table><tr><td style="background-color: rgb(255, 128, 128); width: 60px; height: 30px;"></td><td style="background-color: rgb(255, 0, 0); width: 60px;"></td><td style="background-color: rgb(128, 128, 128); width: 60px;"></td></tr></table>
        @CppEx Doryen::TCODColor myLightishRed = Doryen::TCODColor::red + Doryen::TCODColor::darkGrey
        @CEx TCOD_color_t my_lightish_red = TCOD_color_add(TCOD_red, TCOD_dark_grey);
        @PyEx myLightishRed = libtcod.red + libtcod.dark_grey
        @C#Ex Doryen::TCODColor myLightishRed = Doryen::TCODColor.red.Plus(Doryen::TCODColor.darkGrey)
        @LuaEx myLightishRed = tcod.color.red + tcod.color.darkGrey
        */
        Color operator+( const Color &a ) const
        {
            int r = ( int ) ( this->r ) + a.r;
			int g = (int)(this->g) + a.g;
			int b = (int)(this->b) + a.b;

			r = MIN(255, r);
			g = MIN(255, g);
			b = MIN(255, b);

			return Color(r, g, b);
		}

		void add(const Color& other);

		/**
		@PageName color
		@FuncTitle Subtract two colors
		@FuncDesc  c1 = c1 - c2 => c1.r = MAX(0, c1.r - c2.r)
					  c1.g = MAX(0, c1.g - c2.g)
					  c1.b = MAX(0, c1.b - c2.b)
		redish = red - darkGrey
	<table><tr><td style="background-color: rgb(127, 0, 0); width: 60px; height: 30px;"></td><td style="background-color: rgb(255, 0, 0); width: 60px;"></td><td style="background-color: rgb(128, 128, 128); width: 60px;"></td></tr></table>
		@CppEx Doryen::TCODColor myRedish = Doryen::TCODColor::red - Doryen::TCODColor::darkGrey
		@CEx TCOD_color_t my_redish = TCOD_color_subtract(TCOD_red, TCOD_dark_grey);
		@PyEx myRedish = libtcod.red - libtcod.dark_grey
		@C#Ex Doryen::TCODColor myRedish = Doryen::TCODColor.red.Minus(Doryen::TCODColor.darkGrey)
		@LuaEx myRedish = tcod.color.red - tcod.color.darkGrey
		*/
        Color operator-( const Color &a ) const
        {
            int r = ( int ) ( this->r ) - a.r;
            int g = ( int ) ( this->g ) - a.g;
            int b = ( int ) ( this->b ) - a.b;

            r = MAX( 0, r );
            g = MAX( 0, g );
            b = MAX( 0, b );

            return Color( r, g, b );
        }

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

		/**
		@PageName color
		@FuncTitle Generate a smooth color map
		@FuncDesc You can define a color map from an array of color keys. Colors will be interpolated between the keys.
		0 -> black
		4 -> red
		8 -> white
		Result :
		<table>
		<tbody><tr><td class="code"><pre>map[0]</pre></td><td style="background-color: rgb(0, 0, 0); color: rgb(255, 255, 255); width: 50px;" align="center">&nbsp;</td><td>black</td></tr>

		<tr><td class="code"><pre>map[1]</pre></td><td style="background-color: rgb(63, 0, 0); color: rgb(255, 255, 255); width: 50px;" align="center">&nbsp;</td></tr>
		<tr><td class="code"><pre>map[2]</pre></td><td style="background-color: rgb(127, 0, 0); color: rgb(255, 255, 255); width: 50px;" align="center">&nbsp;</td></tr>
		<tr><td class="code"><pre>map[3]</pre></td><td style="background-color: rgb(191, 0, 0); color: rgb(255, 255, 255); width: 50px;" align="center">&nbsp;</td></tr>
		<tr><td class="code"><pre>map[4]</pre></td><td style="background-color: rgb(255, 0, 0); color: rgb(255, 255, 255); width: 50px;" align="center">&nbsp;</td><td>red</td></tr>
		<tr><td class="code"><pre>map[5]</pre></td><td style="background-color: rgb(255, 63, 63); color: rgb(255, 255, 255); width: 50px;" align="center">&nbsp;</td></tr>
		<tr><td class="code"><pre>map[6]</pre></td><td style="background-color: rgb(255, 127, 127); color: rgb(255, 255, 255); width: 50px;" align="center">&nbsp;</td></tr>

		<tr><td class="code"><pre>map[7]</pre></td><td style="background-color: rgb(255, 191, 191); color: rgb(255, 255, 255); width: 50px;" align="center">&nbsp;</td></tr>
		<tr><td class="code"><pre>map[8]</pre></td><td style="background-color: rgb(255, 255, 255); color: rgb(255, 255, 255); width: 50px;" align="center">&nbsp;</td><td>white</td></tr>
	</tbody></table>
		@Cpp static void genMap(Doryen::TCODColor *map, int nbKey, Doryen::TCODColor const *keyColor, int const *keyIndex)
		@C void TCOD_gen_map(TCOD_color_t *map, int nb_key, TCOD_color_t const *key_color, int const *key_index)
		@Py color_gen_map(keyColor,keyIndex) # returns an array of colors
		@Param map	An array of colors to be filled by the function.
		@Param nbKey	Number of color keys
		@Param keyColor	Array of nbKey colors containing the color of each key
		@Param keyIndex	Array of nbKey integers containing the index of each key.
			If you want to fill the map array, keyIndex[0] must be 0 and keyIndex[nbKey-1] is the number of elements in map minus 1 but you can also use the function to fill only a part of the map array.
		@CppEx
			int idx[] = { 0, 4, 8 }; // indexes of the keys
			Doryen::TCODColor col[] = { Doryen::TCODColor( 0,0,0 ), Doryen::TCODColor(255,0,0), Doryen::TCODColor(255,255,255) }; // colors : black, red, white
			Doryen::TCODColor map[9];
			Doryen::TCODColor::genMap(map,3,col,idx);
		@CEx
			int idx[] = { 0, 4, 8 }; // indexes of the keys
			TCOD_color_t col[] = { { 0,0,0 }, {255,0,0}, {255,255,255} }; // colors : black, red, white
			TCOD_color_t map[9];
			TCOD_color_gen_map(map,3,col,idx);
		@PyEx
			idx = [ 0, 4, 8 ] # indexes of the keys
			col = [ libtcod.Color( 0,0,0 ), libtcod.Color( 255,0,0 ), libtcod.Color(255,255,255) ] # colors : black, red, white
			map=libtcod.color_gen_map(col,idx)
		*/
		static void genMap(Color* map, int nbKey, Color const* keyColor, int const* keyIndex);

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
		static const Color black;
		static const Color grey;
		static const Color white;

		//sepia
		static const Color sepia;

		// standard colors
		static const Color red;
		static const Color flame;
        static const Color orange;
        static const Color amber;
        static const Color yellow;
        static const Color lime;
        static const Color chartreuse;
        static const Color green;
        static const Color sea;
        static const Color turquoise;
        static const Color cyan;
        static const Color sky;
        static const Color azure;
        static const Color blue;
        static const Color han;
        static const Color violet;
        static const Color purple;
        static const Color fuchsia;
        static const Color magenta;
        static const Color pink;
        static const Color crimson;

        // dark colors
        static const Color darkYellow;
        static const Color darkBlue;

        // darker colors
        static const Color darkerBlue;

        // darkest colors
        static const Color darkestFlame;

        // light colors
        static const Color lightYellow;
        static const Color lightBlue;

        //lighter colors
        static const Color lighterBlue;

        // lightest colors
        static const Color lightestYellow;
        static const Color lightestBlue;

        // metallic
        static const Color brass;
        static const Color copper;
        static const Color gold;
        static const Color silver;

        // miscellaneous
        static const Color celadon;
        static const Color peach;
    };
}

Doryen::Color operator*( float value, const Doryen::Color &c );

#endif

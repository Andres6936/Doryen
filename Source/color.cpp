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

#include <color.hpp>

#include "libtcod.hpp"
#include "libtcod_int.h"

// grey levels
const Doryen::Color Doryen::Color::black( TCOD_BLACK );

const Doryen::Color Doryen::Color::grey( TCOD_GREY );

const Doryen::Color Doryen::Color::white( TCOD_WHITE );

//sepia
const Doryen::Color Doryen::Color::sepia( TCOD_SEPIA );

// standard colors
const Doryen::Color Doryen::Color::red( TCOD_RED );

const Doryen::Color Doryen::Color::flame( TCOD_FLAME );

const Doryen::Color Doryen::Color::orange( TCOD_ORANGE );

const Doryen::Color Doryen::Color::amber( TCOD_AMBER );

const Doryen::Color Doryen::Color::yellow( TCOD_YELLOW );

const Doryen::Color Doryen::Color::lime( TCOD_LIME );

const Doryen::Color Doryen::Color::chartreuse( TCOD_CHARTREUSE );

const Doryen::Color Doryen::Color::green( TCOD_GREEN );

const Doryen::Color Doryen::Color::sea( TCOD_SEA );

const Doryen::Color Doryen::Color::turquoise( TCOD_TURQUOISE );

const Doryen::Color Doryen::Color::cyan( TCOD_CYAN );

const Doryen::Color Doryen::Color::sky( TCOD_SKY );

const Doryen::Color Doryen::Color::azure( TCOD_AZURE );

const Doryen::Color Doryen::Color::blue( TCOD_BLUE );

const Doryen::Color Doryen::Color::han( TCOD_HAN );

const Doryen::Color Doryen::Color::violet( TCOD_VIOLET );

const Doryen::Color Doryen::Color::purple( TCOD_PURPLE );

const Doryen::Color Doryen::Color::fuchsia( TCOD_FUCHSIA );

const Doryen::Color Doryen::Color::magenta( TCOD_MAGENTA );

const Doryen::Color Doryen::Color::pink( TCOD_PINK );

const Doryen::Color Doryen::Color::crimson( TCOD_CRIMSON );

// dark colors
const Doryen::Color Doryen::Color::darkYellow( TCOD_DARK_YELLOW );

const Doryen::Color Doryen::Color::darkBlue( TCOD_DARK_BLUE );

// darker colors
const Doryen::Color Doryen::Color::darkerBlue( TCOD_DARKER_BLUE );

// darkest colors
const Doryen::Color Doryen::Color::darkestFlame( TCOD_DARKEST_FLAME );

// light colors
const Doryen::Color Doryen::Color::lightYellow( TCOD_LIGHT_YELLOW );

const Doryen::Color Doryen::Color::lightBlue( TCOD_LIGHT_BLUE );

// lighter colors
const Doryen::Color Doryen::Color::lighterBlue( TCOD_LIGHTER_BLUE );

// lightest colors
const Doryen::Color Doryen::Color::lightestYellow( TCOD_LIGHTEST_YELLOW );

const Doryen::Color Doryen::Color::lightestBlue( TCOD_LIGHTEST_BLUE );

//special
const Doryen::Color Doryen::Color::brass( TCOD_BRASS );

const Doryen::Color Doryen::Color::copper( TCOD_COPPER );

const Doryen::Color Doryen::Color::gold( TCOD_GOLD );

const Doryen::Color Doryen::Color::silver( TCOD_SILVER );

//miscellaneous
const Doryen::Color Doryen::Color::celadon( TCOD_CELADON );

const Doryen::Color Doryen::Color::peach( TCOD_PEACH );

#ifndef TCOD_HAIKU
#endif

// non member operators
Doryen::Color operator*( float value, const Doryen::Color &c )
{
	return c*value;
}

void Doryen::Color::genMap( Doryen::Color *map, int nbKey, Doryen::Color const *keyColor, int const *keyIndex )
{
	for (int segment=0; segment < nbKey-1; segment++) {
		int idxStart=keyIndex[segment];
		int idxEnd=keyIndex[segment+1];
		int idx;
		for ( idx=idxStart;idx <= idxEnd; idx++) {
            map[ idx ] = Doryen::Color::lerp( keyColor[ segment ], keyColor[ segment + 1 ],
                                              ( float ) ( idx - idxStart ) / ( idxEnd - idxStart ));
		}
	}
}

Doryen::Color::Color( short r, short g, short b )
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
}

Doryen::Color::Color( )
{
    r = 0;
    g = 0;
    b = 0;
    a = 255;
}

Doryen::Color::Color( short r, short g, short b, short a )
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

bool Doryen::Color::equals( const Color &c ) const
{
    return r == c.r && g == c.g && b == c.b && a == c.a;
}


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
const TCODColor TCODColor::black(TCOD_BLACK);
const TCODColor TCODColor::grey(TCOD_GREY);
const TCODColor TCODColor::white(TCOD_WHITE);

//sepia
const TCODColor TCODColor::sepia(TCOD_SEPIA);

// standard colors
const TCODColor TCODColor::red(TCOD_RED);
const TCODColor TCODColor::flame(TCOD_FLAME);
const TCODColor TCODColor::orange(TCOD_ORANGE);
const TCODColor TCODColor::amber(TCOD_AMBER);
const TCODColor TCODColor::yellow(TCOD_YELLOW);
const TCODColor TCODColor::lime(TCOD_LIME);
const TCODColor TCODColor::chartreuse(TCOD_CHARTREUSE);
const TCODColor TCODColor::green(TCOD_GREEN);
const TCODColor TCODColor::sea(TCOD_SEA);
const TCODColor TCODColor::turquoise(TCOD_TURQUOISE);
const TCODColor TCODColor::cyan(TCOD_CYAN);
const TCODColor TCODColor::sky(TCOD_SKY);
const TCODColor TCODColor::azure(TCOD_AZURE);
const TCODColor TCODColor::blue(TCOD_BLUE);
const TCODColor TCODColor::han(TCOD_HAN);
const TCODColor TCODColor::violet(TCOD_VIOLET);
const TCODColor TCODColor::purple(TCOD_PURPLE);
const TCODColor TCODColor::fuchsia(TCOD_FUCHSIA);
const TCODColor TCODColor::magenta(TCOD_MAGENTA);
const TCODColor TCODColor::pink(TCOD_PINK);
const TCODColor TCODColor::crimson(TCOD_CRIMSON);

// dark colors
const TCODColor TCODColor::darkYellow(TCOD_DARK_YELLOW);
const TCODColor TCODColor::darkBlue(TCOD_DARK_BLUE);

// darker colors
const TCODColor TCODColor::darkerBlue(TCOD_DARKER_BLUE);

// darkest colors
const TCODColor TCODColor::darkestFlame(TCOD_DARKEST_FLAME);

// light colors
const TCODColor TCODColor::lightYellow(TCOD_LIGHT_YELLOW);
const TCODColor TCODColor::lightBlue(TCOD_LIGHT_BLUE);

// lighter colors
const TCODColor TCODColor::lighterBlue(TCOD_LIGHTER_BLUE);

// lightest colors
const TCODColor TCODColor::lightestYellow(TCOD_LIGHTEST_YELLOW);
const TCODColor TCODColor::lightestBlue(TCOD_LIGHTEST_BLUE);

//special
const TCODColor TCODColor::brass(TCOD_BRASS);
const TCODColor TCODColor::copper(TCOD_COPPER);
const TCODColor TCODColor::gold(TCOD_GOLD);
const TCODColor TCODColor::silver(TCOD_SILVER);

//miscellaneous
const TCODColor TCODColor::celadon(TCOD_CELADON);
const TCODColor TCODColor::peach(TCOD_PEACH);

#ifndef TCOD_HAIKU
#endif

// non member operators
TCODColor operator *(float value, const TCODColor &c) {
	return c*value;
}

void TCODColor::genMap(TCODColor *map, int nbKey, TCODColor const *keyColor, int const *keyIndex) {
	for (int segment=0; segment < nbKey-1; segment++) {
		int idxStart=keyIndex[segment];
		int idxEnd=keyIndex[segment+1];
		int idx;
		for ( idx=idxStart;idx <= idxEnd; idx++) {
			map[idx]=TCODColor::lerp(keyColor[segment],keyColor[segment+1],(float)(idx-idxStart)/(idxEnd-idxStart));
		}
	}
}

TCODColor::TCODColor( short r, short g, short b )
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
}

TCODColor::TCODColor( )
{
    r = 0;
    g = 0;
    b = 0;
    a = 255;
}

TCODColor::TCODColor( short r, short g, short b, short a )
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

bool TCODColor::equals( const TCODColor &c ) const
{
    return r == c.r && g == c.g && b == c.b && a == c.a;
}


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

#include "libtcod.h"
#include "libtcod_int.h"

/* grey levels */
const TCOD_color_t TCOD_black={TCOD_BLACK};
const TCOD_color_t TCOD_white={TCOD_WHITE};

TCOD_color_t TCOD_color_multiply (TCOD_color_t c1, TCOD_color_t c2) {
	TCOD_color_t ret;
	ret.r=(uint8)(((int)c1.r)*c2.r/255);
	ret.g=(uint8)(((int)c1.g)*c2.g/255);
	ret.b=(uint8)(((int)c1.b)*c2.b/255);
	return ret;
}

TCOD_color_t TCOD_color_lerp(TCOD_color_t c1, TCOD_color_t c2, float coef) {
	TCOD_color_t ret;
	ret.r=(uint8)(c1.r+(c2.r-c1.r)*coef);
	ret.g=(uint8)(c1.g+(c2.g-c1.g)*coef);
	ret.b=(uint8)(c1.b+(c2.b-c1.b)*coef);
	return ret;
}

void TCOD_color_gen_map(TCOD_color_t *map, int nb_key, TCOD_color_t const  *key_color, int const  *key_index) {
	int segment=0;
	for (segment=0; segment < nb_key-1; segment++) {
		int idx_start=key_index[segment];
		int idx_end=key_index[segment+1];
		int idx;
		for ( idx=idx_start;idx <= idx_end; idx++) {
			map[idx]=TCOD_color_lerp(key_color[segment],key_color[segment+1],(float)(idx-idx_start)/(idx_end-idx_start));
		}
	}
}



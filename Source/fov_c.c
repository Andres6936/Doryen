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

#include <stdlib.h>
#include "libtcod.h"
#include "libtcod_int.h"
#include <string.h>

void TCOD_map_compute_fov(TCOD_map_t map, int player_x, int player_y, int max_radius, bool light_walls, TCOD_fov_algorithm_t algo) {
	TCOD_IFNOT(map != NULL) return;
	switch(algo) {
		case FOV_DIAMOND : TCOD_map_compute_fov_diamond_raycasting(map,player_x,player_y,max_radius,light_walls); break;
		case FOV_PERMISSIVE_0 :
		case FOV_PERMISSIVE_1 :
		case FOV_PERMISSIVE_2 :
		case FOV_PERMISSIVE_3 :
		case FOV_PERMISSIVE_4 :
		case FOV_PERMISSIVE_5 :
		case FOV_PERMISSIVE_6 :
		case FOV_PERMISSIVE_7 :
		case FOV_PERMISSIVE_8 :
			TCOD_map_compute_fov_permissive2(map,player_x,player_y,max_radius,light_walls, algo-FOV_PERMISSIVE_0);
		break;
		case FOV_RESTRICTIVE : TCOD_map_compute_fov_restrictive_shadowcasting(map,player_x,player_y,max_radius,light_walls); break;
		default:break;
	}
}


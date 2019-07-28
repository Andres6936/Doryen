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
#include <math.h>
#include "libtcod.h"
#include "libtcod_int.h"

/* grey levels */
const TCOD_color_t TCOD_black={TCOD_BLACK};
const TCOD_color_t TCOD_white={TCOD_WHITE};

TCOD_color_t TCOD_color_HSV(float h, float s, float v) {
	TCOD_color_t ret;
	int i;
	float f, p, q, t;

	if( s == 0 ) {
		/* achromatic (grey) */
		ret.r = ret.g = ret.b = (uint8)(v*255.0f+0.5f);
	}
	else {
		while (h < 0.0f) h += 360.0f; /*for H < 0 */
		while (h >= 360.0f) h -= 360.0f; /*for H >= 360 */
		h /= 60;
		i = (int)(h); /*hue sector 0-5 */
		f = h - i;			/* factorial part of h */
		p = v * ( 1 - s );
		q = v * ( 1 - s * f );
		t = v * ( 1 - s * ( 1 - f ) );

		switch (i) {
			case 0:
				ret.r = (uint8)(v*255.0f+0.5f);
				ret.g = (uint8)(t*255.0f+0.5f);
				ret.b = (uint8)(p*255.0f+0.5f);
				break;
			case 1:
				ret.r = (uint8)(q*255.0f+0.5f);
				ret.g = (uint8)(v*255.0f+0.5f);
				ret.b = (uint8)(p*255.0f+0.5f);
				break;
			case 2:
				ret.r = (uint8)(p*255.0f+0.5f);
				ret.g = (uint8)(v*255.0f+0.5f);
				ret.b = (uint8)(t*255.0f+0.5f);
				break;
			case 3:
				ret.r = (uint8)(p*255.0f+0.5f);
				ret.g = (uint8)(q*255.0f+0.5f);
				ret.b = (uint8)(v*255.0f+0.5f);
				break;
			case 4:
				ret.r = (uint8)(t*255.0f+0.5f);
				ret.g = (uint8)(p*255.0f+0.5f);
				ret.b = (uint8)(v*255.0f+0.5f);
				break;
			default:
				ret.r = (uint8)(v*255.0f+0.5f);
				ret.g = (uint8)(p*255.0f+0.5f);
				ret.b = (uint8)(q*255.0f+0.5f);
				break;
		}
	}
	return ret;
}

bool TCOD_color_equals (TCOD_color_t c1, TCOD_color_t  c2) {
	return (c1.r == c2.r && c1.g == c2.g && c1.b == c2.b);
}

TCOD_color_t TCOD_color_add (TCOD_color_t c1, TCOD_color_t c2) {
	TCOD_color_t ret;
	int r,g,b;
	r = (int)(c1.r) + c2.r;
	g = (int)(c1.g) + c2.g;
	b = (int)(c1.b) + c2.b;
	r=MIN(255,r);
	g=MIN(255,g);
	b=MIN(255,b);
	ret.r=(uint8)r;
	ret.g=(uint8)g;
	ret.b=(uint8)b;
	return ret;
}

TCOD_color_t TCOD_color_subtract (TCOD_color_t c1, TCOD_color_t c2) {
	TCOD_color_t ret;
	int r,g,b;
	r = (int)(c1.r) - c2.r;
	g = (int)(c1.g) - c2.g;
	b = (int)(c1.b) - c2.b;
	r=MAX(0,r);
	g=MAX(0,g);
	b=MAX(0,b);
	ret.r=(uint8)r;
	ret.g=(uint8)g;
	ret.b=(uint8)b;
	return ret;
}

TCOD_color_t TCOD_color_multiply (TCOD_color_t c1, TCOD_color_t c2) {
	TCOD_color_t ret;
	ret.r=(uint8)(((int)c1.r)*c2.r/255);
	ret.g=(uint8)(((int)c1.g)*c2.g/255);
	ret.b=(uint8)(((int)c1.b)*c2.b/255);
	return ret;
}

TCOD_color_t TCOD_color_multiply_scalar (TCOD_color_t c1, float value) {
	TCOD_color_t ret;
	int r,g,b;
	r = (int)(c1.r * value);
	g = (int)(c1.g * value);
	b = (int)(c1.b * value);
	ret.r=(uint8)CLAMP(0,255,r);
	ret.g=(uint8)CLAMP(0,255,g);
	ret.b=(uint8)CLAMP(0,255,b);
	return ret;
}

TCOD_color_t TCOD_color_lerp(TCOD_color_t c1, TCOD_color_t c2, float coef) {
	TCOD_color_t ret;
	ret.r=(uint8)(c1.r+(c2.r-c1.r)*coef);
	ret.g=(uint8)(c1.g+(c2.g-c1.g)*coef);
	ret.b=(uint8)(c1.b+(c2.b-c1.b)*coef);
	return ret;
}

/* 0<= h < 360, 0 <= s <= 1, 0 <= v <= 1 */
void TCOD_color_set_HSV(TCOD_color_t *c, float h, float s, float v)
{
	int i;
	float f, p, q, t;

	if( s == 0.0f ) {
		/* achromatic (grey) */
		c->r = c->g = c->b = (uint8)(v*255.0f+0.5f);
		return;
	}

	while (h < 0.0f) h += 360.0f; /*for H < 0 */
	while (h >= 360.0f) h -= 360.0f; /*for H >= 360 */
	h /= 60.0f;			/* sector 0 to 5 */
	i = (int)floor( h );
	f = h - i;			/* factorial part of h */
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			c->r = (uint8)(v*255.0f+0.5f);
			c->g = (uint8)(t*255.0f+0.5f);
			c->b = (uint8)(p*255.0f+0.5f);
			break;
		case 1:
			c->r = (uint8)(q*255.0f+0.5f);
			c->g = (uint8)(v*255.0f+0.5f);
			c->b = (uint8)(p*255.0f+0.5f);
			break;
		case 2:
			c->r = (uint8)(p*255.0f+0.5f);
			c->g = (uint8)(v*255.0f+0.5f);
			c->b = (uint8)(t*255.0f+0.5f);
			break;
		case 3:
			c->r = (uint8)(p*255.0f+0.5f);
			c->g = (uint8)(q*255.0f+0.5f);
			c->b = (uint8)(v*255.0f+0.5f);
			break;
		case 4:
			c->r = (uint8)(t*255.0f+0.5f);
			c->g = (uint8)(p*255.0f+0.5f);
			c->b = (uint8)(v*255.0f+0.5f);
			break;
		default:
			c->r = (uint8)(v*255.0f+0.5f);
			c->g = (uint8)(p*255.0f+0.5f);
			c->b = (uint8)(q*255.0f+0.5f);
			break;
	}
}

void TCOD_color_get_HSV(TCOD_color_t c, float *h, float *s, float *v)
{
  uint8 imax,imin;
	float min, max, delta;

	imax = ( c.r > c.g ?
			( c.r > c.b ? c.r : c.b )
			: ( c.g > c.b ? c.g : c.b) );
	imin = ( c.r < c.g ?
			( c.r < c.b ? c.r : c.b )
			: ( c.g < c.b ? c.g : c.b) );
	max = imax/255.0f;
	min = imin/255.0f;
	*v = max; /* v */

	delta = max - min;
	if( max != 0.0f ) *s = delta / max; /* s */
	else
	{
		*s = 0.0f; /* s */
		*h = 0.0f; /* h */
		return;
	}

	if( c.r == imax ) *h = ( c.g - c.b ) / (255.0f * delta);		/* between yellow & magenta */
	else if( c.g == imax )	*h = 2.0f + ( c.b - c.r ) / (255.0f * delta);	/* between cyan & yellow */
	else *h = 4.0f + ( c.r - c.g ) / (255.0f * delta);	/* between magenta & cyan */

	*h *= 60.0f; /* degrees */
	if( *h < 0 ) *h += 360.0f;
}

float TCOD_color_get_hue (TCOD_color_t c) {
	uint8 max = MAX(c.r,MAX(c.g,c.b));
	uint8 min = MIN(c.r,MIN(c.g,c.b));
	float delta = (float)max - (float)min;
	float ret;
	if (delta == 0.0f) ret = 0.0f; /*achromatic, including black */
	else {
		if (c.r == max) ret = (float)(c.g - c.b) / delta;
		else if (c.g == max) ret = 2.0f + (float)(c.b - c.r) / delta;
		else ret = 4.0f + (float)(c.r - c.g) / delta;
		ret *= 60.0f;
		if (ret < 0.0f) ret += 360.0f;
		if (ret >= 360.0f) ret -= 360.0f;
	}
	return ret;
}

void TCOD_color_set_hue (TCOD_color_t *c, float h) {
	float obsolete, s, v;
	TCOD_color_get_HSV(*c,&obsolete,&s,&v);
	*c = TCOD_color_HSV(h,s,v);
}

float TCOD_color_get_saturation (TCOD_color_t c) {
	float max = (float)(MAX(c.r,MAX(c.g,c.b)))/255.0f;
	float min = (float)(MIN(c.r,MIN(c.g,c.b)))/255.0f;
	float delta = max - min;
	if (max == 0.0f) return 0.0f;
	else return delta/max;
}

void TCOD_color_set_saturation (TCOD_color_t *c, float s) {
	float h, obsolete, v;
	TCOD_color_get_HSV(*c,&h,&obsolete,&v);
	*c = TCOD_color_HSV(h,s,v);
}

float TCOD_color_get_value (TCOD_color_t c) {
	return (float)(MAX(c.r,MAX(c.g,c.b)))/255.0f;
}

void TCOD_color_set_value (TCOD_color_t *c, float v) {
	float h, s, obsolete;
	TCOD_color_get_HSV(*c,&h,&s,&obsolete);
	*c = TCOD_color_HSV(h,s,v);
}

void TCOD_color_shift_hue (TCOD_color_t *c, float hshift) {
	float h, s, v;
	if (hshift == 0.0f) return;
	TCOD_color_get_HSV(*c,&h,&s,&v);
	*c = TCOD_color_HSV(h+hshift,s,v);
}

void TCOD_color_scale_HSV (TCOD_color_t *c, float scoef, float vcoef) {
	float h, s, v;
	TCOD_color_get_HSV(*c,&h,&s,&v);
	s = CLAMP(0.0f,1.0f,s*scoef);
	v = CLAMP(0.0f,1.0f,v*vcoef);
	*c = TCOD_color_HSV(h,s,v);
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



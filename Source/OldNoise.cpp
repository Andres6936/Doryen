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
#include <stdlib.h>
#include <cstring>
#include <iostream>

#include "Doryen/OldNoise.hpp"
#include "Doryen/Random/Number.hpp"

using namespace Doryen;

#define WAVELET_TILE_SIZE 32
#define WAVELET_ARAD 16

#define SIMPLEX_SCALE 0.5f
#define WAVELET_SCALE 2.0f

#define ABS(a) ((a)<0?-(a):(a))
#define CLAMP(a, b, x)        ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))
#define LERP(a, b, x) ( a + x * (b - a) )

float Perlin::lattice(int ix, float fx, int iy, float fy, int iz, float fz, int iw, float fw)
{
	std::array<int, 4> n = { ix, iy, iz, iw };

	std::array<float, 4> f = { fx, fy, fz, fw };

	int nIndex = 0;

	for (int i = 0; i < ndim; i++)
	{
		nIndex = map[(nIndex + n[i]) & 0xFF];
	}

	float value = 0;

	for (int i = 0; i < ndim; i++)
	{
		value += buffer[nIndex][i] * f[i];
	}

	return value;
}

Perlin::Perlin(const std::int32_t _dimensions, const float _lacunarity)
{
	ndim = _dimensions;
	lacunarity = _lacunarity;

	for (int i = 0; i < map.size(); ++i)
	{
		map[i] = i;

		for (int j = 0; j < ndim; j++)
		{
			buffer[i][j] = Random::Number::nextFloat(-0.5, 0.5);
		}

		normalize(buffer[i]);
	}

	int i = map.size();

	while (--i)
	{
		std::int32_t j = Random::Number::nextInteger(0, 255);
		std::swap(map[i], map[j]);
	}

	float f = 1.0f;

	for (int j = 0; j < Perlin::MAX_OCTAVES; j++)
	{
		/*exponent[i] = powf(f, -H); */
		exponent[j] = 1.0f / f;
		f *= lacunarity;
	}
}

void Perlin::normalize(float* f)
{
	float magnitude = 0;

	for (int i = 0; i < ndim; i++)
	{
		magnitude += f[i] * f[i];
	}

	magnitude = 1.0f / std::sqrt(magnitude);

	for (int i = 0; i < ndim; i++)
	{
		f[i] *= magnitude;
	}
}

#define CUBIC(a)    ( a * a * (3 - 2*a) )

float Perlin::noisePerlin(float* f)
{
	int n[Perlin::MAX_DIMENSIONS];            /* Indexes to pass to lattice function */
	int i;
	float r[Perlin::MAX_DIMENSIONS];        /* Remainders to pass to lattice function */
	float w[Perlin::MAX_DIMENSIONS];        /* Cubic values to pass to interpolation function */
	float value;

	for (i = 0; i < ndim; i++)
	{
		n[i] = std::floor(f[i]);
		r[i] = f[i] - n[i];
		w[i] = CUBIC(r[i]);
	}

	if (ndim == 1)
	{
		const auto a = lattice(n[0], r[0], 0, 0, 0, 0, 0, 0);
		const auto b = lattice(n[0] + 1, r[0] - 1, 0, 0, 0, 0, 0, 0);
		const auto x = w[0];

		value = LERP(a, b, x);
	}
	else if (ndim == 2)
	{
		const auto a = lattice(n[0], r[0], n[1], r[1], 0, 0, 0, 0);
		const auto b = lattice(n[0] + 1, r[0] - 1, n[1], r[1], 0, 0, 0, 0);
		const auto x = w[0];

		const auto anotherA = lattice(n[0], r[0], n[1] + 1, r[1] - 1, 0, 0, 0, 0);
		const auto anotherB = lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, 0, 0, 0, 0);

		value = LERP(LERP(a, b, x), LERP(anotherA, anotherB, x), w[1]);
	}
	else if (ndim == 3)
	{
		const auto a = lattice(n[0], r[0], n[1], r[1], n[2], r[2], 0, 0);
		const auto b = lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2], r[2], 0, 0);

		const auto anotherA = lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2], r[2], 0, 0);
		const auto anotherB = lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2], r[2], 0, 0);

		value = LERP(
				LERP(
						LERP(a, b, w[0]),
						LERP(anotherA, anotherB, w[0]), w[1]),
				LERP(
						LERP(lattice(n[0], r[0], n[1], r[1], n[2] + 1, r[2] - 1, 0, 0),
								lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2] + 1, r[2] - 1, 0, 0),
								w[0]),
						LERP(lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1, 0, 0),
								lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1, 0, 0),
								w[0]),
						w[1]),
				w[2]);
	}
		// 4 Dimension or any dimension not equal to 1, 2, 3
	else
	{
		const auto a = lattice(n[0], r[0], n[1], r[1], n[2], r[2], n[3], r[3]);
		const auto b = lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2], r[2], n[3], r[3]);

		const auto anotherA = lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2], r[2], n[3], r[3]);
		const auto anotherB = lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2], r[2], n[3], r[3]);

		value = LERP(LERP(LERP(LERP(a, b, w[0]),
				LERP(anotherA, anotherB, w[0]), w[1]),
				LERP(LERP(lattice(n[0], r[0], n[1], r[1], n[2] + 1, r[2] - 1, n[3], r[3]),
						lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2] + 1, r[2] - 1, n[3], r[3]),
						w[0]),
						LERP(lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1, 0, 0),
								lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1, n[3],
										r[3]),
								w[0]),
						w[1]),
				w[2]),
				LERP(LERP(LERP(lattice(n[0], r[0], n[1], r[1], n[2], r[2], n[3] + 1, r[3] - 1),
						lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2], r[2], n[3] + 1, r[3] - 1),
						w[0]),
						LERP(lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2], r[2], n[3] + 1, r[3] - 1),
								lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2], r[2], n[3] + 1,
										r[3] - 1),
								w[0]),
						w[1]),
						LERP(LERP(lattice(n[0], r[0], n[1], r[1], n[2] + 1, r[2] - 1, n[3] + 1, r[3] - 1),
								lattice(n[0] + 1, r[0] - 1, n[1], r[1], n[2] + 1, r[2] - 1, n[3] + 1,
										r[3] - 1),
								w[0]),
								LERP(lattice(n[0], r[0], n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1, 0, 0),
										lattice(n[0] + 1, r[0] - 1, n[1] + 1, r[1] - 1, n[2] + 1, r[2] - 1,
												n[3] + 1, r[3] - 1),
										w[0]),
								w[1]),
						w[2]),
				w[3]);
	}

	return CLAMP(-0.99999f, 0.99999f, value);
}

/* simplex noise, adapted from Ken Perlin's presentation at Siggraph 2001 */
/* and Stefan Gustavson implementation */

#define TCOD_NOISE_SIMPLEX_GRADIENT_1D(n, h, x) { float grad; h &= 0xF; grad=1.0f+(h & 7); if ( h & 8 ) grad = -grad; n = grad * x; }

#define TCOD_NOISE_SIMPLEX_GRADIENT_2D(n, h, x, y) { float u,v; h &= 0x7; if ( h < 4 ) { u=x; v=2.0f*y; } else { u=y; v=2.0f*x; } n = ((h & 1) ? -u : u) + ((h & 2) ? -v :v ); }

#define TCOD_NOISE_SIMPLEX_GRADIENT_3D(n, h, x, y, z) { float u,v; h &= 0xF; u = (h < 8 ? x : y); v = (h < 4 ? y : ( h == 12 || h == 14 ? x : z ) ); n= ((h & 1) ? -u : u ) + ((h & 2) ? -v : v); }

#define TCOD_NOISE_SIMPLEX_GRADIENT_4D(n, h, x, y, z, t) { float u,v,w; h &= 0x1F; u = (h < 24 ? x:y); v = (h < 16 ? y:z); w = (h < 8 ? z:t); n= ((h & 1) ? -u : u ) + ((h & 2) ? -v : v) + ((h & 4) ? -w : w);}

static float simplex[64][4] = {
		{ 0, 1, 2, 3 },
		{ 0, 1, 3, 2 },
		{ 0, 0, 0, 0 },
		{ 0, 2, 3, 1 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 2, 3, 0 },
		{ 0, 2, 1, 3 },
		{ 0, 0, 0, 0 },
		{ 0, 3, 1, 2 },
		{ 0, 3, 2, 1 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 3, 2, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 2, 0, 3 },
		{ 0, 0, 0, 0 },
		{ 1, 3, 0, 2 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 2, 3, 0, 1 },
		{ 2, 3, 1, 0 },
		{ 1, 0, 2, 3 },
		{ 1, 0, 3, 2 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 2, 0, 3, 1 },
		{ 0, 0, 0, 0 },
		{ 2, 1, 3, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 2, 0, 1, 3 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 3, 0, 1, 2 },
		{ 3, 0, 2, 1 },
		{ 0, 0, 0, 0 },
		{ 3, 1, 2, 0 },
		{ 2, 1, 0, 3 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 3, 1, 0, 2 },
		{ 0, 0, 0, 0 },
		{ 3, 2, 0, 1 },
		{ 3, 2, 1, 0 },

};

float Perlin::noiseSimplex(float* f)
{
	switch (ndim)
	{
	case 1 :
	{
		int i0 = (int)std::floor(f[0] * SIMPLEX_SCALE);
		int i1 = i0 + 1;
		float x0 = f[0] * SIMPLEX_SCALE - i0;
		float x1 = x0 - 1.0f;
		float t0 = 1.0f - x0 * x0;
		float t1 = 1.0f - x1 * x1;
		float n0, n1;
		t0 = t0 * t0;
		t1 = t1 * t1;
		i0 = map[i0 & 0xFF];
		TCOD_NOISE_SIMPLEX_GRADIENT_1D(n0, i0, x0);
		n0 *= t0 * t0;
		i1 = map[i1 & 0xFF];
		TCOD_NOISE_SIMPLEX_GRADIENT_1D(n1, i1, x1);
		n1 *= t1 * t1;
		return 0.25f * (n0 + n1);
	}
		break;
	case 2 :
	{
#define F2 0.366025403f  /* 0.5f * (sqrtf(3.0f)-1.0f); */
#define G2 0.211324865f  /* (3.0f - sqrtf(3.0f))/6.0f; */

		float s = (f[0] + f[1]) * F2 * SIMPLEX_SCALE;
		float xs = f[0] * SIMPLEX_SCALE + s;
		float ys = f[1] * SIMPLEX_SCALE + s;
		int i = std::floor(xs);
		int j = std::floor(ys);
		float t = (i + j) * G2;
		float xo = i - t;
		float yo = j - t;
		float x0 = f[0] * SIMPLEX_SCALE - xo;
		float y0 = f[1] * SIMPLEX_SCALE - yo;
		int i1, j1, ii = i % 256, jj = j % 256;
		float n0, n1, n2, x1, y1, x2, y2, t0, t1, t2;
		if (x0 > y0)
		{
			i1 = 1;
			j1 = 0;
		}
		else
		{
			i1 = 0;
			j1 = 1;
		}
		x1 = x0 - i1 + G2;
		y1 = y0 - j1 + G2;
		x2 = x0 - 1.0f + 2.0f * G2;
		y2 = y0 - 1.0f + 2.0f * G2;
		t0 = 0.5f - x0 * x0 - y0 * y0;
		if (t0 < 0.0f)
		{
			n0 = 0.0f;
		}
		else
		{
			int idx = (ii + map[jj]) & 0xFF;
			t0 *= t0;
			idx = map[idx];
			TCOD_NOISE_SIMPLEX_GRADIENT_2D(n0, idx, x0, y0);
			n0 *= t0 * t0;
		}
		t1 = 0.5f - x1 * x1 - y1 * y1;
		if (t1 < 0.0f)
		{
			n1 = 0.0f;
		}
		else
		{
			int idx = (ii + i1 + map[(jj + j1) & 0xFF]) & 0xFF;
			t1 *= t1;
			idx = map[idx];
			TCOD_NOISE_SIMPLEX_GRADIENT_2D(n1, idx, x1, y1);
			n1 *= t1 * t1;
		}
		t2 = 0.5f - x2 * x2 - y2 * y2;
		if (t2 < 0.0f)
		{
			n2 = 0.0f;
		}
		else
		{
			int idx = (ii + 1 + map[(jj + 1) & 0xFF]) & 0xFF;
			t2 *= t2;
			idx = map[idx];
			TCOD_NOISE_SIMPLEX_GRADIENT_2D(n2, idx, x2, y2);
			n2 *= t2 * t2;
		}
		return 40.0f * (n0 + n1 + n2);
	}
		break;
	case 3 :
	{
#define F3 0.333333333f
#define G3 0.166666667f
		float n0, n1, n2, n3;
		float s = (f[0] + f[1] + f[2]) * F3 * SIMPLEX_SCALE;
		float xs = f[0] * SIMPLEX_SCALE + s;
		float ys = f[1] * SIMPLEX_SCALE + s;
		float zs = f[2] * SIMPLEX_SCALE + s;
		int i = std::floor(xs);
		int j = std::floor(ys);
		int k = std::floor(zs);
		float t = (float)(i + j + k) * G3;
		float xo = i - t;
		float yo = j - t;
		float zo = k - t;
		float x0 = f[0] * SIMPLEX_SCALE - xo;
		float y0 = f[1] * SIMPLEX_SCALE - yo;
		float z0 = f[2] * SIMPLEX_SCALE - zo;
		int i1, j1, k1, i2, j2, k2, ii, jj, kk;
		float x1, y1, z1, x2, y2, z2, x3, y3, z3, t0, t1, t2, t3;
		if (x0 >= y0)
		{
			if (y0 >= z0)
			{
				i1 = 1;
				j1 = 0;
				k1 = 0;
				i2 = 1;
				j2 = 1;
				k2 = 0;
			}
			else if (x0 >= z0)
			{
				i1 = 1;
				j1 = 0;
				k1 = 0;
				i2 = 1;
				j2 = 0;
				k2 = 1;
			}
			else
			{
				i1 = 0;
				j1 = 0;
				k1 = 1;
				i2 = 1;
				j2 = 0;
				k2 = 1;
			}
		}
		else
		{
			if (y0 < z0)
			{
				i1 = 0;
				j1 = 0;
				k1 = 1;
				i2 = 0;
				j2 = 1;
				k2 = 1;
			}
			else if (x0 < z0)
			{
				i1 = 0;
				j1 = 1;
				k1 = 0;
				i2 = 0;
				j2 = 1;
				k2 = 1;
			}
			else
			{
				i1 = 0;
				j1 = 1;
				k1 = 0;
				i2 = 1;
				j2 = 1;
				k2 = 0;
			}
		}
		x1 = x0 - i1 + G3;
		y1 = y0 - j1 + G3;
		z1 = z0 - k1 + G3;
		x2 = x0 - i2 + 2.0f * G3;
		y2 = y0 - j2 + 2.0f * G3;
		z2 = z0 - k2 + 2.0f * G3;
		x3 = x0 - 1.0f + 3.0f * G3;
		y3 = y0 - 1.0f + 3.0f * G3;
		z3 = z0 - 1.0f + 3.0f * G3;
		ii = i % 256;
		jj = j % 256;
		kk = k % 256;
		t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
		if (t0 < 0.0f) n0 = 0.0f;
		else
		{
			int idx = map[(ii + map[(jj + map[kk]) & 0xFF]) & 0xFF];
			t0 *= t0;
			TCOD_NOISE_SIMPLEX_GRADIENT_3D(n0, idx, x0, y0, z0);
			n0 *= t0 * t0;
		}
		t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
		if (t1 < 0.0f) n1 = 0.0f;
		else
		{
			int idx = map[(ii + i1 + map[(jj + j1 + map[(kk + k1) & 0xFF]) & 0xFF]) & 0xFF];
			t1 *= t1;
			TCOD_NOISE_SIMPLEX_GRADIENT_3D(n1, idx, x1, y1, z1);
			n1 *= t1 * t1;
		}
		t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
		if (t2 < 0.0f) n2 = 0.0f;
		else
		{
			int idx = map[(ii + i2 + map[(jj + j2 + map[(kk + k2) & 0xFF]) & 0xFF]) & 0xFF];
			t2 *= t2;
			TCOD_NOISE_SIMPLEX_GRADIENT_3D(n2, idx, x2, y2, z2);
			n2 *= t2 * t2;
		}
		t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
		if (t3 < 0.0f) n3 = 0.0f;
		else
		{
			int idx = map[(ii + 1 + map[(jj + 1 + map[(kk + 1) & 0xFF]) & 0xFF]) & 0xFF];
			t3 *= t3;
			TCOD_NOISE_SIMPLEX_GRADIENT_3D(n3, idx, x3, y3, z3);
			n3 *= t3 * t3;
		}
		return 32.0f * (n0 + n1 + n2 + n3);

	}
		break;
	case 4 :
	{
#define F4 0.309016994f /* (sqrtf(5.0f)-1.0f)/4.0f */
#define G4 0.138196601f /* (5.0f - sqrtf(5.0f))/20.0f */
		float n0, n1, n2, n3, n4;

		const float s = (f[0] + f[1] + f[2] + f[3]) * F4 * SIMPLEX_SCALE;
		const float xs = f[0] * SIMPLEX_SCALE + s;
		const float ys = f[1] * SIMPLEX_SCALE + s;
		const float zs = f[2] * SIMPLEX_SCALE + s;
		const float ws = f[3] * SIMPLEX_SCALE + s;

		const int i = std::floor(xs);
		const int j = std::floor(ys);
		const int k = std::floor(zs);
		const int l = std::floor(ws);

		const float t = (float)(i + j + k + l) * G4;
		const float xo = i - t;
		const float yo = j - t;
		const float zo = k - t;
		const float wo = l - t;

		const float x0 = f[0] * SIMPLEX_SCALE - xo;
		const float y0 = f[1] * SIMPLEX_SCALE - yo;
		const float z0 = f[2] * SIMPLEX_SCALE - zo;
		const float w0 = f[3] * SIMPLEX_SCALE - wo;

		const int c1 = (x0 > y0 ? 32 : 0);
		const int c2 = (x0 > z0 ? 16 : 0);
		const int c3 = (y0 > z0 ? 8 : 0);
		const int c4 = (x0 > w0 ? 4 : 0);
		const int c5 = (y0 > w0 ? 2 : 0);
		const int c6 = (z0 > w0 ? 1 : 0);
		const int c = c1 + c2 + c3 + c4 + c5 + c6;

		const int i1 = simplex[c][0] >= 3 ? 1 : 0;
		const int j1 = simplex[c][1] >= 3 ? 1 : 0;
		const int k1 = simplex[c][2] >= 3 ? 1 : 0;
		const int l1 = simplex[c][3] >= 3 ? 1 : 0;

		const int i2 = simplex[c][0] >= 2 ? 1 : 0;
		const int j2 = simplex[c][1] >= 2 ? 1 : 0;
		const int k2 = simplex[c][2] >= 2 ? 1 : 0;
		const int l2 = simplex[c][3] >= 2 ? 1 : 0;

		const int i3 = simplex[c][0] >= 1 ? 1 : 0;
		const int j3 = simplex[c][1] >= 1 ? 1 : 0;
		const int k3 = simplex[c][2] >= 1 ? 1 : 0;
		const int l3 = simplex[c][3] >= 1 ? 1 : 0;

		const float x1 = x0 - i1 + G4;
		const float y1 = y0 - j1 + G4;
		const float z1 = z0 - k1 + G4;
		const float w1 = w0 - l1 + G4;
		const float x2 = x0 - i2 + 2.0f * G4;
		const float y2 = y0 - j2 + 2.0f * G4;
		const float z2 = z0 - k2 + 2.0f * G4;
		const float w2 = w0 - l2 + 2.0f * G4;
		const float x3 = x0 - i3 + 3.0f * G4;
		const float y3 = y0 - j3 + 3.0f * G4;
		const float z3 = z0 - k3 + 3.0f * G4;
		const float w3 = w0 - l3 + 3.0f * G4;
		const float x4 = x0 - 1.0f + 4.0f * G4;
		const float y4 = y0 - 1.0f + 4.0f * G4;
		const float z4 = z0 - 1.0f + 4.0f * G4;
		const float w4 = w0 - 1.0f + 4.0f * G4;

		const int ii = i % 256;
		const int jj = j % 256;
		const int kk = k % 256;
		const int ll = l % 256;

		float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0 - w0 * w0;
		if (t0 < 0.0f) n0 = 0.0f;
		else
		{
			int idx = map[(ii + map[(jj + map[(kk + map[ll]) & 0xFF]) & 0xFF]) & 0xFF];
			t0 *= t0;
			TCOD_NOISE_SIMPLEX_GRADIENT_4D(n0, idx, x0, y0, z0, w0);
			n0 *= t0 * t0;
		}
		float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1 - w1 * w1;
		if (t1 < 0.0f) n1 = 0.0f;
		else
		{
			int idx = map[(ii + i1 +
						   map[(jj + j1 + map[(kk + k1 + map[(ll + l1) & 0xFF]) & 0xFF]) &
							   0xFF]) & 0xFF];
			t1 *= t1;
			TCOD_NOISE_SIMPLEX_GRADIENT_4D(n1, idx, x1, y1, z1, w1);
			n1 *= t1 * t1;
		}
		float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2 - w2 * w2;
		if (t2 < 0.0f) n2 = 0.0f;
		else
		{
			int idx = map[(ii + i2 +
						   map[(jj + j2 + map[(kk + k2 + map[(ll + l2) & 0xFF]) & 0xFF]) &
							   0xFF]) & 0xFF];
			t2 *= t2;
			TCOD_NOISE_SIMPLEX_GRADIENT_4D(n2, idx, x2, y2, z2, w2);
			n2 *= t2 * t2;
		}
		float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3 - w3 * w3;
		if (t3 < 0.0f) n3 = 0.0f;
		else
		{
			int idx = map[(ii + i3 +
						   map[(jj + j3 + map[(kk + k3 + map[(ll + l3) & 0xFF]) & 0xFF]) &
							   0xFF]) & 0xFF];
			t3 *= t3;
			TCOD_NOISE_SIMPLEX_GRADIENT_4D(n3, idx, x3, y3, z3, w3);
			n3 *= t3 * t3;
		}
		float t4 = 0.6f - x4 * x4 - y4 * y4 - z4 * z4 - w4 * w4;
		if (t4 < 0.0f) n4 = 0.0f;
		else
		{
			int idx = map[(ii + 1 +
						   map[(jj + 1 + map[(kk + 1 + map[(ll + 1) & 0xFF]) & 0xFF]) &
							   0xFF]) & 0xFF];
			t4 *= t4;
			TCOD_NOISE_SIMPLEX_GRADIENT_4D(n4, idx, x4, y4, z4, w4);
			n4 *= t4 * t4;
		}
		return 27.0f * (n0 + n1 + n2 + n3 + n4);

	}
		break;
	}
	return 0.0f;
}

/* wavelet noise, adapted from Robert L. Cook and Tony Derose 'Wavelet noise' paper */

static int absmod(int x, int n)
{
	int m = x % n;
	return m < 0 ? m + n : m;
}

static void TCOD_noise_wavelet_downsample(float* from, float* to, int stride)
{
	static float acoeffs[2 * WAVELET_ARAD] = {
			0.000334f, -0.001528f, 0.000410f, 0.003545f, -0.000938f, -0.008233f, 0.002172f, 0.019120f,
			-0.005040f, -0.044412f, 0.011655f, 0.103311f, -0.025936f, -0.243780f, 0.033979f, 0.655340f,
			0.655340f, 0.033979f, -0.243780f, -0.025936f, 0.103311f, 0.011655f, -0.044412f, -0.005040f,
			0.019120f, 0.002172f, -0.008233f, -0.000938f, 0.003546f, 0.000410f, -0.001528f, 0.000334f,
	};
	static float* a = &acoeffs[WAVELET_ARAD];
	int i;
	for (i = 0; i < WAVELET_TILE_SIZE / 2; i++)
	{
		int k;
		to[i * stride] = 0;
		for (k = 2 * i - WAVELET_ARAD; k < 2 * i + WAVELET_ARAD; k++)
		{
			to[i * stride] += a[k - 2 * i] * from[absmod(k, WAVELET_TILE_SIZE) * stride];
		}
	}
}

static void TCOD_noise_wavelet_upsample(float* from, float* to, int stride)
{
	static float pcoeffs[4] = { 0.25f, 0.75f, 0.75f, 0.25f };
	static float* p = &pcoeffs[2];
	int i;
	for (i = 0; i < WAVELET_TILE_SIZE; i++)
	{
		int k;
		to[i * stride] = 0;
		for (k = i / 2; k < i / 2 + 1; k++)
		{
			to[i * stride] += p[i - 2 * k] * from[absmod(k, WAVELET_TILE_SIZE / 2) * stride];
		}
	}
}

void Perlin::prepareNoiseWavelet()
{
	int ix, iy, iz, i, sz = WAVELET_TILE_SIZE * WAVELET_TILE_SIZE * WAVELET_TILE_SIZE * sizeof(float);

	std::vector<float> temp1;
	temp1.resize(sz);

	std::vector<float> temp2;
	temp2.resize(sz);

	std::vector<float> noise;
	noise.resize(sz);

	int offset;
	for (i = 0; i < WAVELET_TILE_SIZE * WAVELET_TILE_SIZE * WAVELET_TILE_SIZE; i++)
	{

		noise[i] = Random::Number::nextFloat(-1.0f, 1.0f);
	}
	for (iy = 0; iy < WAVELET_TILE_SIZE; iy++)
	{
		for (iz = 0; iz < WAVELET_TILE_SIZE; iz++)
		{
			i = iy * WAVELET_TILE_SIZE + iz * WAVELET_TILE_SIZE * WAVELET_TILE_SIZE;
			TCOD_noise_wavelet_downsample(&noise[i], &temp1[i], 1);
			TCOD_noise_wavelet_upsample(&temp1[i], &temp2[i], 1);
		}
	}
	for (ix = 0; ix < WAVELET_TILE_SIZE; ix++)
	{
		for (iz = 0; iz < WAVELET_TILE_SIZE; iz++)
		{
			i = ix + iz * WAVELET_TILE_SIZE * WAVELET_TILE_SIZE;
			TCOD_noise_wavelet_downsample(&temp2[i], &temp1[i], WAVELET_TILE_SIZE);
			TCOD_noise_wavelet_upsample(&temp1[i], &temp2[i], WAVELET_TILE_SIZE);
		}
	}
	for (ix = 0; ix < WAVELET_TILE_SIZE; ix++)
	{
		for (iy = 0; iy < WAVELET_TILE_SIZE; iy++)
		{
			i = ix + iy * WAVELET_TILE_SIZE;
			TCOD_noise_wavelet_downsample(&temp2[i], &temp1[i], WAVELET_TILE_SIZE * WAVELET_TILE_SIZE);
			TCOD_noise_wavelet_upsample(&temp1[i], &temp2[i], WAVELET_TILE_SIZE * WAVELET_TILE_SIZE);
		}
	}
	for (i = 0; i < WAVELET_TILE_SIZE * WAVELET_TILE_SIZE * WAVELET_TILE_SIZE; i++)
	{
		noise[i] -= temp2[i];
	}
	offset = WAVELET_TILE_SIZE / 2;
	if ((offset & 1) == 0) offset++;
	for (i = 0, ix = 0; ix < WAVELET_TILE_SIZE; ix++)
	{
		for (iy = 0; iy < WAVELET_TILE_SIZE; iy++)
		{
			for (iz = 0; iz < WAVELET_TILE_SIZE; iz++)
			{
				temp1[i++] = noise[absmod(ix + offset, WAVELET_TILE_SIZE)
								   + absmod(iy + offset, WAVELET_TILE_SIZE) * WAVELET_TILE_SIZE
								   + absmod(iz + offset, WAVELET_TILE_SIZE) * WAVELET_TILE_SIZE * WAVELET_TILE_SIZE
				];
			}
		}
	}
	for (i = 0; i < WAVELET_TILE_SIZE * WAVELET_TILE_SIZE * WAVELET_TILE_SIZE; i++)
	{
		noise[i] += temp1[i];
	}

	std::copy(noise.begin(), noise.end(), std::back_inserter(waveletTileData));
}

float Perlin::noiseWavelet(float* f)
{
	float pf[3];

	int p[3], c[3], mid[3], n = WAVELET_TILE_SIZE;
	float w[3][3], t, result = 0.0f;
	if (ndim > 3) return 0.0f; /* not supported */

	if (waveletTileData.empty()) prepareNoiseWavelet();

	for (int i = 0; i < ndim; i++) pf[i] = f[i] * WAVELET_SCALE;

	for (int i = ndim; i < 3; i++) pf[i] = 0.0f;

	for (int i = 0; i < 3; i++)
	{
		mid[i] = (int)ceil(pf[i] - 0.5f);
		t = mid[i] - (pf[i] - 0.5f);
		w[i][0] = t * t * 0.5f;
		w[i][2] = (1.0f - t) * (1.0f - t) * 0.5f;
		w[i][1] = 1.0f - w[i][0] - w[i][2];
	}

	for (p[2] = -1; p[2] <= 1; p[2]++)
	{
		for (p[1] = -1; p[1] <= 1; p[1]++)
		{
			for (p[0] = -1; p[0] <= 1; p[0]++)
			{
				float weight = 1.0f;
				for (int i = 0; i < 3; i++)
				{
					c[i] = absmod(mid[i] + p[i], n);
					weight *= w[i][p[i] + 1];
				}
				result += weight * waveletTileData[c[2] * n * n + c[1] * n + c[0]];
			}
		}
	}
	return CLAMP(-1.0f, 1.0f, result);
}

// Getters

TypeNoise Perlin::getNoiseType() const
{
	return noise_type;
}

// Setters

void Perlin::setNoiseType(TypeNoise _noiseType)
{
	noise_type = _noiseType;
}

#define DELTA                1e-6f
#define SWAP(a, b, t)        t = a; a = b; b = t

float Perlin::noiseFBM(float* f, float octaves)
{
	float tf[Perlin::MAX_DIMENSIONS];
	/* Initialize locals */
	double value = 0;
	int i, j;
	std::memcpy(tf, f, sizeof(float) * ndim);

	/* Inner loop of spectral construction, where the fractal is built */
	for (i = 0; i < (int)octaves; i++)
	{
		if (getNoiseType() == TypeNoise::Perlin)
		{
			value += noisePerlin(tf) * exponent[i];
		}
		else if (getNoiseType() == TypeNoise::Simplex)
		{
			value += noiseSimplex(tf) * exponent[i];
		}
		else if (getNoiseType() == TypeNoise::Wavelet)
		{
			value += noiseWavelet(tf) * exponent[i];
		}
		else
		{
			std::cerr << "Problem, type noise unknown\n";
		}

		for (j = 0; j < ndim; j++) tf[j] *= lacunarity;
	}

	/* Take care of remainder in octaves */
	octaves -= (int)octaves;

	if (octaves > DELTA)
	{
		if (getNoiseType() == TypeNoise::Perlin)
		{
			value += octaves * noisePerlin(tf) * exponent[i];
		}
		else if (getNoiseType() == TypeNoise::Simplex)
		{
			value += octaves * noiseSimplex(tf) * exponent[i];
		}
		else if (getNoiseType() == TypeNoise::Wavelet)
		{
			value += octaves * noiseWavelet(tf) * exponent[i];
		}
		else
		{
			std::cerr << "Problem, type noise unknown\n";
		}
	}

	return CLAMP(-0.99999f, 0.99999f, (float)value);
}

float Perlin::noiseTurbulence(float* f, float octaves)
{
	float tf[Perlin::MAX_DIMENSIONS];

	/* Initialize locals */
	double value = 0;
	int i, j;
	memcpy(tf, f, sizeof(float) * ndim);

	/* Inner loop of spectral construction, where the fractal is built */
	for (i = 0; i < (int)octaves; i++)
	{
		float nval = 0.0f;

		if (getNoiseType() == TypeNoise::Perlin)
		{
			nval += noisePerlin(tf);
		}
		else if (getNoiseType() == TypeNoise::Simplex)
		{
			nval += noiseSimplex(tf);
		}
		else if (getNoiseType() == TypeNoise::Wavelet)
		{
			nval += noiseWavelet(tf);
		}
		else
		{
			std::cerr << "Problem, type noise unknown\n";
		}

		value += (double)(ABS(nval)) * exponent[i];
		for (j = 0; j < ndim; j++) tf[j] *= lacunarity;
	}

	/* Take care of remainder in octaves */
	octaves -= (int)octaves;
	if(octaves > DELTA)
	{

		float nval = 0.0f;

		if (getNoiseType() == TypeNoise::Perlin)
		{
			nval += noisePerlin(tf);
		}
		else if (getNoiseType() == TypeNoise::Simplex)
		{
			nval += noiseSimplex(tf);
		}
		else if (getNoiseType() == TypeNoise::Wavelet)
		{
			nval += noiseWavelet(tf);
		}
		else
		{
			std::cerr << "Problem, type noise unknown\n";
		}

		value += (double)(octaves * ABS(nval)) * exponent[i];
	}
	return CLAMP(-0.99999f, 0.99999f, (float)value);
}

float TCOD_noise_turbulence_perlin(Perlin* noise, float* f, float octaves)
{
	noise->setNoiseType(TypeNoise::Perlin);
	return noise->noiseTurbulence(f, octaves);
}

float TCOD_noise_turbulence_simplex(Perlin* noise, float* f, float octaves)
{
	noise->setNoiseType(TypeNoise::Simplex);
	return noise->noiseTurbulence(f, octaves);
}

float TCOD_noise_turbulence_wavelet(Perlin* noise, float* f, float octaves)
{
	noise->setNoiseType(TypeNoise::Wavelet);
	return noise->noiseTurbulence(f, octaves);
}

float TCOD_noise_get_fbm_ex(Perlin* noise, float* f, float octaves, TypeNoise type)
{
	switch (type)
	{
	case (TypeNoise::Perlin):
		noise->setNoiseType(TypeNoise::Perlin);
		return noise->noiseFBM(f, octaves);

	case (TypeNoise::Simplex):
		noise->setNoiseType(TypeNoise::Simplex);
		return noise->noiseFBM(f, octaves);

	case (TypeNoise::Wavelet):
		noise->setNoiseType(TypeNoise::Wavelet);
		return noise->noiseFBM(f, octaves);

	default:
		noise->setNoiseType(TypeNoise::Perlin);
		return noise->noiseFBM(f, octaves);
	}
}

float TCOD_noise_get_turbulence_ex(Perlin* noise, float* f, float octaves, TypeNoise type)
{
	switch (type)
	{
	case (TypeNoise::Perlin):
		return TCOD_noise_turbulence_perlin(noise, f, octaves);

	case (TypeNoise::Simplex):
		return TCOD_noise_turbulence_simplex(noise, f, octaves);

	case (TypeNoise::Wavelet):
		return TCOD_noise_turbulence_wavelet(noise, f, octaves);

	default:
		return TCOD_noise_turbulence_perlin(noise, f, octaves);
	}
}

float TCOD_noise_get_fbm(Perlin* noise, float* f, float octaves)
{
	switch ((noise)->noise_type)
	{
	case (TypeNoise::Perlin):
		noise->setNoiseType(TypeNoise::Perlin);
		return noise->noiseFBM(f, octaves);

	case (TypeNoise::Simplex):
		noise->setNoiseType(TypeNoise::Simplex);
		return noise->noiseFBM(f, octaves);

	case (TypeNoise::Wavelet):
		noise->setNoiseType(TypeNoise::Wavelet);
		return noise->noiseFBM(f, octaves);

	default:
		noise->setNoiseType(TypeNoise::Simplex);
		return noise->noiseFBM(f, octaves);
	}
}

float TCOD_noise_get_turbulence(Perlin* noise, float* f, float octaves)
{
	switch ((noise)->noise_type)
	{
	case (TypeNoise::Perlin):
		return TCOD_noise_turbulence_perlin(noise, f, octaves);
		break;
	case (TypeNoise::Simplex):
		return TCOD_noise_turbulence_simplex(noise, f, octaves);
		break;
	case (TypeNoise::Wavelet):
		return TCOD_noise_turbulence_wavelet(noise, f, octaves);
		break;
	default:
		return TCOD_noise_turbulence_simplex(noise, f, octaves);
		break;
	}
}

void TCOD_noise_delete(Perlin* noise)
{
	free(noise);
}

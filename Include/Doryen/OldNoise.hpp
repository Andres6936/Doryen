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

#ifndef _TCOD_PERLIN_H
#define _TCOD_PERLIN_H

#include <cstdint>
#include <vector>
#include <array>

typedef void* TCOD_noise_t;

enum class TypeNoise : unsigned char
{
	Perlin,
	Simplex,
	Wavelet,
	Default,
};

class Perlin
{

public:

	const static std::uint8_t MAX_OCTAVES = 128;
	const static std::uint8_t MAX_DIMENSIONS = 4;

	constexpr static float DEFAULT_HURST = 0.5f;
	constexpr static float DEFAULT_LACUNARITY = 2.0f;

	int ndim;

	float buffer[256][MAX_DIMENSIONS];    /* Random 256 x ndim buffer */

	/* fractal stuff */
	float lacunarity;

	std::vector<float> waveletTileData;

	std::array<unsigned char, 256> map; /* Randomized map of indexes into buffer */

	std::array<float, MAX_OCTAVES> exponent;

	/* noise type */
	TypeNoise noise_type = TypeNoise::Default;

	// Construct

	Perlin() = default;

	Perlin(std::int32_t _dimensions, float _lacunarity);

	// Methods

	void prepareNoiseWavelet();

	void normalize(float* f);

	float noisePerlin(float* f);

	float noiseSimplex(float* f);

	float noiseWavelet(float* f);

	float lattice(int ix, float fx, int iy, float fy, int iz, float fz, int iw, float fw);

	// Getters

	TypeNoise getNoiseType() const;

	// Setters

	void setNoiseType(TypeNoise _noiseType);
};


float TCOD_noise_get_fbm_ex(Perlin* noise, float* f, float octaves, TypeNoise type);

float TCOD_noise_get_turbulence_ex(Perlin* noise, float* f, float octaves, TypeNoise type);

float TCOD_noise_get(Perlin* noise, float* f);

float TCOD_noise_get_fbm(Perlin* noise, float* f, float octaves);

float TCOD_noise_get_turbulence(Perlin* noise, float* f, float octaves);

/* delete the noise object */
void TCOD_noise_delete(Perlin* noise);

#endif

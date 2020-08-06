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

#include "Doryen/noise.hpp"
#include <Doryen/Random/Number.hpp>

using namespace Doryen;

Noise::Noise(int dimensions, TypeNoise type)
{
	data = new Perlin(dimensions, 2.0f);
	data->setNoiseType(type);
}

Noise::Noise(int dimensions, float lacunarity, TypeNoise type)
{
	data = new Perlin(dimensions, lacunarity);
	data->setNoiseType(type);
}

void Noise::setType(TypeNoise type)
{
	data->setNoiseType(type);
}

float Noise::get(float* f, TypeNoise type)
{
	switch (data->getNoiseType())
	{
	case (TypeNoise::Perlin):
		return data->noisePerlin(f);

	case (TypeNoise::Simplex):
		return data->noiseSimplex(f);

	case (TypeNoise::Wavelet):
		return data->noiseWavelet(f);

	default:
		return data->noiseSimplex(f);
	}
}

float Noise::getFbm(float* f, float octaves, TypeNoise type)
{
	switch (data->getNoiseType())
	{
	case (TypeNoise::Perlin):
		data->setNoiseType(TypeNoise::Perlin);
		return data->noiseFBM(f, octaves);

	case (TypeNoise::Simplex):
		data->setNoiseType(TypeNoise::Simplex);
		return data->noiseFBM(f, octaves);

	case (TypeNoise::Wavelet):
		data->setNoiseType(TypeNoise::Wavelet);
		return data->noiseFBM(f, octaves);

	default:
		data->setNoiseType(TypeNoise::Simplex);
		return data->noiseFBM(f, octaves);
	}
}

float Noise::getTurbulence(float* f, float octaves, TypeNoise type)
{
	if (type == TypeNoise::Default) return TCOD_noise_get_turbulence(data, f, octaves);
	else return TCOD_noise_get_turbulence_ex(data, f, octaves, type);
}

Noise::~Noise()
{
	TCOD_noise_delete(data);
}
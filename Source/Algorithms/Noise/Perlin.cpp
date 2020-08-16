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
#include <cstring>
#include <iostream>
#include <algorithm>

#include "Doryen/Algorithms/Noise/Perlin.hpp"
#include "Doryen/Algorithms/Noise/Noise.hpp"
#include "Doryen/Random/Number.hpp"

using namespace Doryen;
using namespace Doryen::Algorithms;

#define ABS(a) ((a)<0?-(a):(a))
#define CLAMP(a, b, x)        ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

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

template<int Dimension>
float Perlin<Dimension>::noiseFBM(const std::array<float, Dimension>& f, float octaves)
{
	std::array<float, Dimension> tf{ f.begin(), f.end() };

	/* Initialize locals */
	double value = 0;
	int i;

	/* Inner loop of spectral construction, where the fractal is built */
	for (i = 0; i < (int)octaves; i++)
	{
		if (getNoiseType() == TypeNoise::Perlin)
		{
			value += Noise<Dimension>::perlin.noise(tf) * exponent[i];
		}
		else if (getNoiseType() == TypeNoise::Simplex)
		{
			value += Noise<Dimension>::simplex.noise(tf) * exponent[i];
		}
		else if (getNoiseType() == TypeNoise::Wavelet)
		{
			value += Noise<Dimension>::wavelet.noise(tf) * exponent[i];
		}
		else
		{
			std::cerr << "Problem, type noise unknown\n";
		}

		for (int j = 0; j < Dimension; j++) tf[j] *= lacunarity;
	}

	/* Take care of remainder in octaves */
	octaves -= (int)octaves;

	if (octaves > DELTA)
	{
		if (getNoiseType() == TypeNoise::Perlin)
		{
			value += octaves * Noise<Dimension>::perlin.noise(tf) * exponent[i];
		}
		else if (getNoiseType() == TypeNoise::Simplex)
		{
			value += octaves * Noise<Dimension>::simplex.noise(tf) * exponent[i];
		}
		else if (getNoiseType() == TypeNoise::Wavelet)
		{
			value += octaves * Noise<Dimension>::wavelet.noise(tf) * exponent[i];
		}
		else
		{
			std::cerr << "Problem, type noise unknown\n";
		}
	}

	return std::clamp(static_cast<float>(value), -0.99999f, 0.99999f);
}

template<int Dimension>
float Perlin<Dimension>::noiseTurbulence(const std::array<float, Dimension>& f, float octaves)
{
	std::array<float, Dimension> tf{ f.begin(), f.end() };

	/* Initialize locals */
	double value = 0;
	int i;

	/* Inner loop of spectral construction, where the fractal is built */
	for (i = 0; i < (int)octaves; i++)
	{
		float nval = 0.0f;

		if (getNoiseType() == TypeNoise::Perlin)
		{
			nval += Noise<Dimension>::perlin.noise(tf);
		}
		else if (getNoiseType() == TypeNoise::Simplex)
		{
			nval += Noise<Dimension>::simplex.noise(tf);
		}
		else if (getNoiseType() == TypeNoise::Wavelet)
		{
			nval += Noise<Dimension>::wavelet.noise(tf);
		}
		else
		{
			std::cerr << "Problem, type noise unknown\n";
		}

		value += (double)(ABS(nval)) * exponent[i];
		for (int j = 0; j < Dimension; j++) tf[j] *= lacunarity;
	}

	/* Take care of remainder in octaves */
	octaves -= (int)octaves;
	if(octaves > DELTA)
	{

		float nval = 0.0f;

		if (getNoiseType() == TypeNoise::Perlin)
		{
			nval += Noise<Dimension>::perlin.noise(tf);
		}
		else if (getNoiseType() == TypeNoise::Simplex)
		{
			nval += Noise<Dimension>::simplex.noise(tf);
		}
		else if (getNoiseType() == TypeNoise::Wavelet)
		{
			nval += Noise<Dimension>::wavelet.noise(tf);
		}
		else
		{
			std::cerr << "Problem, type noise unknown\n";
		}

		value += (double)(octaves * ABS(nval)) * exponent[i];
	}
	return std::clamp(static_cast<float>(value), -0.99999f, 0.99999f);
}

template<int Dimension>
Perlin<Dimension>::Perlin()
{
	for (size_t i = 0; i < 256; ++i)
	{
		p[i] = static_cast<std::uint8_t>(i);
	}

	std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine{});

	for (size_t i = 0; i < 256; ++i)
	{
		p[256 + i] = p[i];
	}
}

template<int Dimension>
float Perlin<Dimension>::Fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

template<int Dimension>
float Perlin<Dimension>::noise(const std::array<float, Dimension>& input)
{
	// Extract the values for x, y and z depended of dimension of Perlin
	float x = (Dimension == 1) ? input[0] : 0.0f;
	float y = (Dimension == 2) ? input[1] : 0.0f;
	float z = (Dimension == 3) ? input[2] : 0.0f;

	const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & 255;
	const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & 255;
	const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & 255;

	x -= std::floor(x);
	y -= std::floor(y);
	z -= std::floor(z);

	const float u = Fade(x);
	const float v = Fade(y);
	const float w = Fade(z);

	const std::int32_t A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
	const std::int32_t B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

	return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
			Grad(p[BA], x - 1, y, z)),
			Lerp(u, Grad(p[AB], x, y - 1, z),
					Grad(p[BB], x - 1, y - 1, z))),
			Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
					Grad(p[BA + 1], x - 1, y, z - 1)),
					Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
							Grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

template<int Dimension>
float Perlin<Dimension>::Lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

template<int Dimension>
float Perlin<Dimension>::Grad(std::uint8_t hash, float x, float y, float z)
{
	const std::uint8_t h = hash & 15;
	const float u = h < 8 ? x : y;
	const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}


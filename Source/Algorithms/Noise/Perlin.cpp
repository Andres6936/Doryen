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

template<int Dimension>
Perlin<Dimension>::Perlin(const std::int32_t _dimensions, const float _lacunarity)
{
	float f = 1.0f;

	for (int j = 0; j < Perlin::MAX_OCTAVES; j++)
	{
		/*exponent[i] = powf(f, -H); */
		exponent[j] = 1.0f / f;
		f *= lacunarity;
	}
}

// Getters

template<int Dimension>
TypeNoise Perlin<Dimension>::getNoiseType() const
{
	return noise_type;
}

// Setters

template<int Dimension>
void Perlin<Dimension>::setNoiseType(TypeNoise _noiseType)
{
	noise_type = _noiseType;
}

#define DELTA                1e-6f
#define SWAP(a, b, t)        t = a; a = b; b = t

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

template<int Dimension>
float Perlin<Dimension>::fractalNoise(const std::array<float, Dimension>& input, int octaves)
{
	float lacunarity = 2.0f;
	float gain = 0.5f;

	float amplitud = 0.5;
	float frecuency = 1.0f;

	float value = 0.0f;

	for (int i = 0; i < octaves; ++i)
	{
		value += amplitud * (noise(input) * frecuency);
		frecuency *= lacunarity;
		amplitud *= gain;
	}

	return value;
}

template<int Dimension>
float Perlin<Dimension>::turbulenceNoise(const std::array<float, Dimension>& input, int octaves)
{
	std::array<float, Dimension> st = input;

	float amplitud = 0.5;

	float value = 0.0f;

	for (int i = 0; i < octaves; ++i)
	{
		value += amplitud * std::abs(noise(st));
		std::for_each(st.begin(), st.end(), [](float& element)
		{ element *= 0.2f; });
		amplitud *= 0.5f;
	}

	return value;
}

// Explicitly instantiate the template

template
class Doryen::Algorithms::Perlin<1>;

template
class Doryen::Algorithms::Perlin<2>;

template
class Doryen::Algorithms::Perlin<3>;

template
class Doryen::Algorithms::Perlin<4>;
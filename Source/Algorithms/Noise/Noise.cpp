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

#include "Doryen/Algorithms/Noise/Noise.hpp"

using namespace Doryen;

template<int Dimension>
Noise<Dimension>::Noise(int dimensions, TypeNoise type)
{
	data = new Algorithms::Perlin<Dimension>(dimensions, 2.0f);
	data->setNoiseType(type);
}

template<int Dimension>
Noise<Dimension>::Noise(int dimensions, float lacunarity, TypeNoise type)
{
	data = new Algorithms::Perlin<Dimension>(dimensions, lacunarity);
	data->setNoiseType(type);
}

template<int Dimension>
void Noise<Dimension>::setType(TypeNoise type)
{
	data->setNoiseType(type);
}

template<int Dimension>
float Noise<Dimension>::get(std::array<float, Dimension>& f, TypeNoise type)
{
	data->setNoiseType(type);

	switch (data->getNoiseType())
	{
	case (TypeNoise::Perlin):
		return perlin.noise(f);

	case (TypeNoise::Simplex):
		return simplex.noise(f);

	case (TypeNoise::Wavelet):
		return wavelet.noise(f);

	default:
		return simplex.noise(f);
	}
}

template<int Dimension>
float Noise<Dimension>::getFbm(const std::array<float, Dimension>& f, float octaves, TypeNoise type)
{
	data->setNoiseType(type);

	switch (data->getNoiseType())
	{
	case (TypeNoise::Perlin):
		return perlin.fractalNoise(f, octaves);

	case (TypeNoise::Simplex):
		return simplex.fractalNoise(f, octaves);

	case (TypeNoise::Wavelet):
		return wavelet.fractalNoise(f, octaves);

	default:
		return simplex.fractalNoise(f, octaves);
	}
}

template<int Dimension>
float Noise<Dimension>::getTurbulence(const std::array<float, Dimension>& f, float octaves, TypeNoise type)
{
	data->setNoiseType(type);

	switch (data->getNoiseType())
	{
	case (TypeNoise::Perlin):
		return data->noiseTurbulence(f, octaves);

	case (TypeNoise::Simplex):
		return data->noiseTurbulence(f, octaves);

	case (TypeNoise::Wavelet):
		return data->noiseTurbulence(f, octaves);

	default:
		return data->noiseTurbulence(f, octaves);
	}
}

template<int Dimension>
Noise<Dimension>::~Noise()
{
	delete data;
}

// Explicitly instantiate the template

template
class Doryen::Noise<1>;

template
class Doryen::Noise<2>;

template
class Doryen::Noise<3>;

template
class Doryen::Noise<4>;
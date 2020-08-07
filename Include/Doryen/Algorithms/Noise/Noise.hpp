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

#ifndef _TCOD_PERLIN_HPP
#define _TCOD_PERLIN_HPP

#include "Doryen/Algorithms/Noise/Perlin.hpp"

#include <array>
#include <vector>
#include <cstdint>

namespace Doryen
{

	/**
	 * This toolkit provides several functions to generate Perlin noise and
	 * other derived noises.
	 *
	 * @note It can handle noise functions from 1 to 4 dimensions.
	 *
	 * Usage example:
	 *
	 * @li 1D noise : the variation of a torch intensity
	 * @li 2D fbm : heightfield generation or clouds
	 * @li 3D fbm : animated smoke
	 *
	 * If you don't know what is Perlin noise and derived functions, or what
	 * is the influence of the different fractal parameters, check the Perlin
	 * noise sample included with the library.
	 *
	 * In 4D, Perlin noise is 17 times slower than simplex noise.
	 */
	class Noise
	{

	public :

		/**
		 * Those functions initialize a noise generator from a number of
		 * dimensions (from 1 to 4), some fractal parameters and a random
		 * number generator.
		 *
		 * @param dimensions From 1 to 4.
		 * @param type Type of algorithm to used for generate the noise.
		 */
		Noise(int dimensions, TypeNoise type = TypeNoise::Simplex);

		Noise(int dimensions, float lacunarity, TypeNoise type = TypeNoise::Simplex);

		virtual ~Noise();

		/**
		 * Use this function to define the default algorithm used by the noise
		 * functions.
		 *
		 * The default algorithm is simplex. It's much faster than Perlin,
		 * especially in 4 dimensions. It has a better contrast too.
		 *
		 * @param type The algorithm to use, either Simplex, Perlin or Wavelet.
		 */
		void setType(TypeNoise type);

		/**
		@PageName noise_get
	 @PageFather noise
	 @PageTitle Getting flat noise
	 @FuncDesc This function returns the noise function value between -1.0 and 1.0 at given coordinates.
	 @Cpp float TCODNoise::get(float *f, TypeNoise type = TypeNoise::Default)
	 @C	float TCOD_noise_get(TCOD_noise_t noise, float *f)
float TCOD_noise_get_ex(TCOD_noise_t noise, float *f, TypeNoise type)
	 @Py noise_get(noise, f, type=NOISE_DEFAULT)
	 @C# float TCODNoise::get(float[] f, type=NoiseDefault)
	 @Param noise	In the C version, the generator handler, returned by the initialization function.
	 @Param f	An array of coordinates, depending on the generator dimensions (between 1 and 4). The same array of coordinates will always return the same value.
	 @Param type	The algorithm to use. If not defined, use the default one (set with setType or simplex if not set)
	 @CppEx
		 // 1d noise
		 TCODNoise * noise1d = new TCODNoise(1);
		 float p=0.5f;
		 // get a 1d simplex value
		 float value = noise1d->get(&p);
		 // 2d noise
		 TCODNoise * noise2d = new TCODNoise(2);
		 float p[2]={0.5f,0.7f};
		 // get a 2D Perlin value
		 float value = noise2d->get(p, TCOD_NOISE_PERLIN);
	 @CEx
		 // 1d noise
		 TCOD_noise_t noise1d = TCOD_noise_new(1,TypeNoise::Default_HURST, TypeNoise::Default_LACUNARITY,NULL);
		 float p=0.5f;
		 // get a 1d simplex value
		 float value = TCOD_noise_get(noise1d,&p);
		 // 2d noise
		 TCOD_noise_t noise2d = TCOD_noise_new(2,TypeNoise::Default_HURST, TypeNoise::Default_LACUNARITY,NULL);
		 float p[2]={0.5f,0.7f};
		 // get a 2d perlin value
		 float value = TCOD_noise_get_ex(noise2d,p,TCOD_NOISE_PERLIN);
	 @PyEx
		 # 1d noise
		 noise1d = libtcod.noise_new(1)
		 # get a 1d simplex value
		 value = libtcod.noise_get(noise1d,[0.5])
		 # 2d noise
		 noise2d = libtcod.noise_new(2)
		 # get a 2d perlin value
		 value = libtcod.noise_get(noise2d,[0.5,0.7], libtcod.NOISE_PERLIN)
		*/
		float get(float* f, TypeNoise type = TypeNoise::Simplex);

		/**
		@PageName noise_get_fbm
		@PageFather noise
		@PageTitle Getting fbm noise
		@FuncDesc This function returns the fbm function value between -1.0 and 1.0 at given coordinates, using fractal hurst and lacunarity defined when the generator has been created.
		@Cpp float TCODNoise::getFbm(float *f, float octaves, TypeNoise type = TypeNoise::Default)
		@C	float TCOD_noise_get_fbm(TCOD_noise_t noise, float *f, float octaves)
float TCOD_noise_get_fbm(TCOD_noise_t noise, float *f, float octaves, TypeNoise type)
		@Py noise_get_fbm(noise, f, octaves, type=NOISE_DEFAULT)
		@C# float TCODNoise::getBrownianMotion(float[] f, float octaves, type=NoiseDefault)
		@Param noise	In the C version, the generator handler, returned by the initialization function.
		@Param f	An array of coordinates, depending on the generator dimensions (between 1 and 4). The same array of coordinates will always return the same value.
		@Param octaves	Number of iterations. Must be < TCOD_NOISE_MAX_OCTAVES = 128
		@Param type	The algorithm to use. If not defined, use the default one (set with setType or simplex if not set)
		@CppEx
			// 1d fbm
			TCODNoise * noise1d = new TCODNoise(1);
			float p=0.5f;
			// get a 1d simplex fbm
			float value = noise1d->getFbm(&p,32.0f);
			// 2d fbm
			TCODNoise * noise2d = new TCODNoise(2);
			float p[2]={0.5f,0.7f};
			// get a 2d perlin fbm
			float value = noise2d->getFbm(p,32.0f, TCOD_NOISE_PERLIN);
		@CEx
			// 1d fbm
			TCOD_noise_t noise1d = TCOD_noise_new(1,TypeNoise::Default_HURST, TypeNoise::Default_LACUNARITY,NULL);
			float p=0.5f;
			// get a 1d simplex fbm
			float value = TCOD_noise_get_fbm(noise1d,&p,32.0f);
			// 2d fbm
			TCOD_noise_t noise2d = TCOD_noise_new(2,TypeNoise::Default_HURST, TypeNoise::Default_LACUNARITY,NULL);
			float p[2]={0.5f,0.7f};
			// get a 2d perlin fbm
			float value = TCOD_noise_get_fbm_ex(noise2d,p,32.0f,TCOD_NOISE_PERLIN);
		@PyEx
			# 1d noise
			noise1d = libtcod.noise_new(1)
			# 1d simplex fbm
			value = libtcod.noise_get_fbm(noise1d,[0.5],32.0)
			# 2d noise
			noise2d = libtcod.noise_new(2)
			# 2d perlin fbm
			value = libtcod.noise_get_fbm(noise2d,[0.5,0.7],32.0, libtcod.NOISE_PERLIN)
		*/
		float getFbm(float* f, float octaves, TypeNoise type = TypeNoise::Simplex);

		/**
		@PageName noise_get_turbulence
		@PageFather noise
		@PageTitle Getting turbulence
		@FuncDesc This function returns the turbulence function value between -1.0 and 1.0 at given coordinates, using fractal hurst and lacunarity defined when the generator has been created.
		@Cpp float TCODNoise::getTurbulence(float *f, float octaves, TypeNoise type = TypeNoise::Default)
		@C	float TCOD_noise_get_turbulence(TCOD_noise_t noise, float *f, float octaves)
float TCOD_noise_get_turbulence_ex(TCOD_noise_t noise, float *f, float octaves, TypeNoise)
		@Py noise_get_turbulence(noise, f, octaves, type=NOISE_DEFAULT)
		@C# float TCODNoise::getTurbulence(float[] f, float octaves, type=NoiseDefault)
		@Param noise	In the C version, the generator handler, returned by the initialization function.
		@Param f	An array of coordinates, depending on the generator dimensions (between 1 and 4). The same array of coordinates will always return the same value.
		@Param octaves	Number of iterations. Must be < TCOD_NOISE_MAX_OCTAVES = 128
		@CppEx
			// 1d fbm
			TCODNoise * noise1d = new TCODNoise(1);
			float p=0.5f;
			// a 1d simplex turbulence
			float value = noise1d->getTurbulence(&p,32.0f);
			// 2d fbm
			TCODNoise * noise2d = new TCODNoise(2);
			float p[2]={0.5f,0.7f};
			// a 2d perlin turbulence
			float value = noise2d->getTurbulence(p,32.0f, TCOD_NOISE_PERLIN);
		@CEx
			// 1d fbm
			TCOD_noise_t noise1d = TCOD_noise_new(1,TypeNoise::Default_HURST, TypeNoise::Default_LACUNARITY,NULL);
			float p=0.5f;
			// a 1d simplex turbulence
			float value = TCOD_noise_get_turbulence(noise1d,&p,32.0f);
			// 2d fbm
			TCOD_noise_t noise2d = TCOD_noise_new(2,TypeNoise::Default_HURST, TypeNoise::Default_LACUNARITY,NULL);
			float p[2]={0.5f,0.7f};
			// a 2d perlin turbulence
			float value = TCOD_noise_get_turbulence_ex(noise2d,p,32.0f, TCOD_NOISE_PERLIN);
		@PyEx
			# 1d noise
			noise1d = libtcod.noise_new(1)
			# 1d simplex turbulence
			value = libtcod.noise_get_turbulence(noise1d,[0.5],32.0)
			# 2d noise
			noise2d = libtcod.noise_new(2)
			# 2d perlin turbulence
			value = libtcod.noise_get_turbulence(noise2d,[0.5,0.7],32.0,libtcod.NOISE_PERLIN)
		*/
		float getTurbulence(float* f, float octaves, TypeNoise type = TypeNoise::Simplex);

	public:

		Algorithms::Perlin* data;
	};

}

#endif

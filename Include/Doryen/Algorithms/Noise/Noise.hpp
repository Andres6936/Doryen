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

#include "Doryen/Algorithms/Noise/FBM.hpp"
#include "Doryen/Algorithms/Noise/Perlin.hpp"
#include "Doryen/Algorithms/Noise/Wavelet.hpp"
#include "Doryen/Algorithms/Noise/Simplex.hpp"

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
	template<int Dimension>
	class Noise
	{

	private:

		inline static Algorithms::Simplex<Dimension> simplex{};

		inline static Algorithms::Wavelet<Dimension> wavelet{};

		inline static Algorithms::Perlin<Dimension> perlin{};

		TypeNoise type = TypeNoise::Simplex;

	public :

		Noise() noexcept = default;

		/**
		 * Those functions initialize a noise generator from a number of
		 * dimensions (from 1 to 4), some fractal parameters and a random
		 * number generator.
		 *
		 * @param dimensions From 1 to 4.
		 * @param type Type of algorithm to used for generate the noise.
		 */
		Noise(TypeNoise _type) noexcept;

		/**
		 * Use this function to define the default algorithm used by the noise
		 * functions.
		 *
		 * The default algorithm is simplex. It's much faster than Perlin,
		 * especially in 4 dimensions. It has a better contrast too.
		 *
		 * @param type The algorithm to use, either Simplex, Perlin or Wavelet.
		 */
		void setType(TypeNoise _type);

		/**
		 * This function returns the noise function value between -1.0 and 1.0
		 * at given coordinates.
		 *
		 * @example
		 * @code
		 *
		 * // 1 Dimension noise
		 * Noise noise1d {1};
		 * float p=0.5f;
		 *
		 * // get a 1d simplex value
		 * float value = noise1d.get(&p);
		 *
		 * // 2 Dimension noise
		 * Noise noise2d {2};
		 * float p[2]={0.5f,0.7f};
		 *
		 * // get a 2D Perlin value
		 * float value = noise2d.get(p, TCOD_NOISE_PERLIN);
		 * @endcode
		 *
		 * @param f An array of coordinates, depending on the generator
		 *  dimensions (between 1 and 4). The same array of coordinates
		 *  will always return the same value.
		 *
		 * @param type The algorithm to use. If not defined, use the
		 *  default one (set with setType or simplex if not set).
		 *
		 * @return noise function value between -1.0 and 1.0.
		 */
		float get(const std::array<float, Dimension>& f);

		/**
		 * This function returns the fbm function value between -1.0 and 1.0 at
		 * given coordinates, using fractal hurst and lacunarity defined when
		 * the generator has been created.
		 *
		 * @param f An array of coordinates, depending on the generator
		 *  dimensions (between 1 and 4). The same array of coordinates
		 *  will always return the same value.
		 *
		 * @param octaves Number of iterations. Must be < MAX_OCTAVES = 128
		 *
		 * @param type The algorithm to use. If not defined, use the default
		 *  one (set with setType or simplex if not set)
		 *
		 * @return the fbm function value between -1.0 and 1.0.
		 */
		float getFbm(const std::array<float, Dimension>& f, float octaves);

		/**
		 * This function returns the turbulence function value between -1.0
		 * and 1.0 at given coordinates, using fractal hurst and lacunarity
		 * defined when the generator has been created.
		 *
		 * @param f An array of coordinates, depending on the generator
		 *  dimensions (between 1 and 4). The same array of coordinates
		 *  will always return the same value.
		 *
		 * @param octaves Number of iterations. Must be < MAX_OCTAVES = 128
		 *
		 * @param type The algorithm to use. If not defined, use the default
		 *  one (set with setType or simplex if not set)
		 *
		 * @return the turbulence function value between -1.0 and 1.0.
		 */
		float getTurbulence(const std::array<float, Dimension>& f, float octaves);
	};

}

#endif

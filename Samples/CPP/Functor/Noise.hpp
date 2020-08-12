// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_NOISE_HPP
#define DORYEN_NOISE_HPP

#include <Doryen/Doryen.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class Noise : public ISample
	{

	private:

		enum class NoiseRender
		{
			PERLIN,
			SIMPLEX,
			WAVELET,
			FBM_PERLIN,
			TURBULENCE_PERLIN,
			FBM_SIMPLEX,
			TURBULENCE_SIMPLEX,
			FBM_WAVELET,
			TURBULENCE_WAVELET
		};

		constexpr static std::array funcName = {
				"1 : Perlin Noise       ",
				"2 : Simplex Noise      ",
				"3 : Wavelet Noise      ",
				"4 : Perlin Fbm         ",
				"5 : Perlin Turbulence  ",
				"6 : Simplex Fbm        ",
				"7 : Simplex Turbulence ",
				"8 : Wavelet Fbm        ",
				"9 : Wavelet Turbulence ",
		};

		/**
		 * Which function of noise we render
		 */
		NoiseRender func = NoiseRender::PERLIN;

		void drawTextNoiseRender();

		void switchNoise(const KeyCode& key);

	public:

		Noise(Console& _console);

		void render(KeyCode key, const Doryen::Mouse& mouse) override;

	};
}

#endif //DORYEN_NOISE_HPP

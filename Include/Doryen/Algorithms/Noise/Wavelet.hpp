// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_WAVELET_HPP
#define DORYEN_WAVELET_HPP

#include <array>
#include <vector>
#include <cstdint>

namespace Doryen::Algorithms
{

	template<int Dimension>
	class Wavelet
	{

	private:

		static const std::int8_t WAVELET_TILE_SIZE = 32;

		static const std::int8_t WAVELET_ARAD = 16;

		static constexpr float WAVELET_SCALE = 2.0f;

		std::vector<float> waveletTileData;

		void TCOD_noise_wavelet_downsample(float* from, float* to, int stride);

		void TCOD_noise_wavelet_upsample(float* from, float* to, int stride);

		void prepareNoiseWavelet();

	public:

		float noise(const std::array<float, Dimension>& f);

		float fractalNoise(const std::array<float, Dimension>& input, int octaves);

		float turbulenceNoise(const std::array<float, Dimension>& input, int octaves);

	};

}

#endif //DORYEN_WAVELET_HPP

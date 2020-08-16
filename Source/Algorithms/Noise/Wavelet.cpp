// Joan Andrés (@Andres6936) Github.

#include <cmath>
#include <algorithm>

#include "Doryen/Random/Number.hpp"
#include "Doryen/Algorithms/Noise/Wavelet.hpp"

using namespace Doryen;

static int absmod(int x, int n)
{
	int m = x % n;
	return m < 0 ? m + n : m;
}

template<int Dimension>
void Algorithms::Wavelet<Dimension>::TCOD_noise_wavelet_downsample(float* from, float* to, int stride)
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

template<int Dimension>
void Algorithms::Wavelet<Dimension>::TCOD_noise_wavelet_upsample(float* from, float* to, int stride)
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

template<int Dimension>
void Algorithms::Wavelet<Dimension>::prepareNoiseWavelet()
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

template<int Dimension>
float Algorithms::Wavelet<Dimension>::noise(const std::array<float, Dimension>& f)
{
	// Not support for dimension greater that 3
	if constexpr (Dimension > 3) return 0.0f;

	std::array<float, Dimension> pf;

	int p[3], c[3], mid[3], n = WAVELET_TILE_SIZE;
	float w[3][3], t, result = 0.0f;

	if (waveletTileData.empty()) prepareNoiseWavelet();

	for (int i = 0; i < Dimension; i++) pf[i] = f[i] * WAVELET_SCALE;

	for (int i = Dimension; i < 3; i++) pf[i] = 0.0f;

	for (int i = 0; i < 3; i++)
	{
		mid[i] = std::ceil(pf[i] - 0.5f);
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
	return std::clamp(result, -1.0f, 1.0f);
}

template<int Dimension>
float Algorithms::Wavelet<Dimension>::fractalNoise(const std::array<float, Dimension>& input, int octaves)
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
float Algorithms::Wavelet<Dimension>::turbulenceNoise(const std::array<float, Dimension>& input, int octaves)
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
class Doryen::Algorithms::Wavelet<1>;

template
class Doryen::Algorithms::Wavelet<2>;

template
class Doryen::Algorithms::Wavelet<3>;

template
class Doryen::Algorithms::Wavelet<4>;
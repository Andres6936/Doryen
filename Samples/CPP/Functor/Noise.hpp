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

		/**
		 * Which function of noise we render
		 */
		NoiseRender func = NoiseRender::PERLIN;

		void switchNoise(const KeyCode& key);

	public:

		Noise(const std::string _name, Console& _console);

		void render(KeyCode key, const Doryen::Mouse& mouse) override;

	};
}

#endif //DORYEN_NOISE_HPP

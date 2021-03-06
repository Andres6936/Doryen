// Joan Andrés (@Andres6936) Github.

#include "Noise.hpp"


using namespace Doryen;

void Functor::Noise::switchNoise(const KeyCode& key)
{
	switch (key)
	{
	case KeyCode::K_1:
		func = NoiseRender::PERLIN;
		break;
	case KeyCode::K_2:
		func = NoiseRender::SIMPLEX;
		break;
	case KeyCode::K_3:
		func = NoiseRender::WAVELET;
		break;
	case KeyCode::K_4:
		func = NoiseRender::FBM_PERLIN;
		break;
	case KeyCode::K_5:
		func = NoiseRender::TURBULENCE_PERLIN;
		break;
	case KeyCode::K_6:
		func = NoiseRender::FBM_SIMPLEX;
		break;
	case KeyCode::K_7:
		func = NoiseRender::TURBULENCE_SIMPLEX;
		break;
	case KeyCode::K_8:
		func = NoiseRender::FBM_WAVELET;
		break;
	case KeyCode::K_9:
		func = NoiseRender::TURBULENCE_WAVELET;
		break;
	}
}

void Functor::Noise::drawTextNoiseRender()
{
	sample.setForegroundColor(Palette::GRAY_WARN_1);

	for (int i = 0; i < funcName.size(); ++i)
	{
		sample.writeString(2, 2 + i, funcName[i]);
	}
}

void Functor::Noise::render(KeyCode key, const Mouse& mouse)
{
	static Doryen::Noise<2>* noise = NULL;
	static float dx = 0.0f, dy = 0.0f;
	static float octaves = 4.0f;
	static float lacunarity = 2.0f;
	static Doryen::Image* img = NULL;
	static float zoom = 3.0f;
	if (!noise)
	{
		// Created a Noise of two dimensions
		noise = new Doryen::Noise<2>();
		img = new Doryen::Image(sample.getWidth() * 2, sample.getHeight() * 2);
	}

	// texture animation
	dx += 0.01f;
	dy += 0.01f;
	// render the 2d noise function
	for (int y = 0; y < 2 * sample.getHeight(); y++)
	{
		for (int x = 0; x < 2 * sample.getWidth(); x++)
		{
			std::array<float, 2> f;
			f[0] = zoom * x / (2 * sample.getWidth()) + dx;
			f[1] = zoom * y / (2 * sample.getHeight()) + dy;
			float value = 0.0f;
			switch (func)
			{
			case NoiseRender::PERLIN :
				noise->setType(TypeNoise::Perlin);
				value = noise->get(f);
				break;
			case NoiseRender::SIMPLEX :
				noise->setType(TypeNoise::Simplex);
				value = noise->get(f);
				break;
			case NoiseRender::WAVELET :
				noise->setType(TypeNoise::Wavelet);
				value = noise->get(f);
				break;
			case NoiseRender::FBM_PERLIN :
				noise->setType(TypeNoise::Perlin);
				value = noise->getFbm(f, octaves);
				break;
			case NoiseRender::TURBULENCE_PERLIN :
				noise->setType(TypeNoise::Perlin);
				value = noise->getTurbulence(f, octaves);
				break;
			case NoiseRender::FBM_SIMPLEX :
				noise->setType(TypeNoise::Simplex);
				value = noise->getFbm(f, octaves);
				break;
			case NoiseRender::TURBULENCE_SIMPLEX :
				noise->setType(TypeNoise::Simplex);
				value = noise->getTurbulence(f, octaves);
				break;
			case NoiseRender::FBM_WAVELET :
				noise->setType(TypeNoise::Wavelet);
				value = noise->getFbm(f, octaves);
				break;
			case NoiseRender::TURBULENCE_WAVELET :
				noise->setType(TypeNoise::Wavelet);
				value = noise->getTurbulence(f, octaves);
				break;
			}
			uint8 c = (uint8)((value + 1.0f) / 2.0f * 255);
			// use a bluish color
			Color col((short)(c / 2), (short)(c / 2), (short)c);
			img->setPixel(x, y, col);
		}
	}
	// blit the noise image on the console with subcell resolution
	img->blit2x(sample, 0, 0);

	// draw a transparent rectangle
	sample.setBackgroundColor(Palette::GRAY_WARN_90);
	sample.drawFillRect(2, 2, 23, 10, false, Doryen::BlendModes::SET);

	drawTextNoiseRender();

	// draw parameters
	sample.setForegroundColor(Palette::GRAY_WARN_1);
	sample.writeString(2, 11, format("Y/H : zoom {2.1f}         ", zoom));
	if (func > NoiseRender::WAVELET)
	{
		sample.writeString(2, 13, format("R/F : lacunarity {2.1f}", lacunarity));
		sample.writeString(2, 14, format("T/G : octaves {2.1f}", octaves));
	}
	// handle keypress
	if (key == KeyCode::NONE) return;

	switchNoise(key);

	if (key == KeyCode::E)
	{
		delete noise;
		noise = new Doryen::Noise<2>();
	}
	else if (key == KeyCode::D)
	{
		delete noise;
		noise = new Doryen::Noise<2>();
	}
	else if (key == KeyCode::R)
	{
		// increase lacunarity
		lacunarity += 0.5f;
		delete noise;
		noise = new Doryen::Noise<2>();
	}
	else if (key == KeyCode::F)
	{
		// decrease lacunarity
		lacunarity -= 0.5f;
		delete noise;
		noise = new Doryen::Noise<2>();
	}
	else if (key == KeyCode::T)
	{
		// increase octaves
		octaves += 0.5f;
	}
	else if (key == KeyCode::G)
	{
		// decrease octaves
		octaves -= 0.5f;
	}
	else if (key == KeyCode::Y)
	{
		// increase zoom
		zoom += 0.2f;
	}
	else if (key == KeyCode::H)
	{
		// decrease zoom
		zoom -= 0.2f;
	}
}

Functor::Noise::Noise(Console& _console) : ISample("Noise", _console)
{

}

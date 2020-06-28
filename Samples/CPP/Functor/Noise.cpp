// Joan Andrés (@Andres6936) Github.

#include "Noise.hpp"


using namespace Doryen;

void Functor::Noise::render(KeyCode key, const Mouse& mouse)
{
	enum
	{
		PERLIN, SIMPLEX, WAVELET,
		FBM_PERLIN, TURBULENCE_PERLIN,
		FBM_SIMPLEX, TURBULENCE_SIMPLEX,
		FBM_WAVELET, TURBULENCE_WAVELET
	}; // which function we render
	static const char* funcName[] = {
			"1 : perlin noise       ",
			"2 : simplex noise      ",
			"3 : wavelet noise      ",
			"4 : perlin fbm         ",
			"5 : perlin turbulence  ",
			"6 : simplex fbm        ",
			"7 : simplex turbulence ",
			"8 : wavelet fbm        ",
			"9 : wavelet turbulence ",
	};
	static int func = PERLIN;
	static TCODNoise* noise = NULL;
	static float dx = 0.0f, dy = 0.0f;
	static float octaves = 4.0f;
	static float hurst = TCOD_NOISE_DEFAULT_HURST;
	static float lacunarity = TCOD_NOISE_DEFAULT_LACUNARITY;
	static Doryen::Image* img = NULL;
	static float zoom = 3.0f;
	if (!noise)
	{
		noise = new TCODNoise(2, hurst, lacunarity);
		img = new Doryen::Image(sample.get().getWidth() * 2, sample.get().getHeight() * 2);
	}

	// texture animation
	dx += 0.01f;
	dy += 0.01f;
	// render the 2d noise function
	for (int y = 0; y < 2 * sample.get().getHeight(); y++)
	{
		for (int x = 0; x < 2 * sample.get().getWidth(); x++)
		{
			float f[2];
			f[0] = zoom * x / (2 * sample.get().getWidth()) + dx;
			f[1] = zoom * y / (2 * sample.get().getHeight()) + dy;
			float value = 0.0f;
			switch (func)
			{
			case PERLIN :
				value = noise->get(f, TCOD_NOISE_PERLIN);
				break;
			case SIMPLEX :
				value = noise->get(f, TCOD_NOISE_SIMPLEX);
				break;
			case WAVELET :
				value = noise->get(f, TCOD_NOISE_WAVELET);
				break;
			case FBM_PERLIN :
				value = noise->getFbm(f, octaves, TCOD_NOISE_PERLIN);
				break;
			case TURBULENCE_PERLIN :
				value = noise->getTurbulence(f, octaves, TCOD_NOISE_PERLIN);
				break;
			case FBM_SIMPLEX :
				value = noise->getFbm(f, octaves, TCOD_NOISE_SIMPLEX);
				break;
			case TURBULENCE_SIMPLEX :
				value = noise->getTurbulence(f, octaves, TCOD_NOISE_SIMPLEX);
				break;
			case FBM_WAVELET :
				value = noise->getFbm(f, octaves, TCOD_NOISE_WAVELET);
				break;
			case TURBULENCE_WAVELET :
				value = noise->getTurbulence(f, octaves, TCOD_NOISE_WAVELET);
				break;
			}
			uint8 c = (uint8)((value + 1.0f) / 2.0f * 255);
			// use a bluish color
			Doryen::Color col((short)(c / 2), (short)(c / 2), (short)c);
			img->putPixel(x, y, col);
		}
	}
	// blit the noise image on the console with subcell resolution
	img->blit2x(sample, 0, 0);
	// draw a transparent rectangle
	sample.get().setDefaultBackground(Doryen::Color::grey);
	sample.get().rect(2, 2, 23, (func <= WAVELET ? 10 : 13), false, Doryen::BackgroundFlag::MULTIPLY);
	for (int y = 2; y < 2 + (func <= WAVELET ? 10 : 13); y++)
	{
		for (int x = 2; x < 2 + 23; x++)
		{
			Doryen::Color col = sample.get().getCharForeground(x, y);
			col = col * Doryen::Color::grey;
			sample.get().setCharForeground(x, y, col);
		}
	}

	// draw the text
	for (int curfunc = PERLIN; curfunc <= TURBULENCE_WAVELET; curfunc++)
	{
		if (curfunc == func)
		{
			sample.get().setDefaultForeground(Doryen::Color::white);
			sample.get().setDefaultBackground(Doryen::Color::lightBlue);
			sample.get().printEx(2, 2 + curfunc, TCOD_BKGND_SET, TCOD_LEFT, funcName[curfunc]);
		}
		else
		{
			sample.get().setDefaultForeground(Doryen::Color::grey);
			sample.get().print(2, 2 + curfunc, funcName[curfunc]);
		}
	}
	// draw parameters
	sample.get().setDefaultForeground(Doryen::Color::white);
	sample.get().print(2, 11, format("Y/H : zoom {2.1f}", zoom));
	if (func > WAVELET)
	{
		sample.get().print(2, 12, format("E/D : hurst {2.1f}", hurst));
		sample.get().print(2, 13, format("R/F : lacunarity {2.1f}", lacunarity));
		sample.get().print(2, 14, format("T/G : octaves {2.1f}", octaves));
	}
	// handle keypress
	if (key == KeyCode::NONE) return;

//    if ( key->c >= '1' && key->c <= '9' )
//    {
//        // change the noise function
//        func = key->c - '1';
//    }
//    else if ( key->c == 'E' || key->c == 'e' )
//    {
//        // increase hurst
//        hurst += 0.1f;
//        delete noise;
//        noise = new TCODNoise( 2, hurst, lacunarity );
//    }
//    else if ( key->c == 'D' || key->c == 'd' )
//    {
//        // decrease hurst
//        hurst -= 0.1f;
//        delete noise;
//        noise = new TCODNoise( 2, hurst, lacunarity );
//    }
//    else if ( key->c == 'R' || key->c == 'r' )
//    {
//        // increase lacunarity
//        lacunarity += 0.5f;
//        delete noise;
//        noise = new TCODNoise( 2, hurst, lacunarity );
//    }
//    else if ( key->c == 'F' || key->c == 'f' )
//    {
//        // decrease lacunarity
//        lacunarity -= 0.5f;
//        delete noise;
//        noise = new TCODNoise( 2, hurst, lacunarity );
//    }
//    else if ( key->c == 'T' || key->c == 't' )
//    {
//        // increase octaves
//        octaves += 0.5f;
//    }
//    else if ( key->c == 'G' || key->c == 'g' )
//    {
//        // decrease octaves
//        octaves -= 0.5f;
//    }
//    else if ( key->c == 'Y' || key->c == 'y' )
//    {
//        // increase zoom
//        zoom += 0.2f;
//    }
//    else if ( key->c == 'H' || key->c == 'h' )
//    {
//        // decrease zoom
//        zoom -= 0.2f;
//    }
}

Functor::Noise::Noise(std::string _name, std::reference_wrapper<Console> _console) : ISample(_name), sample(_console)
{

}

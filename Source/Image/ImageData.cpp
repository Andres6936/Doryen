#include <array>
#include <fstream>
#include <iostream>

#include <Console.hpp>
#include <Image/PNG/State.hpp>
#include "Image/ImageData.hpp"

bool Doryen::ImageData::isTypeImageBMP(const std::string& filename)
{
	std::array <unsigned char, 2> magicNumber = { 0x42, 0x4d };
	// Open file in mode of only read in mode binary
	std::ifstream stream(filename, std::ios::in | std::ios::binary);

	if (stream.is_open())
	{
		// Create a buffer with a size equals to of magicNumber
		char buffer[magicNumber.size()];

		stream.read(buffer, magicNumber.size());
		stream.close();

		for (int i = 0; i < magicNumber.size(); ++i)
		{
			if (buffer[i] != magicNumber[i])
			{
				// No is a BMP
				return false;
			}
		}

		// Is a BMP
		return true;
	}
	else
	{
		// Throw Exception
	}
}

bool Doryen::ImageData::isTypeImagePNG(const std::string& filename)
{
	std::array <unsigned char, 8> magicNumber = { 137, 80, 78, 71, 13, 10, 26, 10 };
	// Open file in mode of only read in mode binary
	std::ifstream stream(filename, std::ios::in | std::ios::binary);

	if (stream.is_open())
	{
		// Create a buffer with a size equals to of magicNumber
		unsigned char buffer[magicNumber.size()];

		// Is neccessary the cast for get the correct
		// magic number from of buffer
		stream.read((char*)buffer, magicNumber.size());
		stream.close();

		for (int i = 0; i < magicNumber.size(); ++i)
		{
			if (buffer[i] != magicNumber[i])
			{
				// No is a PNG
				return false;
			}
		}

		// Is a PNG
		return true;
	}
	else
	{
		// Throw Exception
	}
}

Doryen::ImageData::ImageData(const std::string& filename)
{
	if (isTypeImageBMP(filename))
	{
		readImageBMP(filename);
	}
	else if (isTypeImagePNG(filename))
	{
		readImagePNG(filename);
	}
}

void Doryen::ImageData::readImageBMP(const std::string& filename)
{
	representation = SDL_LoadBMP(filename.c_str());

	if (representation == nullptr)
	{
		// Throw Error
		std::cout << SDL_GetError() << "\n";
	}

	// Convert low color images to 24 bits
	if (representation->format->BytesPerPixel != 3)
	{
		unsigned int rmask = 0;
		unsigned int gmask = 0;
		unsigned int bmask = 0;

		if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		{
			rmask = 0xFF0000;
			gmask = 0x00FF00;
			bmask = 0x0000FF;
		}
		else
		{
			rmask = 0x0000FF;
			gmask = 0x00FF00;
			bmask = 0xFF0000;
		}

		SDL_Surface* temp = SDL_CreateRGBSurface(SDL_SWSURFACE,
				representation->w, representation->h,
				24, rmask, gmask, bmask, 0);
		SDL_BlitSurface(representation, nullptr, temp, nullptr);
		SDL_FreeSurface(temp);
	}
}

void Doryen::ImageData::readImagePNG(const std::string& filename)
{
	size_t pngsize = 0;

	std::ifstream stream;

	// Open file in mode of only read and mode of read binary
	stream.open(filename, std::ifstream::binary);

	unsigned char* png;

	if (stream.is_open())
	{
		// Retrieves the file size of the open file
		stream.seekg(0, std::ifstream::end);
		unsigned int filesize = stream.tellg();
		stream.seekg(0, std::ifstream::beg);

		pngsize = filesize;

		// allocate buffer
		png = new unsigned char[filesize];

		stream.read((char*)png, filesize);
		stream.close();
	}
	else
	{
		// TODO: Throw Exception
	}

	unsigned width, height;

	LodePNGState state;

	state.inspect(&width, &height, png, pngsize);

	unsigned bpp = state.getBitsPerPixel();

	if (bpp == 24)
	{
		/* don't convert to 32 bits because libtcod's 24bits renderer is faster */
		state.info_raw.colortype = LCT_RGB;
	}
	else if (bpp != 24 && bpp != 32)
	{
		/* paletted png. convert to 24 bits */
		state.info_raw.colortype = LCT_RGB;
		state.info_raw.bitdepth = 8;
		bpp = 24;
	}

	// Where the imagen be saved
	std::vector <unsigned char> image;

//	unsigned error = lodepng_decode(&image, &width, &height, &state, png, pngsize);
	unsigned error = state.decode(image, &width, &height, png, pngsize);

	delete[] png;

	if (error)
	{
		//printf("error %u: %s\n", error, lodepng_error_text(error));
		//lodepng_state_cleanup(&state);
	}

	/* create the SDL surface */
	this->representation = createNewSurface(width, height, bpp == 32);

	// Reserve
	unsigned char* source = new unsigned char[image.size()];

	for (int i = 0; i < image.size(); ++i)
	{
		source[i] = image[i];
	}

	unsigned int rowsize = width * bpp / 8;

	for (unsigned y = 0; y < height; y++)
	{
		auto row_pointer = (unsigned char*)this->representation->pixels + (y * this->representation->pitch);
		memcpy(row_pointer, source, rowsize);
		source += rowsize;
	}
}

SDL_Surface* Doryen::ImageData::createNewSurface(
		const unsigned int width,
		const unsigned int height,
		const bool alpha)
{
	Uint32 rmask;
	Uint32 gmask;
	Uint32 bmask;
	Uint32 amask;

	int flags = SDL_SWSURFACE;

	if (alpha)
	{
		if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		{
			rmask = 0x000000FF;
			gmask = 0x0000FF00;
			bmask = 0x00FF0000;
			amask = 0xFF000000;
		}
		else
		{
			rmask = 0xFF000000;
			gmask = 0x00FF0000;
			bmask = 0x0000FF00;
			amask = 0x000000FF;
		}
#if !SDL_VERSION_ATLEAST(2, 0, 0)
		flags |= SDL_SRCALPHA;
#endif
	}
	else
	{
		if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		{
			rmask = 0x0000FF;
			gmask = 0x00FF00;
			bmask = 0xFF0000;
		}
		else
		{
			rmask = 0xFF0000;
			gmask = 0x00FF00;
			bmask = 0x0000FF;
		}
		amask = 0;
	}
#if SDL_VERSION_ATLEAST(2, 0, 0)
	bitmap=SDL_CreateRGBSurface(flags,width,height,
#else
	SDL_Surface* bitmap = SDL_AllocSurface(flags, width, height,
#endif
			alpha ? 32 : 24,
			rmask, gmask, bmask, amask);
	if (alpha)
	{
#if SDL_VERSION_ATLEAST(2, 0, 0)
		SDL_SetSurfaceAlphaMod(bitmap, 255);
#else
		SDL_SetAlpha(bitmap, SDL_SRCALPHA, 255);
#endif
	}
	return bitmap;
}

Doryen::ImageData::ImageData(
		unsigned int width,
		unsigned int heigth)
{
	mipmaps.resize(Mipmap::getLevels(width, heigth));

	// Access to first element and resize the buffer of colors.
	mipmaps.at(0).buf.resize(width * heigth);

	for (int i = 0; i < width * heigth; ++i)
	{
		mipmaps.at(0).buf[i] = Color::black;
	}

	float fw = (float)width;
	float fh = (float)heigth;

	for (Mipmap& mipmap : mipmaps)
	{
		mipmap.width = width;
		mipmap.height = heigth;
		mipmap.fwidth = fw;
		mipmap.fheight = fh;

		width >>= 1;
		heigth >>= 1;

		fw *= 0.5f;
		fh *= 0.5f;
	}
}

void Doryen::ImageData::createBitmapFrom(const Console& console)
{
	unsigned int w = console.getWidth();
	unsigned int h = console.getHeight();

	representation = createNewSurface(w, h, false);

	Color fadingColor = console.getFadingColor();

	int fade = console.getFade();

	// TODO: Implemented (Imposible, is needed use charmap)
}

SDL_Surface* Doryen::ImageData::getRepresentation() const
{
	return representation;
}

Size Doryen::ImageData::getSize() const
{
	if (representation == nullptr)
	{
		if (mipmaps.empty())
		{
			return Size(0, 0);
		}
		else
		{
			Size size;
			size.w = (int)mipmaps[0].width;
			size.h = (int)mipmaps[0].height;

			return size;
		}
	}
	else
	{
		return Size(representation->w, representation->h);
	}
}

Doryen::ImageData::~ImageData()
{
	SDL_FreeSurface((SDL_Surface*)representation);
}

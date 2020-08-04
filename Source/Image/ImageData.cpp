#include <array>
#include <fstream>
#include <iostream>

#include "Doryen/Console/Console.hpp"
#include <Doryen/Image/PNG/State.hpp>
#include "Doryen/Image/ImageData.hpp"
#include "Doryen/Graphics/Color/Palette.hpp"

using namespace Doryen;

bool ImageData::isTypeImageBMP(const std::string& filename)
{
	std::array<unsigned char, 2> magicNumber = { 0x42, 0x4d };
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

bool ImageData::isTypeImagePNG(const std::string& filename)
{
	std::array<unsigned char, 8> magicNumber = { 137, 80, 78, 71, 13, 10, 26, 10 };
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

ImageData::ImageData(const std::string& filename)
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

void ImageData::readImageBMP(const std::string& filename)
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

void ImageData::readImagePNG(const std::string& filename)
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

SDL_Surface* ImageData::createNewSurface(
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

ImageData::ImageData(
		unsigned int width,
		unsigned int heigth)
{
	mipmaps.resize(Mipmap::getLevels(width, heigth));

	// Access to first element and resize the buffer of colors.
	mipmaps.at(0).buf.resize(width * heigth);

	for (int i = 0; i < width * heigth; ++i)
	{
		mipmaps.at(0).buf[i] = Palette::GRAY_WARN_90;
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

void ImageData::createBitmapFrom(const Console& console)
{
	unsigned int w = console.getWidth();
	unsigned int h = console.getHeight();

	representation = createNewSurface(w, h, false);

	Color fadingColor = console.getFadingColor();

	int fade = console.getFade();

	// TODO: Implemented (Imposible, is needed use charmap)
}

SDL_Surface* ImageData::getRepresentation() const
{
	return representation;
}

Geometry::Size ImageData::getSize() const
{
	if (representation == nullptr)
	{
		if (mipmaps.empty())
		{
			return { 0, 0 };
		}
		else
		{
			Geometry::Size size;
			size.w = (int)mipmaps[0].width;
			size.h = (int)mipmaps[0].height;

			return size;
		}
	}
	else
	{
		return { representation->w, representation->h };
	}
}

//ImageData::~ImageData()
//{
//	SDL_FreeSurface((SDL_Surface*)representation);
//}

Color ImageData::getPixel(int x, int y) const
{
	if (representation == nullptr)
	{
		if (isInvariantSatisfied(x, y))
		{
			return mipmaps[0].getColorAt(x, y);
		}
		else
		{
			return Palette::GRAY_WARN_90;
		}
	}
	else
	{
		// Verify invariant
		if (not isCoordinateInsideRange(x, y))
		{
			// Invariant not satisfied
			return Palette::GRAY_WARN_90;
		}

		Uint8 bytesPerPixel = representation->format->BytesPerPixel;
		// The address to the pixel we want to retrieve
		Uint8* pixel = (Uint8*)representation->pixels + y *
														representation->pitch + x * bytesPerPixel;

		if (bytesPerPixel == 1)
		{
			if (representation->format->palette not_eq nullptr)
			{
				SDL_Color color = representation->format->palette->colors[(*pixel)];

				return Color(color.r, color.g, color.b);
			}
			else
			{
				return Palette::GRAY_WARN_90;
			}
		}
		else
		{
			Color _color;

			_color.r = *((pixel) + representation->format->Rshift / 8);
			_color.g = *((pixel) + representation->format->Gshift / 8);
			_color.b = *((pixel) + representation->format->Bshift / 8);

			return _color;
		}
	}
}

bool ImageData::isInvariantSatisfied(int _x, int _y) const
{
	if (mipmaps.empty()) return false;

	// Else, evaluate the condition
	return _x >= 0 and _x < mipmaps[0].width and
		   _y >= 0 and _y < mipmaps[0].height;
}

int ImageData::getAlpha(int x, int y) const
{
	if (representation not_eq nullptr)
	{
		if (not isCoordinateInsideRange(x, y)) return 255;

		Uint8 bytesPerPixel = representation->format->BytesPerPixel;

		// If the image not have channel for alpha
		// Assume that the channel alpha is 255
		if (bytesPerPixel != 4) return 255;

		Uint8* pixel = ((Uint8*)representation->pixels) + y *
														  representation->pitch + x * bytesPerPixel;

		return *((pixel) + representation->format->Ashift / 8);
	}
	else
	{
		return 255;
	}
}

bool ImageData::isCoordinateInsideRange(int _x, int _y) const
{
	return _x >= 0 and _y >= 0 and
		   _x < representation->w and
		   _y < representation->h;
}

bool ImageData::isHasKeyColor() const
{
	return hasKeyColor;
}

const Color& ImageData::getKeyColor() const
{
	return keyColor;
}

const Color& ImageData::getMipmapPixel(
		const Pointf& _point0,
		const Pointf& _point1)
{
	if (representation not_eq nullptr)
	{
		if (mipmaps.empty())
		{
			initMipmaps();
		}

		int texelXSize = (int)(_point1.x - _point0.x);
		int texelYSize = (int)(_point1.y - _point0.y);
		int texelSize = 0;

		if (texelXSize < texelYSize)
		{
			texelSize = texelYSize;
		}
		else
		{
			texelSize = texelXSize;
		}

		int mip = 0;
		int curSize = 1;

		const unsigned numberMipmaps = mipmaps.size();

		while (mip < mipmaps.size() - 1 and curSize < texelSize)
		{
			++mip;
			curSize <<= 1;
		}

		if (mip > 0) --mip;

		int texelX = (int)(_point0.x * mipmaps[mip].width / mipmaps[0].fwidth);
		int texelY = (int)(_point0.y * mipmaps[mip].height / mipmaps[0].fheight);

		if (mipmaps[mip].buf.empty())
		{
			generateMip(mip);
		}
		else if (mipmaps[mip].dirty)
		{
			generateMip(mip);
		}

		if (texelX < 0 or texelY < 0 or
			texelX > mipmaps[mip].width or
			texelY > mipmaps[mip].height)
		{
			return Palette::GRAY_WARN_90;
		}

		return mipmaps[mip].buf[texelX + mipmaps[mip].width * texelY];
	}
	else
	{
		return Palette::GRAY_WARN_90;
	}
}

void ImageData::initMipmaps()
{
	Geometry::Size size = getSize();
	mipmaps.resize(Mipmap::getLevels(size.w, size.h));
	mipmaps[0].buf.resize(size.w * size.h);

	for (int x = 0; x < size.w; ++x)
	{
		for (int y = 0; y < size.h; ++y)
		{
			mipmaps[0].buf[x + size.w * y] = getPixel(x, y);
		}
	}

	float fw = (float)size.w;
	float fh = (float)size.h;

	for (Mipmap& mipmap : mipmaps)
	{
		mipmap.width = size.w;
		mipmap.height = size.h;
		mipmap.fwidth = fw;
		mipmap.fheight = fh;
		mipmap.dirty = true;

		size.w >>= 1;
		size.h >>= 1;

		fw *= 0.5f;
		fh *= 0.5f;
	}

	mipmaps[0].dirty = false;
}

void ImageData::generateMip(int _mip)
{
	const Mipmap& origin = mipmaps.at(0);
	Mipmap& current = mipmaps.at(_mip);

	if (current.buf.empty())
	{
		current.buf.resize(current.width * current.height);
	}

	current.dirty = false;

	for (int x = 0; x < current.width; ++x)
	{
		for (int y = 0; y < current.height; ++y)
		{

			int count = 0;
			// Component red
			int r = 0;
			// Component green
			int g = 0;
			// Component blue
			int b = 0;

			for (int sx = (x << _mip); sx < (x + 1) << _mip; ++sx)
			{
				for (int sy = (y << _mip); sy < (y + 1) << _mip; ++sy)
				{
					int offset = sx + origin.width * sy;
					++count;

					r += origin.buf.at(offset).r;
					g += origin.buf.at(offset).g;
					b += origin.buf.at(offset).b;
				}
			}

			r /= count;
			g /= count;
			b /= count;

			Color& color = current.buf.at(x + current.width * y);

			color.r = (short)r;
			color.g = (short)g;
			color.b = (short)b;
		}
	}
}

void ImageData::setPixel(int x, int y, const Color& _color)
{
//	if (representation not_eq nullptr)
	{
		if (mipmaps.empty())
		{
			initMipmaps();
		}
		else
		{
			if (x >= 0 and x < mipmaps.at(0).width and
				y >= 0 and y < mipmaps.at(0).height)
			{
				mipmaps.at(0).setColorAt(x, y, _color);

				// Skip the first mipmap
				for (int i = 1; i < mipmaps.size(); ++i)
				{
					mipmaps[i].setDirty(true);
				}
			}
		}
	}
}

void ImageData::setKeyColor(const Color& _keyColor)
{
	keyColor = _keyColor;
	hasKeyColor = true;
}

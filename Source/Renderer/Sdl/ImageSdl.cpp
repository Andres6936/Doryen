#include <array>
#include <fstream>
#include <algorithm>

#include <Doryen/Image/PNG/State.hpp>
#include "Doryen/Renderer/Sdl/ImageSdl.hpp"
#include "Doryen/Graphics/Color/Palette.hpp"

using namespace Doryen;

// Construct

ImageSdl::ImageSdl(const std::uint32_t width, const std::uint32_t height)
		: MetaImage(width, height)
{
	// Initialize the variables of the parent class {MetaImage}.

	mipmaps.resize(Mipmap::getLevelCount(width, height));

	Size size{ width, height };

	for (Mipmap& mipmap : mipmaps)
	{
		mipmap.setSize(size);

		size.setWidth(size.getWidth() >> 1);
		size.setHeight(size.getHeight() >> 1);
	}
}

ImageSdl::ImageSdl(ImageSdl&& other)
		: MetaImage(other.getWidth(), other.getHeight())
{
	// Initialize the variables of the parent class {MetaImage}.

	representation = other.representation;
	// Assign the data members of the source object to default values.
	// This prevents the destructor from freeing resources (such as memory)
	// multiple times.
	// Release the data pointer from the source object so that
	// the destructor does not free the memory multiple times.
	other.representation = nullptr;

	mipmaps = std::move(other.mipmaps);
	// Assign the data members of the source object to default values.
	other.mipmaps.clear();

	keyColor = other.keyColor;
	hasKeyColor = other.hasKeyColor;
}

ImageSdl::ImageSdl(const ImageSdl& other)
		: MetaImage(other.getWidth(), other.getHeight())
{
	// Initialize the variables of the parent class {MetaImage}.

	// Copy the representation of image
	representation = other.getCopySurface();

	// Copy the content of mipmaps
	std::copy(other.mipmaps.begin(), other.mipmaps.end(), std::back_inserter(this->mipmaps));

	keyColor = other.keyColor;
	hasKeyColor = other.hasKeyColor;
}

ImageSdl::ImageSdl(const std::string& filename) : MetaImage(0, 0)
{
	// Initialize the variables of the parent class {MetaImage} to 0 width and 0 height.

	if (isTypeImageBMP(filename))
	{
		readImageBMP(filename);
	}
	else if (isTypeImagePNG(filename))
	{
		readImagePNG(filename);
	}
}


// Overload Operator

ImageSdl& ImageSdl::operator=(ImageSdl&& other)
{
	// In the move assignment operator, add a conditional statement that
	// performs no operation if you try to assign the object to itself.
	if (this not_eq &other)
	{
		representation = other.representation;
		// Assign the data members of the source object to default values.
		// This prevents the destructor from freeing resources (such as memory)
		// multiple times.
		// Release the data pointer from the source object so that
		// the destructor does not free the memory multiple times.
		other.representation = nullptr;

		mipmaps = std::move(other.mipmaps);
		// Assign the data members of the source object to default values.
		other.mipmaps.clear();

		width = other.getWidth();
		height = other.getHeight();
		keyColor = other.keyColor;
		hasKeyColor = other.hasKeyColor;
	}

	return *this;
}

ImageSdl& ImageSdl::operator=(const ImageSdl& other)
{
	// In the move assignment operator, add a conditional statement that
	// performs no operation if you try to assign the object to itself.
	if (this not_eq &other)
	{
		// Clear the surface used for represent the image
		SDL_FreeSurface(representation);
		// Copy the representation of image
		representation = other.getCopySurface();

		// Clear the content of mipmaps
		mipmaps.clear();
		// Copy the content of mipmaps
		std::copy(other.mipmaps.begin(), other.mipmaps.end(), std::back_inserter(this->mipmaps));

		width = other.getWidth();
		height = other.getHeight();
		keyColor = other.keyColor;
		hasKeyColor = other.hasKeyColor;
	}

	return *this;
}

bool ImageSdl::isTypeImageBMP(const std::string& filename)
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
		throw std::runtime_error("The type of image cannot will be opened.\n");
	}
}

bool ImageSdl::isTypeImagePNG(const std::string& filename)
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
		throw std::runtime_error("The type of image cannot will be opened.\n");
	}
}

void ImageSdl::readImageBMP(const std::string& filename)
{
	representation = SDL_LoadBMP(filename.c_str());

	if (representation == nullptr)
	{
		// Throw Error
		throw std::runtime_error(SDL_GetError());
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

	// Precondition: Set the width and height of image.
	this->width = representation->w;
	this->height = representation->h;
}

void ImageSdl::readImagePNG(const std::string& filename)
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

	std::vector<unsigned char>::iterator iteratorImage = image.begin();

	unsigned int rowsize = width * bpp / 8;

	for (unsigned y = 0; y < height; y++)
	{
		auto row_pointer =
				(unsigned char*)this->representation->pixels + (y * this->representation->pitch);
		// iterator::base() returns the underlying base iterator. In this case < unsigned char* >
		memcpy(row_pointer, iteratorImage.base(), rowsize);
		iteratorImage += rowsize;
	}

	// Precondition: Set the width and height of image.
	this->width = width;
	this->height = height;
}

SDL_Surface* ImageSdl::createNewSurface(
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

SDL_Surface* ImageSdl::getCopySurface() const
{
	return SDL_ConvertSurface(representation, representation->format, SDL_SWSURFACE);
}

ImageSdl::~ImageSdl()
{
	// Avoid that the destructor free the memory multiple times.
	if (representation not_eq nullptr)
	{
		SDL_FreeSurface(representation);
	}
}

// Methods Override

Color ImageSdl::getPixel(const std::int32_t x, const std::int32_t y) const
{
	if (representation == nullptr)
	{
		if (isInvariantSatisfied(x, y))
		{
			return mipmaps[0].getPixelAt(x, y);
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

			_color.setRed(*((pixel) + representation->format->Rshift / 8));
			_color.g = *((pixel) + representation->format->Gshift / 8);
			_color.b = *((pixel) + representation->format->Bshift / 8);

			return _color;
		}
	}
}

void ImageSdl::setPixel(const std::int32_t x, const std::int32_t y, const Color& pixel)
{
//	if (representation not_eq nullptr)
	{
		if (mipmaps.empty())
		{
			updateMipmaps();
		}
		else
		{
			if (x >= 0 and x < mipmaps.at(0).getWidth() and
				y >= 0 and y < mipmaps.at(0).getHeight())
			{
				mipmaps.at(0).setPixelAt(x, y, pixel);

				// Skip the first mipmap
				for (int i = 1; i < mipmaps.size(); ++i)
				{
					mipmaps[i].setUpdated(true);
				}
			}
		}
	}
}

int ImageSdl::getAlpha(const std::int32_t x, const std::int32_t y) const
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

bool ImageSdl::isInvariantSatisfied(int _x, int _y) const
{
	if (mipmaps.empty()) return false;

	// Else, evaluate the condition
	return _x >= 0 and _x < mipmaps[0].getWidth() and
		   _y >= 0 and _y < mipmaps[0].getHeight();
}

bool ImageSdl::isCoordinateInsideRange(int _x, int _y) const
{
	return _x >= 0 and _y >= 0 and
		   _x < representation->w and
		   _y < representation->h;
}

bool ImageSdl::verifyThatAllMipmapsHasBeenSizing() const noexcept
{
	// For start, verify that the array of mipmaps have at least one mipmap.
	// If the array of mipmap is empty, obviously it has nothing.
	if (mipmaps.empty()) return false;

	for (const Mipmap& mipmap : mipmaps)
	{
		// Verify each mipmap and determine if has been sizing.
		// If the mipmap is empty, the sizing of object has failed.
		if (mipmap.isEmpty()) return false;
	}

	// All the objects has been verify successfully.
	return true;
}

bool ImageSdl::isHasKeyColor() const
{
	return hasKeyColor;
}

const Color& ImageSdl::getMipmapPixel(
		const Pointf& _point0,
		const Pointf& _point1)
{
	if (representation not_eq nullptr)
	{
		if (mipmaps.empty())
		{
			updateMipmaps();
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

		const float fWidth = static_cast<float>(mipmaps[0].getWidth());
		const float fHeight = static_cast<float>(mipmaps[0].getHeight());

		int texelX = (int)(_point0.x * mipmaps[mip].getWidth() / fWidth);
		int texelY = (int)(_point0.y * mipmaps[mip].getHeight() / fHeight);

		if (mipmaps[mip].isEmpty())
		{
			generateMip(mip);
		}
		else if (not mipmaps[mip].isUpdated())
		{
			generateMip(mip);
		}

		if (texelX < 0 or texelY < 0 or
			texelX > mipmaps[mip].getWidth() or
			texelY > mipmaps[mip].getHeight())
		{
			return Palette::GRAY_WARN_90;
		}

		return mipmaps[mip].getPixelAt(texelX, texelY);
	}
	else
	{
		return Palette::GRAY_WARN_90;
	}
}

void ImageSdl::updateMipmaps()
{
	// Copy of size
	Size size{ getSize() };

	mipmaps.resize(Mipmap::getLevelCount(size.getWidth(), size.getHeight()));

	for (Mipmap& mipmap : mipmaps)
	{
		mipmap.setSize(size);
		mipmap.setUpdated(false);

		// In an mipmap chains (array of mipmaps), each mipmap (level) is the
		// half of small than the previous one.
		size.setWidth(size.getWidth() / 2);
		size.setHeight(size.getHeight() / 2);
	}

	// The first mipmap is the same original image (same dimension, same pixels).
	// Get the reference to first mipmap for initialize it.
	Mipmap& mipmap = mipmaps[0];

	for (int x = 0; x < mipmap.getWidth(); ++x)
	{
		for (int y = 0; y < mipmap.getHeight(); ++y)
		{
			// Same pixels to original image.
			mipmap.setPixelAt(x, y, getPixel(x, y));
		}
	}

	// The first mipmap always is initialized.
	mipmap.setUpdated(true);

	// Verify the invariant
	if (not verifyThatAllMipmapsHasBeenSizing())
	{
		// The invariant not has been satisfied, emit a alert
		// TODO: Not is possible use <iostream> for print the warning,
		//  application that using ncurses might crashed to we use the
		//  stream that ncurses override, but we can use a <fstream> for
		//  print the warning.
	}
}

void ImageSdl::generateMip(int _mip)
{
	const Mipmap& origin = mipmaps.at(0);
	Mipmap& current = mipmaps.at(_mip);

	current.setUpdated(true);

	for (int x = 0; x < current.getWidth(); ++x)
	{
		for (int y = 0; y < current.getHeight(); ++y)
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
					Color pixel = origin.getPixelAt(sx, sy);
					++count;

					r += pixel.getRed();
					g += pixel.g;
					b += pixel.b;
				}
			}

			r /= count;
			g /= count;
			b /= count;

			current.setPixelAt(x, y, { r, g, b });
		}
	}
}


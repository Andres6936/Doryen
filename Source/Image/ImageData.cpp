#include <array>
#include <fstream>
#include <iostream>
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
	std::array <char, 8> magicNumber = { -119, 80, 78, 71, 13, 10, 26, 10 };
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

	}
	else if (isTypeImagePNG(filename))
	{

	}
}

void Doryen::ImageData::readImageBMP(const std::string& filename)
{
	systemImage = SDL_LoadBMP(filename.c_str());

	if (systemImage == nullptr)
	{
		// Throw Error
		std::cout << SDL_GetError() << "\n";
	}

	// Convert low color images to 24 bits
	if (systemImage->format->BytesPerPixel != 3)
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
				systemImage->w, systemImage->h,
				24, rmask, gmask, bmask, 0);
		SDL_BlitSurface(systemImage, nullptr, temp, nullptr);
		SDL_FreeSurface(temp);
	}
}

void Doryen::ImageData::readImagePNG(const std::string& filename)
{

}

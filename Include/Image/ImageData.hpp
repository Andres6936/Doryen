#ifndef LIBTCOD_IMAGEDATA_HPP
#define LIBTCOD_IMAGEDATA_HPP

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <Color.hpp>
#include <Console.hpp>

#include "Image/Mipmap.hpp"

namespace Doryen
{
	class ImageData
	{

	private:

		SDL_Surface* systemImage = nullptr;

		std::vector <Mipmap> mipmaps;

		Color key_color;

		bool has_key_color;

		// Methods

		void readImageBMP(const std::string& filename);

		void readImagePNG(const std::string& filename);

	public:

		ImageData() = default;

		ImageData(unsigned int width, unsigned int heigth);

		explicit ImageData(const std::string& filename);

		static bool isTypeImageBMP(const std::string& filename);

		static bool isTypeImagePNG(const std::string& filename);

		static SDL_Surface* createNewSurface(unsigned int width, unsigned int height, bool alpha);

		void createBitmapFrom(const Console& console);
	};
}


#endif //LIBTCOD_IMAGEDATA_HPP

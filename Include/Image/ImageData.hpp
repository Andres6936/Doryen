#ifndef LIBTCOD_IMAGEDATA_HPP
#define LIBTCOD_IMAGEDATA_HPP

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include <Color.hpp>

#include "Image/Mipmap.hpp"

namespace Doryen
{
	class ImageData
	{

	private:

		SDL_Surface* systemImage = nullptr;

		int nb_mipmaps;

		std::vector <Mipmap> mipmaps;

		Color key_color;

		bool has_key_color;

		// Methods

		void readImageBMP(const std::string& filename);

		void readImagePNG(const std::string& filename);

	public:

		ImageData(const std::string& filename);

		static bool isTypeImageBMP(const std::string& filename);

		static bool isTypeImagePNG(const std::string& filename);
	};
}


#endif //LIBTCOD_IMAGEDATA_HPP

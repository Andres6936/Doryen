#ifndef LIBTCOD_IMAGEDATA_HPP
#define LIBTCOD_IMAGEDATA_HPP

#include <vector>
#include <string>
#include <SDL/SDL.h>

#include "Color.hpp"
#include "Console.hpp"
#include "Image/Mipmap.hpp"
#include "Geometry/Size.hpp"

namespace Doryen
{

	/**
	 * Represent two things, a struct that content an representation
	 * of an image or a vector that content a representation of pixels
	 * that represent an image.
	 */
	class ImageData
	{

	private:

		SDL_Surface* representation = nullptr;

		std::vector<Mipmap> mipmaps;

		Color key_color;

		bool has_key_color = false;

		// Methods

		void readImageBMP(const std::string& filename);

		void readImagePNG(const std::string& filename);

	public:

		ImageData() = default;

		ImageData(unsigned int width, unsigned int heigth);

		virtual ~ImageData();

		explicit ImageData(const std::string& filename);

		static bool isTypeImageBMP(const std::string& filename);

		static bool isTypeImagePNG(const std::string& filename);

		static SDL_Surface* createNewSurface(unsigned int width, unsigned int height, bool alpha);

		void createBitmapFrom(const Console& console);

		// Getter

		SDL_Surface* getRepresentation() const;

		Size getSize() const;
	};
}


#endif //LIBTCOD_IMAGEDATA_HPP

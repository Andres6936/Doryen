#ifndef LIBTCOD_IMAGEDATA_HPP
#define LIBTCOD_IMAGEDATA_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <SDL/SDL.h>

#include "Doryen/Graphics/Color/Color.hpp"
#include "Doryen/Console/Console.hpp"
#include "Doryen/Geometry/Size.hpp"
#include "Doryen/Image/Mipmap.hpp"

namespace Doryen
{

	/**
	 * Represent two things, a struct that content an representation
	 * of an image or a vector that content a representation of pixels
	 * that represent an image.
	 */
	class ImageSdl
	{

	private:

		// Definitions

		using Pointf = Geometry::Point2D<float>;

		using Size = Geometry::Size;

		// Fields

		SDL_Surface* representation = nullptr;

		std::vector<Mipmap> mipmaps;

		Color keyColor;

		bool hasKeyColor = false;

		// Methods

		void readImageBMP(const std::string& filename);

		void readImagePNG(const std::string& filename);

		void initMipmaps();

		void generateMip(int _mip);

		/**
		 * Verify that the parameters are greater or equal to 0
		 * and lesser that width mipmaps.
		 * @param _x Range between [0, width mipmaps)
		 * @param _y Range between [0, height mipmaps)
		 * @return True if the invariant are satisfied
		 */
		bool isInvariantSatisfied(int _x, int _y) const;

		/**
		 * Verify that the coordinate are inside the range
		 * @param _x Range between [0, width)
		 * @param _y Range between [0, height)
		 * @return True if the invariant are satisfied
		 */
		bool isCoordinateInsideRange(int _x, int _y) const;

	public:

		ImageSdl() = default;

		ImageSdl(const std::int32_t width, const std::int32_t heigth);

		virtual ~ImageSdl();

		explicit ImageSdl(const std::string& filename);

		static bool isTypeImageBMP(const std::string& filename);

		static bool isTypeImagePNG(const std::string& filename);

		static SDL_Surface* createNewSurface(unsigned int width, unsigned int height, bool alpha);

		void createBitmapFrom(const Console& console);

		// Getter

		bool isHasKeyColor() const;

		int getAlpha(int x, int y) const;

		Geometry::Size getSize() const;

		Color getPixel(int x, int y) const;

		const Color& getKeyColor() const;

		const Color& getMipmapPixel(const Pointf& _point0, const Pointf& _point1);

		SDL_Surface* getCopySurface() const;

		// Setters

		void setPixel(int x, int y, const Color& _color);

		void setKeyColor(const Color& _keyColor);
	};
}


#endif //LIBTCOD_IMAGEDATA_HPP

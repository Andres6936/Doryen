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

		// Definitions

		using Pointf = Geometry::Point2D<float>;

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

		ImageData() = default;

		ImageData(unsigned int width, unsigned int heigth);

		virtual ~ImageData();

		explicit ImageData(const std::string& filename);

		static bool isTypeImageBMP(const std::string& filename);

		static bool isTypeImagePNG(const std::string& filename);

		static SDL_Surface* createNewSurface(unsigned int width, unsigned int height, bool alpha);

		void createBitmapFrom(const Console& console);

		// Getter

		bool isHasKeyColor() const;

		int getAlpha(int x, int y) const;

		Size getSize() const;

		Color getPixel(int x, int y) const;

		const Color& getKeyColor() const;

		const Color& getMipmapPixel(const Pointf& _point0, const Pointf& _point1) const;

		SDL_Surface* getRepresentation() const;
	};
}


#endif //LIBTCOD_IMAGEDATA_HPP

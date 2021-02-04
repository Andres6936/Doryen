#ifndef LIBTCOD_IMAGEDATA_HPP
#define LIBTCOD_IMAGEDATA_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <SDL/SDL.h>

#include <Doryen/Geometry/Point2D.hpp>
#include "Doryen/Graphics/Color/Color.hpp"
#include "Doryen/Geometry/Size.hpp"
#include "Doryen/Image/Mipmap.hpp"
#include <Doryen/Image/MetaImage.hpp>

namespace Doryen
{

	/**
	 * Represent two things, a struct that content an representation
	 * of an image or a vector that content a representation of pixels
	 * that represent an image.
	 */
	class ImageSdl : public MetaImage
	{

	private:

		// Definitions

		using Pointf = Geometry::Point2D<float>;

		using Size = Geometry::Size;

		// Fields

		/**
		 * The method getSize() return the width and height of this Surface.
		 */
		SDL_Surface* representation = nullptr;

		/**
		 * The method getSize() return the width and height of first mipmap.
		 */
		std::vector<Mipmap> mipmaps;

		bool hasKeyColor = false;

		// Methods

		/**
		 * Precondition: The width and height of image has been set. (Units: pixels).
		 *
		 * @param filename A string that contains the name of the file from which to create the Image.
		 */
		void readImageBMP(const std::string& filename);

		/**
		 * Precondition: The width and height of image has been set. (Units: pixels).
		 *
		 * @param filename A string that contains the name of the file from which to create the Image.
		 */
		void readImagePNG(const std::string& filename);

		/**
		 * Post-condition: All the mipmaps has been sizing (warning, sizing
		 * not is same that initialize or generate, it is only guaranteed that
		 * the object has enough space to carry out the operations that will
		 * initialized it).
		 */
		void updateMipmaps();

		/**
		 * Precondition: The mipmap has enough space to generate it.
		 *
		 * @param _mip The index of mipmap in the array of mipmaps.
		 */
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

		/**
		 * Is important call to this method for verify that the mipmaps has
		 * been sizing.
		 *
		 * The objects of type Mipmaps have 'strong exception guarantee' when
		 * are sizing (this happens when the object is updated or initialized).
		 *
		 * @return True if the array of mipmaps has been sizing properly.
		 */
		bool verifyThatAllMipmapsHasBeenSizing() const noexcept;

	public:

		// Construct

		ImageSdl(const std::int32_t width, const std::int32_t height);

		/**
		 * Move constructor
		 *
		 * @param other The image to move.
		 */
		ImageSdl(ImageSdl&& other);

		/**
		 * Copy constructor
		 *
		 * @param other The image to copy.
		 */
		ImageSdl(const ImageSdl& other);

		/**
		 * Creates an Image from the specified file.
		 *
		 * @param filename A string that contains the name of the file from which to create the Image.
		 */
		ImageSdl(const std::string& filename);

		// Destructor

		/**
		 * Releases all resources used by this Image.
		 */
		virtual ~ImageSdl();

		// Overload Operators

		/**
		 * Move assignment
		 *
		 * @param other The image to move.
		 * @return Instance of this image.
		 */
		ImageSdl& operator=(ImageSdl&& other);

		/**
		 * Copy assignment
		 *
		 * @param other The image to copy.
		 * @return Instance of this image.
		 */
		ImageSdl& operator=(const ImageSdl& other);

		// Methods Override

		/**
		 * @param x The coordinate in the axis X of this image.
		 * @param y The coordinate in the axis y of this image.
		 * @return Get pixel (RGB) of this Image at the position indicate.
		 */
		Color getPixel(const std::int32_t x, const std::int32_t y) const override;

		/**
		 * @param x The coordinate in the axis X of this image.
		 * @param y The coordinate in the axis y of this image.
		 * @param pixel Set pixel (RGB) of this Image at the position indicate.
		 */
		void setPixel(const std::int32_t x, const std::int32_t y, const Color& pixel) override;

		/**
		 * @param x The coordinate in the axis X of this image.
		 * @param y The coordinate in the axis y of this image.
		 * @return Get the alpha of pixel in this Image. The value is in the range of (0, 255).
		 */
		std::int32_t getAlpha(const std::int32_t x, const std::int32_t y) const override;

		// Methods

		static bool isTypeImageBMP(const std::string& filename);

		static bool isTypeImagePNG(const std::string& filename);

		static SDL_Surface* createNewSurface(unsigned int width, unsigned int height, bool alpha);

		// Getter

		bool isHasKeyColor() const;

		const Color& getMipmapPixel(const Pointf& _point0, const Pointf& _point1);

		SDL_Surface* getCopySurface() const;

		// Setters
	};
}


#endif //LIBTCOD_IMAGEDATA_HPP

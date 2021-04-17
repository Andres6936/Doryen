// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_IMAGEDUMMY_HPP
#define DORYEN_IMAGEDUMMY_HPP

#include <Doryen/Image/MetaImage.hpp>

namespace Doryen
{

	/**
	 * Represent two things, a struct that content an representation
	 * of an image or a vector that content a representation of pixels
	 * that represent an image.
	 */
	class ImageDummy : public MetaImage
	{

	public:

		/**
		 * Initializes a new instance of the ImageDummy class with the specified size.
		 *
		 * @param width The width, in pixels, of the new MetaImage.
		 * @param height The height, in pixels, of the new MetaImge.
		 */
		ImageDummy(const std::int32_t width, const std::int32_t height);

		/**
		 * Creates an Image from the specified file.
		 *
		 * @param filename A string that contains the name of the file from which to create the Image.
		 */
		ImageDummy(const std::string& filename);

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
		int32_t getAlpha(const std::int32_t x, const std::int32_t y) const override;

	};

}

#endif //DORYEN_IMAGEDUMMY_HPP

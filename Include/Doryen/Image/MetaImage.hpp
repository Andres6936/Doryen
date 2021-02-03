// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_METAIMAGE_HPP
#define DORYEN_METAIMAGE_HPP

#include <cstdint>

namespace Doryen
{

	/**
	 * Encapsulates a bitmap, which consists of the pixel data for a graphics
	 * image and its attributes. A Bitmap is an object used to work with images
	 * defined by pixel data.
	 */
	class MetaImage
	{

	private:

		/**
		 * The height, in pixels, of this Image.
		 */
		std::int32_t width{ 0 };

		/**
		 * The width, in pixels, of this Image.
		 */
		std::int32_t height{ 0 };

	public:

		// Constructs

		/**
		 * Initializes a new instance of the MetaImage class with the specified size.
		 *
		 * @param width The width, in pixels, of the new MetaImage.
		 * @param height The height, in pixels, of the new MetaImge.
		 */
		MetaImage(const std::int32_t width, const std::int32_t height);

		// Getters

		/**
		 * @return Gets the width, in pixels, of this Image.
		 */
		std::int32_t getWidth() const noexcept;

		/**
		 * @return Gets the height, in pixels, of this Image.
		 */
		std::int32_t getHeight() const noexcept;

		// Setters

	};

}

#endif //DORYEN_METAIMAGE_HPP

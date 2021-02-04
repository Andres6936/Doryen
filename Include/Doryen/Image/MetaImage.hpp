// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_METAIMAGE_HPP
#define DORYEN_METAIMAGE_HPP

#include <cstdint>
#include <optional>

#include <Doryen/Geometry/Size.hpp>
#include <Doryen/Graphics/Color/Color.hpp>

namespace Doryen
{

	/**
	 * Encapsulates a bitmap, which consists of the pixel data for a graphics
	 * image and its attributes. A Bitmap is an object used to work with images
	 * defined by pixel data.
	 */
	class MetaImage
	{

	protected:

		/**
		 * The height, in pixels, of this Image.
		 */
		std::int32_t width{ 0 };

		/**
		 * The width, in pixels, of this Image.
		 */
		std::int32_t height{ 0 };

		/**
		 * By default the key color not exist. If you wish to establish a key
		 * color, call to the function setKeyColor().
		 */
		std::optional<Color> keyColor;

	public:

		// Constructs

		/**
		 * Initializes a new instance of the MetaImage class with the specified size.
		 *
		 * @param width The width, in pixels, of the new MetaImage.
		 * @param height The height, in pixels, of the new MetaImge.
		 */
		MetaImage(const std::int32_t width, const std::int32_t height);

		// Destructor

		/**
		 * Releases all resources used by this Image. Marked virtual for call the child destructors.
		 */
		virtual ~MetaImage() = default;

		// Methods

		/**
		 * @return Get the width and height of this Image.
		 */
		Geometry::Size getSize() const;

		/**
		 * @param x The coordinate in the axis X of this image.
		 * @param y The coordinate in the axis y of this image.
		 * @return Get pixel (RGB) of this Image at the position indicate.
		 */
		virtual Color getPixel(const std::int32_t x, const std::int32_t y) const = 0;

		/**
		 * @param x The coordinate in the axis X of this image.
		 * @param y The coordinate in the axis y of this image.
		 * @return Get the alpha of pixel in this Image. The value is in the range of (0, 255).
		 */
		virtual std::int32_t getAlpha(const std::int32_t x, const std::int32_t y) const = 0;

		// Getters

		/**
		 * @return Gets the width, in pixels, of this Image.
		 */
		std::int32_t getWidth() const noexcept;

		/**
		 * @return Gets the height, in pixels, of this Image.
		 */
		std::int32_t getHeight() const noexcept;

		/**
		 * @return Get the key color of this Image. (Can be nullable).
		 */
		std::optional<Color> getKeyColor() const noexcept;

		// Setters

		/**
		 * Set a the key color of this image.
		 *
		 * @param keyColor The key color that will represent the simulation of
		 * transparency in the image.
		 */
		void setKeyColor(const Color& keyColor);

	};

}

#endif //DORYEN_METAIMAGE_HPP

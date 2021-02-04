// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_METAIMAGE_HPP
#define DORYEN_METAIMAGE_HPP

#include <cstdint>

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
		 * By default the key color is the color black (R: 0, G: 0, B: 0).
		 */
		Color keyColor{ 0, 0, 0 };

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
		 * The method return a reference for avoid return an copy, is need take
		 * into account the life cycle of the variable that is returned.
		 *
		 * If the reference returned by the method is to be used almost
		 * immediately, there will be no problem with the life cycle, if on the
		 * contrary the reference is stored and the life cycle of this reference
		 * ends, an unauthorized memory access will occur and the operating
		 * system will terminate the execution of the application.
		 *
		 * If you need to store the data that the method returns, create a copy
		 * from the reference to avoid problems with the life cycle.
		 *
		 * @return Get the key color of this Image.
		 */
		const Color& getKeyColor() const noexcept;

		// Setters

	};

}

#endif //DORYEN_METAIMAGE_HPP

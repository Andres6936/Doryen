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

		Color getPixel(const std::int32_t x, const std::int32_t y) const override;

		void setPixel(const std::int32_t x, const std::int32_t y, const Color& pixel) override;

		int32_t getAlpha(const std::int32_t x, const std::int32_t y) const override;

	};

}

#endif //DORYEN_IMAGEDUMMY_HPP

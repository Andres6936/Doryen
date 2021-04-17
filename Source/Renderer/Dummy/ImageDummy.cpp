// Joan Andrés (@Andres6936) Github.

#include <Doryen/Renderer/Dummy/ImageDummy.hpp>

using namespace Doryen;

// Constructor

ImageDummy::ImageDummy(const std::int32_t width, const std::int32_t height)
		: MetaImage(width, height)
{

}

ImageDummy::ImageDummy(const std::string& filename) : MetaImage(0, 0)
{

}

// Methods

Color ImageDummy::getPixel(const std::int32_t x, const std::int32_t y) const
{
	return Color();
}

void ImageDummy::setPixel(const std::int32_t x, const std::int32_t y, const Color& pixel)
{

}

int32_t ImageDummy::getAlpha(const std::int32_t x, const std::int32_t y) const
{
	return 0;
}

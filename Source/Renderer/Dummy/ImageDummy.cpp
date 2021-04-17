// Joan Andrés (@Andres6936) Github.

#include <Doryen/Renderer/Dummy/ImageDummy.hpp>

using namespace Doryen;

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

// Joan Andrés (@Andres6936) Github.

#include "Doryen/Image/MetaImage.hpp"

using namespace Doryen;

// Construct

MetaImage::MetaImage(const std::int32_t _width, const std::int32_t _height)
		: width(_width), height(_height)
{

}

// Methods

Geometry::Size MetaImage::getSize() const
{
	return { width, height };
}

// Getters

std::int32_t MetaImage::getWidth() const noexcept
{
	return width;
}

std::int32_t MetaImage::getHeight() const noexcept
{
	return height;
}

std::optional<Color> MetaImage::getKeyColor() const noexcept
{
	return keyColor;
}

// Setters
// Joan Andrés (@Andres6936) Github.

#include "Doryen/Image/MetaImage.hpp"

using namespace Doryen;

// Construct

MetaImage::MetaImage(const std::uint32_t _width, const std::uint32_t _height)
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

void MetaImage::setKeyColor(const Color& _keyColor)
{
	// It is recommended to use optional<T> in situations where there is exactly
	// one, clear (to all parties) reason for having no value of type T, and
	// where the lack of value is as natural as having any regular value of T
	keyColor = std::make_optional(_keyColor);
}

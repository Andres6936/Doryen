#include "Doryen/Image/Mipmap.hpp"

using namespace Doryen;

// Private Methods

void Mipmap::reduce(const Mipmap::Size& _size) noexcept
{
	// The buffer is reduced
	resize(_size.getWidth() * _size.getHeight());

	// Set the new size
	dimension = _size;
}

// Static Methods

std::uint32_t Mipmap::getLevelCount(const std::uint32_t width, const std::uint32_t height) noexcept
{
	// Original size is: 30 x 40
	// 	Resize to: 15 x 20
	// 	Resize to: 7 x 10
	// 	Resize to: 3 x 5
	// 	Resize to: 1 x 2
	// 	Resize to: 0 x 1
	// End resize, total levels is: 5

	std::uint32_t curw = width;
	std::uint32_t curh = height;

	int levels = 0;

	while (curw > 0 && curh > 0)
	{
		levels++;
		curw >>= 1;
		curh >>= 1;
	}

	return levels;
}

// Debug

std::string Mipmap::toString() const
{
	return { "Width: " + std::to_string(dimension.getWidth()) + ", Height: " +
			 std::to_string(dimension.getHeight()) };
}


// Getters

const bool Mipmap::isEmpty() const noexcept
{
	// Verify the content of std::vector
	return this->empty();
}

const bool Mipmap::isUpdated() const noexcept
{
	return updated;
}

const Mipmap::Size& Mipmap::getSize() const noexcept
{
	return dimension;
}

const Color& Mipmap::getPixelAt(int _x, int _y) const noexcept
{
	return (*this)[_x + _y * dimension.getWidth()];
}

const std::int32_t Mipmap::getWidth() const noexcept
{
	return dimension.getWidth();
}

const std::int32_t Mipmap::getHeight() const noexcept
{
	return dimension.getHeight();
}

// Setters

void Mipmap::setUpdated(const bool hasBeenUpdated) noexcept
{
	updated = hasBeenUpdated;
}

void Mipmap::setSize(const Mipmap::Size& newSize) noexcept
{
	// Copy of current size buffer
	const Size currentSize{ getSize() };

	// Is the new size lesser than actual size of buffer ?
	if (newSize.lessThan(currentSize))
	{
		// Reduce the size of buffer and exit of function.
		return reduce(newSize);
	}
		// No, the new size is greater than actual size of buffer
	else
	{
		// The nature of std::vector::resize is that: If an exception is thrown,
		// 	this function has no effect (strong exception guarantee).
		// In order for us to provide this same guarantee we must verify that the
		// 	buffer size has changed.
		resize(newSize.getWidth() * newSize.getHeight());

		// Verify that the buffer has been resized.
		// In this point, currentSize had the previous size of buffer.
		// Compare the previous size with the actual and verify that
		// 	this are different.
		const std::int32_t previousLengthBuffer = currentSize.getWidth() * currentSize.getHeight();
		const std::int32_t currentLengthBuffer = size();

		if (previousLengthBuffer != currentLengthBuffer)
		{
			// If the buffer has been resized successfully set the new size
			dimension = newSize;
		}
	}
}

void Mipmap::setPixelAt(const std::uint32_t _x, const std::int32_t _y, const Color& _pixel) noexcept
{
	(*this)[_x + _y * dimension.getWidth()].copy(_pixel);
}

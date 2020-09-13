// Design by Joan Andrés.

#ifndef LIBTCOD_SIZE_HPP
#define LIBTCOD_SIZE_HPP

#include <cstdint>

namespace Doryen
{
	namespace Geometry
	{

		/**
		 * The Size class encapsulates the width and height of a component (in
		 * integer precision) in a single object.
		 *
		 * Normally the values of width and height are non-negative integers.
		 * The constructors that allow you to create a dimension do not prevent
		 * you from setting a negative value for these properties. If the value
		 * of width or height is negative, the behavior of some methods defined
		 * by other objects is undefined.
		 */
		class Size
		{

		public:

			/**
			 * Width
			 */
			std::int32_t w = 0;

			/**
			 * Height
			 */
			std::int32_t h = 0;

			Size() = default;

			/**
			 * @param _w Width
			 * @param _h Height
			 */
			Size(std::int32_t _w, std::int32_t _h) noexcept
			{
				w = _w;
				h = _h;
			}

			// Methods

			bool equals(const Size& _rhs) const noexcept
			{
				return this->w == _rhs.w and this->h == _rhs.h;
			}

			// Overload Operator

			bool operator<(const Size& _rhs) const noexcept
			{
				if (w < _rhs.w) return true;

				if (_rhs.w < w) return false;

				return h < _rhs.h;
			}

			bool operator>(const Size& _rhs) const noexcept
			{
				return _rhs < *this;
			}

			bool operator<=(const Size& _rhs) const noexcept
			{
				return !(_rhs < *this);
			}

			bool operator>=(const Size& _rhs) const noexcept
			{
				return !(*this < _rhs);
			}

			// Getters

			/**
			 * @return Returns the width of this Dimension in Integer precision.
			 */
			std::int32_t getWidth() const noexcept
			{
				return w;
			}

			/**
			 * @return Returns the height of this Dimension in Integer precision.
			 */
			std::int32_t getHeight() const noexcept
			{
				return h;
			}

			// Setters

			/**
			 * @param _w Set the width of object to specified in the parameter.
			 */
			void setWidth(std::int32_t _w) noexcept
			{
				w = _w;
			}

			/**
			 * @param _h Set the height of object to specified in the parameter.
			 */
			void setHeight(std::int32_t _h) noexcept
			{
				h = _h;
			}
		};

	}
}

#endif //LIBTCOD_SIZE_HPP

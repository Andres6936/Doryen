// Design by Joan Andrés.

#ifndef LIBTCOD_SIZE_HPP
#define LIBTCOD_SIZE_HPP

#include <string>
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
			 * The width dimension; negative values can be used.
			 */
			std::int32_t w = 0;

			/**
			 * The height dimension; negative values can be used.
			 */
			std::int32_t h = 0;

			/**
			 * Creates an instance of Dimension with a width of zero and a height of zero.
			 */
			Size() = default;

			/**
			 * Constructs a Dimension and initializes it to the specified width and specified height.
			 *
			 * @param _w Width of object.
			 * @param _h Height of object.
			 */
			Size(std::int32_t _w, std::int32_t _h) noexcept
			{
				w = _w;
				h = _h;
			}

			// Debug

			[[maybe_unused]] std::string toString() const
			{
				return { '{' + std::to_string(w) + " ," + std::to_string(h) + '}' };
			}

			// Methods

			bool lessThan(const Size& _object) const noexcept
			{
				if (w < _object.w) return true;

				if (_object.w < w) return false;

				return h < _object.h;
			}

			bool greaterThan(const Size& _object) const noexcept
			{
				return _object.lessThan(*this);
			}

			/**
			 * Checks whether two dimension objects have equal values.
			 *
			 * @param _rhs Another object.
			 * @return True if the properties (width and height) of the objects are the same.
			 */
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
			 * @return The width of this Dimension in Integer precision.
			 */
			std::int32_t getWidth() const noexcept
			{
				return w;
			}

			/**
			 * @return The height of this Dimension in Integer precision.
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

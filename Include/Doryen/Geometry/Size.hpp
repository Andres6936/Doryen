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

		private:

			/**
			 * The first 32 bits are the width, the last 32 bits are the height.
			 * The schema is - W H - for a total of 64 bits.
			 * The width dimension; negative values can be used.
			 * The height dimension; negative values can be used.
			 */
			std::uint64_t value{ 0 };

		public:

			/**
			 * Creates an instance of Dimension with a width of zero and a height of zero.
			 */
			Size() noexcept = default;

			/**
			 * Constructs a Dimension and initializes it to the specified width and specified height.
			 *
			 * @param _w Width of object.
			 * @param _h Height of object.
			 */
			Size(std::int32_t _w, std::int32_t _h) noexcept
			{
				setWidth(_w);
				setHeight(_h);
			}

			// Debug

			[[maybe_unused]] std::string toString() const
			{
				return { '{' + std::to_string(getWidth()) + " ," + std::to_string(getHeight()) +
						 '}' };
			}

			// Methods

			bool lessThan(const Size& _object) const noexcept
			{
				if (getWidth() < _object.getWidth()) return true;

				if (_object.getWidth() < getWidth()) return false;

				return getHeight() < _object.getHeight();
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
				return this->value == _rhs.value;
			}

			// Overload Operator

			bool operator<(const Size& _rhs) const noexcept
			{
				if (getWidth() < _rhs.getWidth()) return true;

				if (_rhs.getWidth() < getWidth()) return false;

				return getHeight() < _rhs.getHeight();
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
				// Get the width from the schema of 64 bits where the
				// first 32 bits are the width of object.
				return value >> 32;
			}

			/**
			 * @return The height of this Dimension in Integer precision.
			 */
			std::int32_t getHeight() const noexcept
			{
				// Get the height from the schema of 64 bits where the
				// last 32 bits are the height of object.
				// First clear the first 32 bits (the width) for avoid
				// get wrong results.
				const std::int64_t height = value << 32;
				// The current schema of height is - H 0 -
				// Needed move the schema in height for that match with
				// the schema: - 0 H -
				// So that move the 32 bits (from 0 to 31) to the
				// positions 32 - 63 for get only the height f object.
				return height >> 32;
			}

			// Setters

			/**
			 * @param _w Set the width of object to specified in the parameter.
			 */
			void setWidth(std::uint64_t _w) noexcept
			{
				static_assert(sizeof(value) == sizeof(_w));

				value <<= 32;
				value >>= 32;
				_w <<= 32;
				value ^= _w;
			}

			/**
			 * @param _h Set the height of object to specified in the parameter.
			 */
			void setHeight(std::uint64_t _h) noexcept
			{
				value >>= 32;
				value <<= 32;
				value ^= _h;
			}
		};

	}
}

#endif //LIBTCOD_SIZE_HPP

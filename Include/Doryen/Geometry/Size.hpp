// Design by Joan Andrés.

#ifndef LIBTCOD_SIZE_HPP
#define LIBTCOD_SIZE_HPP

namespace Doryen
{
	namespace Geometry
	{

		/**
		 * The Size class is to encapsulate a width and a height dimension.
		 */
		class Size
		{

		public:

			/**
			 * Width
			 */
			int w = 0;

			/**
			 * Height
			 */
			int h = 0;

			Size() = default;

			/**
			 * @param _w Width
			 * @param _h Height
			 */
			Size(int _w, int _h) noexcept
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
			int getWidth() const noexcept
			{
				return w;
			}

			/**
			 * @return Returns the height of this Dimension in Integer precision.
			 */
			int getHeight() const noexcept
			{
				return h;
			}

			// Setters

			/**
			 * @param _w Set the width of object to specified in the parameter.
			 */
			void setWidth(int _w) noexcept
			{
				w = _w;
			}

			/**
			 * @param _h Set the height of object to specified in the parameter.
			 */
			void setHeight(int _h) noexcept
			{
				h = _h;
			}
		};

	}
}

#endif //LIBTCOD_SIZE_HPP

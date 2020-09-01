#ifndef LIBTCOD_POINT2D_HPP
#define LIBTCOD_POINT2D_HPP

#include <string>
#include <cstdint>

namespace Doryen::Geometry
{

	/**
	 * The Point2D class defines a point representing a location in (x,y)
	 * coordinate space.
	 *
	 * @note If arguments are not provided, default arguments initialize
	 *  the point to (0, 0).
	 *
	 * @tparam T Arithmetic type: Integer, Real and Boolean
	 */
	template<typename T = std::int32_t>
	class Point2D
	{

		static_assert(std::is_arithmetic_v<T>, "Only arithmetic types are allowed (Operations: +, -, /, *)");

	public:

		// Fields Public

		T x{ 0 };
		T y{ 0 };

		// Constructs

		/**
		 * Initialize the point to (0, 0).
		 */
		Point2D() noexcept = default;

		/**
		 * The constructor initializes a new point with x and y coordinates.
		 *
		 * @param _x Coordinate in X.
		 * @param _y Coordinate in Y.
		 */
		Point2D(T _x, T _y) noexcept
		{
			x = _x;
			y = _y;
		}

		// Methods

		/**
		 * Determines whether or not two points are equal. Two instances of
		 * Point2D are equal if the values of their x and y member fields,
		 * representing their position in the coordinate space, are the same.
		 *
		 * @param _rhs an object to be compared with this Point2D
		 *
		 * @return true if the object to be compared is an instance of
		 *  Point2D and has the same values; false otherwise.
		 */
		bool equals(const Point2D<T>& _rhs) const noexcept
		{
			return this->x == _rhs.x and this->y == _rhs.y;
		}

		// Debug

		[[maybe_unused]] std::string toString() const
		{
			return { '{' + std::to_string(x) + ", " + std::to_string(y) + '}' };
		}

		// Getters

		/**
		 * @return the X coordinate of this Point2D.
		 */
		T getX() const noexcept
		{
			return x;
		}

		/**
		 * @return the Y coordinate of this Point2D.
		 */
		T getY() const noexcept
		{
			return y;
		}

		// Setters

		/**
		 * @param _x the new X coordinate of this Point2D
		 */
		void setX(T _x) noexcept
		{
			x = _x;
		}

		/**
		 * @param _y the new Y coordinate of this Point2D
		 */
		void setY(T _y) noexcept
		{
			y = _y;
		}

	};
}

#endif //LIBTCOD_POINT2D_HPP

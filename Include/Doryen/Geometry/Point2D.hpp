#ifndef LIBTCOD_POINT2D_HPP
#define LIBTCOD_POINT2D_HPP

#include <string>
#include <cstdint>

namespace Doryen
{
	namespace Geometry
	{
		template<typename T = std::int32_t>
		class Point2D
		{

		public:

			Point2D() noexcept
			{
				x = T{};
				y = T{};
			}

			Point2D(T _x, T _y) noexcept
			{
				x = _x;
				y = _y;
			}

			T x;
			T y;

			bool equals(const Point2D<T>& _rhs) const noexcept
			{
				return this->x == _rhs.x and this->y == _rhs.y;
			}

			// Debug

			[[maybe_unused]] std::string toString() const
			{
				return { '{' + std::to_string(x) + ", " + std::to_string(y) + '}' };
			}

		};
    }
}

#endif //LIBTCOD_POINT2D_HPP

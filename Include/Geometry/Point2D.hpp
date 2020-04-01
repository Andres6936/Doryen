#ifndef LIBTCOD_POINT2D_HPP
#define LIBTCOD_POINT2D_HPP

namespace Doryen
{
    namespace Geometry
	{
		template<typename T = int>
		class Point2D
		{

		public:

			Point2D(T _x, T _y)
			{
				x = _x;
				y = _y;
			}

			T x;
			T y;

		};
    }
}

#endif //LIBTCOD_POINT2D_HPP

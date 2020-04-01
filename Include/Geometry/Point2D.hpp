#ifndef LIBTCOD_POINT2D_HPP
#define LIBTCOD_POINT2D_HPP

namespace Doryen
{
    namespace Geometry
	{
		class Point2D
		{

		public:

			Point2D()
			{
				x = 0;
				y = 0;
            }

            Point2D( int X, int Y )
            {
                x = X;
                y = Y;
            }

            int x;
            int y;

        };
    }
}

#endif //LIBTCOD_POINT2D_HPP

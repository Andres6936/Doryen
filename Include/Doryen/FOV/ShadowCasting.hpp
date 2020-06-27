#ifndef LIBTCOD_SHADOWRAYCASTING_HPP
#define LIBTCOD_SHADOWRAYCASTING_HPP

#include <Doryen/FOV/Fov.hpp>

namespace Doryen
{

	class ShadowRayCasting
	{

	private:

		static void castLight(Doryen::Map& map, int cx, int cy,
				int row, float start, float end, int radius, int r2,
				int xx, int xy, int yx, int yy, int id, bool light_walls);

	public:

		void operator()(Doryen::Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls);

	};
}

#endif //LIBTCOD_SHADOWRAYCASTING_HPP

#ifndef LIBTCOD_SHADOWRAYCASTING_HPP
#define LIBTCOD_SHADOWRAYCASTING_HPP

#include <fov.hpp>

namespace Doryen
{

	class ShadowRayCasting
	{

	public:

		void operator()(Doryen::Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls);

	};
}

#endif //LIBTCOD_SHADOWRAYCASTING_HPP

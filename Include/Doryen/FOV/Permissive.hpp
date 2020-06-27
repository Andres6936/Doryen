#ifndef LIBTCOD_PERMISSIVE_HPP
#define LIBTCOD_PERMISSIVE_HPP

#include <Doryen/FOV/Fov.hpp>

namespace Doryen
{

	class Permissive
	{

	public:

		void operator()(Doryen::Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls, int fovType);
	};

}

#endif //LIBTCOD_PERMISSIVE_HPP

#ifndef LIBTCOD_RESTRICTIVE_HPP
#define LIBTCOD_RESTRICTIVE_HPP

#include <FOV/Fov.hpp>

namespace Doryen
{

	class Restrictive
	{

	public:

		void operator()(Doryen::Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls);

	};
}

#endif //LIBTCOD_RESTRICTIVE_HPP

#ifndef LIBTCOD_MAPDATA_HPP
#define LIBTCOD_MAPDATA_HPP

#include <libtcod.h>

class MapData
{

public:

	/**
	 * Direction of lowest neighbour
	 */
	uint8 flowDir = 0;
	float slope = 0;
	uint8 riverId = 0;

	MapData();

};


#endif //LIBTCOD_MAPDATA_HPP

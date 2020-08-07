#ifndef LIBTCOD_MAPDATA_HPP
#define LIBTCOD_MAPDATA_HPP

#include <cstdint>

class MapData
{

public:

	/**
	 * Direction of lowest neighbour
	 */
	std::uint8_t flowDir = 0;
	float slope = 0;
	std::uint8_t riverId = 0;

	MapData();

};


#endif //LIBTCOD_MAPDATA_HPP

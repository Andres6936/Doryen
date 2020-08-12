#ifndef LIBTCOD_FROSTMANAGER_HPP
#define LIBTCOD_FROSTMANAGER_HPP

#pragma once

#include "Doryen/Doryen.hpp"

using namespace Doryen;

class Frost;

class FrostManager
{

private:

	/**
	 * The width.
	 */
	int w;

	/**
	 * The height.
	 */
	int h;

	float* grid;

	Console& console;

	Doryen::Color frostColor[256];

	std::vector<Frost> frost;

	Doryen::Image* img;

public :

	FrostManager(int width, int height, Console& _console);

    ~FrostManager( );

    void addFrost( int x, int y );

    void update( float elapsed );

    void render( );

    void clear( );

    inline float getValue( int cx, int cy )
	{
		if (cx + cy * w >= 0 && cx + cy * w < w * h)
		{
			return grid[cx + cy * w];
		}

		return 0.0f;
	}

    inline void setValue( int cx, int cy, float v )
	{
		if (cx + cy * w >= 0 && cx + cy * w < w * h)
		{
			grid[cx + cy * w] = v;
		}
	}

    Doryen::Color *getFrostColor( );

protected :

    friend class Frost;
};

#endif //LIBTCOD_FROSTMANAGER_HPP

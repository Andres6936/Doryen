#ifndef LIBTCOD_FROSTMANAGER_HPP
#define LIBTCOD_FROSTMANAGER_HPP

#pragma once

#include <vector>
#include "libtcod.hpp"
#include "libtcod_int.h"

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

    float *grid;

    Doryen::Color frostColor[256];

    std::vector <Frost> frost;

    Doryen::TCODImage* img;

public :

    FrostManager( int width, int height );

    ~FrostManager( );

    void addFrost( int x, int y );

    void update( float elapsed );

    void render( );

    void clear( );

    inline float getValue( int cx, int cy )
    {
        TCOD_IFNOT( cx + cy * w >= 0 && cx + cy * w < w * h )
        { return 0.0f; }
        return grid[ cx + cy * w ];
    }

    inline void setValue( int cx, int cy, float v )
    {
        TCOD_IFNOT( cx + cy * w >= 0 && cx + cy * w < w * h )
        { return; }
        grid[ cx + cy * w ] = v;
    }

    Doryen::Color *getFrostColor( );

protected :

    friend class Frost;
};

#endif //LIBTCOD_FROSTMANAGER_HPP

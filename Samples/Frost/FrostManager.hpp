#ifndef LIBTCOD_FROSTMANAGER_HPP
#define LIBTCOD_FROSTMANAGER_HPP

#pragma once

#include "libtcod.hpp"
#include "libtcod_int.h"

class Frost;

class FrostManager
{

public :

    Doryen::Color frostCol[256];

    Doryen::Color *getFrostCol( );

    FrostManager( int w, int h );

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

protected :

    friend class Frost;

    TCODList <Frost *> list;
    float *grid;
    TCODImage *img;
    int w, h;
};

#endif //LIBTCOD_FROSTMANAGER_HPP

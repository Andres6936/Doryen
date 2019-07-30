#ifndef LIBTCOD_FROST_HPP
#define LIBTCOD_FROST_HPP

#pragma once

#include <cmath>

#include "FrostManager.hpp"

#define GROW 5000.0f
#define ANGLE_DELAY 0.2f
#define FROST_LEVEL 0.8f
#define SMOOTH 0.3f
#define PIX_PER_FRAME 6
#define RANGE 10

class Frost
{

private:

    int x;
    int y;

    int bestx;
    int besty;

    int rx;
    int ry;

    int border;

    float timer;
    float ra;
    float rr;

    FrostManager *manager;

public:

    Frost( int x, int y, FrostManager *nFrostManager );

    inline float getValue( int cx, int cy )
    {
        return manager->getValue( x - RANGE + cx, y - RANGE + cy );
    }

    inline void setValue( int cx, int cy, float v )
    {
        manager->setValue( x - RANGE + cx, y - RANGE + cy, v );
    }

    bool update( float elapsed );

    void render( TCODImage *img );
};

#endif //LIBTCOD_FROST_HPP

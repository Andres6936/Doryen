#ifndef LIBTCOD_CIRCULARRAYCASTING_HPP
#define LIBTCOD_CIRCULARRAYCASTING_HPP

#pragma once

#include <Doryen/FOV/Fov.hpp>

namespace Doryen
{
    class CircularRaycasting
    {

    private:

        static void castRay( Doryen::Map &map, int xo, int yo, int xd, int yd,
                             int radiusDouble, bool ligthWalls );

        static void postProcessing( Doryen::Map &map, int x0, int y0, int x1,
                                    int y1, int dx, int dy );

    public:

        void operator()( Doryen::Map &map, int playerX, int playerY,
                         int maxRadius, bool ligthWalls );

    };
}

#endif //LIBTCOD_CIRCULARRAYCASTING_HPP

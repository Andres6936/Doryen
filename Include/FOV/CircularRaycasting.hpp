#ifndef LIBTCOD_CIRCULARRAYCASTING_HPP
#define LIBTCOD_CIRCULARRAYCASTING_HPP

#pragma once

#include <fov.hpp>

namespace Doryen
{
    class CircularRaycasting
    {

    private:

        static void castRay( Doryen::Map &map, int xo, int yo, int xd, int yd,
                             int radiusDouble, bool ligthWalls );

    public:

        void operator()( Doryen::Map &map, int playerX, int playerY,
                         int maxRadius, bool ligthWalls );

    };
}

#endif //LIBTCOD_CIRCULARRAYCASTING_HPP

#ifndef LIBTCOD_CELL_HPP
#define LIBTCOD_CELL_HPP

namespace Doryen
{
    class Cell
    {

    public:

        bool transparent;
        bool walkable;
        bool fov;

        Cell( );

        Cell( bool isTransparent, bool isWalkable, bool isFOV );
    };
}

#endif //LIBTCOD_CELL_HPP

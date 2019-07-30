#ifndef LIBTCOD_LINELISTENER_HPP
#define LIBTCOD_LINELISTENER_HPP

#include <libtcod.hpp>

#include "SampleRenderer.hpp"

class LineListener : public TCODLineListener
{

public :

    bool putPoint( int x, int y );
};


#endif //LIBTCOD_LINELISTENER_HPP

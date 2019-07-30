#ifndef LIBTCOD_LINELISTENER_HPP
#define LIBTCOD_LINELISTENER_HPP

#include <libtcod.hpp>

#include "SampleRenderer.hpp"

class LineListener : public TCODLineListener
{
public :
    bool putPoint( int x, int y )
    {
        if ( x >= 0 && y >= 0 && x < SAMPLE_SCREEN_WIDTH && y < SAMPLE_SCREEN_HEIGHT )
        {
            sampleConsole.setCharBackground( x, y, Doryen::Color::lightBlue, ( TCOD_bkgnd_flag_t ) bkFlag );
        }
        return true;
    }
};


#endif //LIBTCOD_LINELISTENER_HPP

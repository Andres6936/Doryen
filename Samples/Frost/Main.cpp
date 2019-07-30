#include "libtcod.hpp"

#include "FrostManager.hpp"
#include "Frost.hpp"

int keys[4] = { 0, 60, 200, 255 };

Doryen::Color keyCols[4] = { Doryen::Color::black, Doryen::Color::darkerBlue, Doryen::Color::lighterBlue,
                             Doryen::Color::lightestBlue };

int main( )
{
    Doryen::Console console = Doryen::Console( );
    console.initRoot( 80, 50, "Frost test", false );

    FrostManager frostManager( 160, 100 );

    TCOD_mouse_t mouse;
    TCOD_key_t key;
    Doryen::Platform::setFps( 25 );
    Doryen::Color::genMap( frostManager.getFrostCol( ), sizeof( keys ) / sizeof( int ), keyCols, keys );

    while ( !Doryen::Console::isWindowClosed( ))
    {
        frostManager.render( );
        Doryen::Console::flush( );
        Doryen::Platform::checkForEvent( TCOD_EVENT_KEY_RELEASE | TCOD_EVENT_MOUSE, &key, &mouse );
        if ( key.vk == TCODK_BACKSPACE )
        { frostManager.clear( ); }
        if ( mouse.lbutton )
        {
            frostManager.addFrost( mouse.cx * 2, mouse.cy * 2 );
        }
        frostManager.update( Doryen::Platform::getLastFrameLength( ));
    }

    return 0;
}
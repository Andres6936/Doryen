#include "Renderer/Sdl.hpp"

void Doryen::SDL::setFps( const short value )
{
    if ( value == 0 )
    {
        minimunFrameLength = 0;
    }
    else
    {
        minimunFrameLength = 1000 / value;
    }
}

short Doryen::SDL::getFps( ) const
{
    return framePerSecond;
}

float Doryen::SDL::getLasFrameLength( ) const
{
    return lastFrameLength;
}

void Doryen::SDL::sleepMilli( int milliseconds )
{
    SDL_Delay( milliseconds );
}

int Doryen::SDL::getElapsedMilli( ) const
{
    return SDL_GetTicks( );
}

float Doryen::SDL::getElapsedSeconds( ) const
{
    return SDL_GetTicks( ) * 1.0f / 1000.0f;
}

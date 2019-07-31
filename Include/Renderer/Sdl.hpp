#ifndef LIBTCOD_SDL_HPP
#define LIBTCOD_SDL_HPP

#include <SDL/SDL.h>

namespace Doryen
{
    class SDL
    {

    private:

        /**
         * Minimum length for a frame (when fps are limited)
         */
        int minimunFrameLength = 0;

        /**
         * Minimum length for a frame (when fps are limited)
         */
        int minimunFrameLengthBackup = 0;

        /**
         * Length of the last rendering loop.
         */
        float lastFrameLength = 0.0f;

        /**
         * Number of frames in the last second.
         */
        short framePerSecond = 0;

        /**
         * Current number of frames.
         */
        short currentFramePerSecond = 0;

    public:

        void setFps( short value );

        short getFps( ) const;

        float getLasFrameLength( ) const;

        void sleepMilli( int milliseconds );

        int getElapsedMilli( ) const;

        float getElapsedSeconds( ) const;
    };
}

#endif //LIBTCOD_SDL_HPP
#ifndef LIBTCOD_SAMPLERENDERER_HPP
#define LIBTCOD_SAMPLERENDERER_HPP

#pragma once

#include <SDL/SDL.h>

#include <Doryen/libtcod.hpp>

// sample screen size
constexpr short SAMPLE_SCREEN_WIDTH = 46;

constexpr short SAMPLE_SCREEN_HEIGHT = 20;

// sample screen position
constexpr short SAMPLE_SCREEN_X = 20;

constexpr short SAMPLE_SCREEN_Y = 10;

static bool sdl_callback_enabled = false;

// the offscreen console in which the samples are rendered
static Doryen::Console sampleConsole( SAMPLE_SCREEN_WIDTH, SAMPLE_SCREEN_HEIGHT );

/**
 * current blending mode
 */
static Doryen::BackgroundFlag backFlag = Doryen::BackgroundFlag::SET;

class SampleRenderer : public CallbackRender
{

public :

	SampleRenderer() : effectNum(0), delay(3.0f)
	{
		noise = new TCODNoise(3);
	}

	~SampleRenderer() override
    {
        delete noise;
    }

    void render( void *sdlSurface ) override;

protected :

    TCODNoise *noise;

    int effectNum;

    float delay;

    void burn( SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh );

    void explode( SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh );

    void blur( SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh );
};


#endif //LIBTCOD_SAMPLERENDERER_HPP

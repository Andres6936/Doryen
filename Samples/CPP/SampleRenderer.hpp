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

// the offscreen console in which the samples are rendered
static Doryen::Console sampleConsole( SAMPLE_SCREEN_WIDTH, SAMPLE_SCREEN_HEIGHT );

/**
 * current blending mode
 */
static Doryen::BackgroundFlag backFlag = Doryen::BackgroundFlag::SET;

class SampleRenderer : public Doryen::CallbackRender
{

private:

	enum class State
	{
		Draw_Burn,
		Draw_Blur,
		Draw_Explode,
		Stop,
	};

	SDL_Surface* screen;

	float delay = 2.0f;

	State state = State::Draw_Blur;

	TCODNoise* noise = new TCODNoise(3);

	void burn(SDL_Surface* screen, int samplex, int sampley, int samplew, int sampleh);

	void explode(SDL_Surface* screen, int samplex, int sampley, int samplew, int sampleh);

	void blur(SDL_Surface* screen, int samplex, int sampley, int samplew, int sampleh);

	void nextState();

public :

	SampleRenderer() = default;

	~SampleRenderer() override
	{
		delete noise;
	}

	void render(std::any _surface) override;

};


#endif //LIBTCOD_SAMPLERENDERER_HPP

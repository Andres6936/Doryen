#ifndef LIBTCOD_SAMPLERENDERER_HPP
#define LIBTCOD_SAMPLERENDERER_HPP

#pragma once

#include <SDL/SDL.h>

#include <Doryen/Doryen.hpp>

// sample screen size
constexpr short SAMPLE_SCREEN_WIDTH = 46;

constexpr short SAMPLE_SCREEN_HEIGHT = 20;

// sample screen position
constexpr short SAMPLE_SCREEN_X = 20;

constexpr short SAMPLE_SCREEN_Y = 10;

/**
 * current blending mode
 */
static Doryen::BlendModes backFlag = Doryen::BlendModes::SET;

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

	SDL_Surface* screen = nullptr;

	float delay = 2.0f;

	Doryen::Console& console;

	State state = State::Draw_Blur;

	Doryen::Noise* noise = new Doryen::Noise(3);

	void effectBurn(int samplex, int sampley, int samplew, int sampleh);

	void effectBlur(int samplex, int sampley, int samplew, int sampleh);

	void effectExplode(int samplex, int sampley, int samplew, int sampleh);

	void nextState();

public :

	SampleRenderer(Doryen::Console& _console) : console(_console)
	{
	};

	~SampleRenderer() override
	{
		delete noise;
	}

	void render(std::any _surface) override;

};


#endif //LIBTCOD_SAMPLERENDERER_HPP

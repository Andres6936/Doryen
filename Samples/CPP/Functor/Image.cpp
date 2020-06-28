// Joan Andrés (@Andres6936) Github.

#include "Image.hpp"

using namespace Doryen;


Functor::Image::Image(std::string _name, const Console& _console) : ISample(_name), sample(_console)
{

}

void Functor::Image::render(KeyCode key, const Mouse& mouse)
{
	static Doryen::Image* img = NULL, * circle = NULL;
	static Doryen::Color blue(0, 0, 255);
	static Doryen::Color green(0, 255, 0);
	if (img == NULL)
	{
		img = new Doryen::Image("Data/img/skull.png");
		img->setKeyColor(Doryen::Color::black);
		circle = new Doryen::Image("Data/img/circle.png");
	}

	sample.setDefaultBackground(Doryen::Color::black);
	sample.clear();
	float x = sample.getWidth() / 2 + cosf(Doryen::Platform::getElapsedSeconds()) * 10.0f;
	float y = (float)(sample.getHeight() / 2);
	float scalex = 0.2f + 1.8f * (1.0f + cosf(Doryen::Platform::getElapsedSeconds() / 2)) / 2.0f;
	float scaley = scalex;
	float angle = Doryen::Platform::getElapsedSeconds();
	long elapsed = Doryen::Platform::getElapsedMilli() / 2000;
	if (elapsed & 1)
	{
		// split the color channels of circle.png
		// the red channel
		sample.setDefaultBackground(Doryen::Color::red);
		sample.rect(0, 3, 15, 15, false, Doryen::BackgroundFlag::SET);
		circle->blitRect(&sample, 0, 3, -1, -1, TCOD_BKGND_MULTIPLY);
		// the green channel
		sample.setDefaultBackground(green);
		sample.rect(15, 3, 15, 15, false, Doryen::BackgroundFlag::SET);
		circle->blitRect(&sample, 15, 3, -1, -1, TCOD_BKGND_MULTIPLY);
		// the blue channel
		sample.setDefaultBackground(blue);
		sample.rect(30, 3, 15, 15, false, Doryen::BackgroundFlag::SET);
		circle->blitRect(&sample, 30, 3, -1, -1, TCOD_BKGND_MULTIPLY);
	}
	else
	{
		// render circle.png with normal blitting
		circle->blitRect(&sample, 0, 3, -1, -1, TCOD_BKGND_SET);
		circle->blitRect(&sample, 15, 3, -1, -1, TCOD_BKGND_SET);
		circle->blitRect(&sample, 30, 3, -1, -1, TCOD_BKGND_SET);
	}

	img->blit(sample, { static_cast<int>(x), static_cast<int>(y) },
			BackgroundFlag::SET, scalex, scaley, angle);
}

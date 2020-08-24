// Joan Andrés (@Andres6936) Github.

#include "Image.hpp"

using namespace Doryen;


Functor::Image::Image(Console& _console) : ISample("Image toolkit", _console)
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
		img->setKeyColor(Doryen::Palette::GRAY_WARN_90);
		circle = new Doryen::Image("Data/img/circle.png");
	}

	sample.setBackgroundColor(Doryen::Palette::GRAY_WARN_90);
	sample.clear();
	float x = sample.getWidth() / 2 + cosf(sample.getElapsedSeconds()) * 10.0f;
	float y = (float)(sample.getHeight() / 2);
	float scalex = 0.2f + 1.8f * (1.0f + cosf(sample.getElapsedSeconds() / 2)) / 2.0f;
	float scaley = scalex;
	float angle = sample.getElapsedSeconds();
	long elapsed = sample.getElapsedMilliseconds() / 2000;
	if (elapsed & 1)
	{
		// split the color channels of circle.png
		// the red channel
		sample.setBackgroundColor(Doryen::Palette::RED);
		sample.drawFillRect(0, 3, 15, 15, false, Doryen::BlendModes::SET);
		circle->blitRect(sample, 0, 3, -1, -1, BlendModes::MULTIPLY);
		// the green channel
		sample.setBackgroundColor(green);
		sample.drawFillRect(15, 3, 15, 15, false, Doryen::BlendModes::SET);
		circle->blitRect(sample, 15, 3, -1, -1, BlendModes::MULTIPLY);
		// the blue channel
		sample.setBackgroundColor(blue);
		sample.drawFillRect(30, 3, 15, 15, false, Doryen::BlendModes::SET);
		circle->blitRect(sample, 30, 3, -1, -1, BlendModes::MULTIPLY);
	}
	else
	{
		// render circle.png with normal blitting
		circle->blitRect(sample, 0, 3);
		circle->blitRect(sample, 15, 3);
		circle->blitRect(sample, 30, 3);
	}

	img->blit(sample, { static_cast<int>(x), static_cast<int>(y) },
			BlendModes::SET, scalex, scaley, angle);
}

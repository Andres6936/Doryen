/*
* Copyright (c) 2010 Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cstdio>
#include "Main.hpp"

TCODNoise noise1d(1);

TCODNoise noise2d(2);

Weather weather;

float dayTime = 6 * 3600.0f; // starts at 6.00am
Doryen::Color lightningColor(220, 220, 255);

Doryen::Image* ground;

void update(float elapsed, Doryen::Key k, TCOD_mouse_t mouse)
{
	if (k.getKeyCode() == Doryen::KeyCode::KP_ADD)
	{
		float d = weather.getIndicatorDelta();
		weather.setIndicatorDelta(d + elapsed * 0.1f);
	}
	else if (k.getKeyCode() == Doryen::KeyCode::KP_SUB)
	{
		float d = weather.getIndicatorDelta();
		weather.setIndicatorDelta(d - elapsed * 0.1f);
	}
	else if (k.getKeyCode() == Doryen::KeyCode::ENTER ||
			 k.getKeyCode() == Doryen::KeyCode::KP_ENTER)
	{
		elapsed *= 20.0f;
	}

	dayTime += elapsed * 60 * 3; // 1 real sec = 3 min
	if (dayTime >= 24 * 3600)
	{ dayTime -= 24 * 3600; }
	weather.update(elapsed);
	weather.calculateAmbient(dayTime);
}

const char* getDaytime()
{
	static char buf[6];
	int h = (int)(dayTime / 3600);
	int m = (int)((dayTime - h * 3600) / 60);
	sprintf(buf, "%02d:%02d", h, m);
	return buf;
}

void render()
{
	static Doryen::Image img(CON_W * 2, CON_H * 2);
	for (int x = 0; x < CON_W * 2; x++)
	{
		for (int y = 0; y < CON_H * 2; y++)
		{
			// we don't use color operation to avoid 0-255 clamping at every step
			// sort of poor man's HDR...
			int r = 0, g = 0, b = 0;

			// default ground color
			Doryen::Color groundCol = ground->getPixel(x, y);

			// take cloud shadow into account
			float cloudCoef = weather.getCloud(x, y);
			r += (int)(cloudCoef * weather.getAmbientLightColor().r);
			g += (int)(cloudCoef * weather.getAmbientLightColor().g);
			b += (int)(cloudCoef * weather.getAmbientLightColor().b);

			// take lightning into account
			int lr = 0, lg = 0, lb = 0;
			float lightning = weather.getLightning(x, y);
			if (lightning > 0.0f)
			{
				lr = (int)(2 * lightning * lightningColor.r);
				lg = (int)(2 * lightning * lightningColor.g);
				lb = (int)(2 * lightning * lightningColor.b);
				r += lr;
				g += lg;
				b += lb;
			}
			r = MIN(255, r);
			g = MIN(255, g);
			b = MIN(255, b);
			r = groundCol.r * r / 200;
			g = groundCol.g * g / 200;
			b = groundCol.b * b / 200;
			img.putPixel(x, y, Doryen::Color(r, g, b));
		}
	}
	img.blit2x(Doryen::Console::root, 0, 0);
	// rain drops
	for (int x = 0; x < CON_W; x++)
	{
		for (int y = 0; y < CON_H; y++)
		{
			if (weather.hasRainDrop())
			{
				float lightning = weather.getLightning(x * 2, y * 2);
				float cloudCoef = weather.getCloud(x * 2, y * 2);
				Doryen::Color col = Doryen::Color::darkBlue * cloudCoef;
				col = col * weather.getAmbientLightColor();
				if (lightning > 0.0f)
				{ col = col + 2 * lightning * lightningColor; }
				Doryen::Console::root->setChar(x, y, '/');
				Doryen::Console::root->setCharForeground(x, y, col);
			}
		}
	}
	Doryen::Console::root->setDefaultForeground(Doryen::Color::white);
	Doryen::Console::root->print(5, CON_H - 12, "TCOD's Weather system :\n"
												"- wind with varying speed and direction\n"
												"- rain\n"
												"- lightnings\n"
												"- day/night cycle\n"
												"Day time : %s\n"
												"Weather : %s\n\n"
												"Weather evolves automatically\nbut you can alter it by holding + or - : %.1f\n"
												"Accelerate time with ENTER", getDaytime(), weather.getWeather(),
			weather.getIndicatorDelta());
}

int main(int argc, char* argv[])
{
	// initialize the game window
	Doryen::Console console = Doryen::Console();
	console.initRoot(CON_W, CON_H, "Weather system v 0.1.0", false, TCOD_RENDERER_SDL);
	TCODMouse::showCursor(true);
	Doryen::Platform::setFps(25);

	weather.init(CON_W * 2, CON_H * 2);
	ground = new Doryen::Image(CON_W * 2, CON_H * 2);
	// generate some good locking ground
	Doryen::Color colors[] = {
			Doryen::Color(40, 117, 0), // grass
			Doryen::Color(69, 125, 0), // sparse grass
			Doryen::Color(110, 125, 0), // withered grass
			Doryen::Color(150, 143, 92), // dried grass
			Doryen::Color(133, 115, 71), // bare ground
			Doryen::Color(111, 100, 73) // dirt
	};
	int keys[] = {
			0, 51, 102, 153, 204, 255
	};
	Doryen::Color gradientMap[256];
	Doryen::Color::genMap(gradientMap, 6, colors, keys);
	for (int x = 0; x < CON_W * 2; x++)
	{
		for (int y = 0; y < CON_H * 2; y++)
		{
			float f[2] = { x * 3.0f / CON_W, y * 3.0f / CON_H };
			float h = noise2d.getFbm(f, 6.0f, TCOD_NOISE_SIMPLEX);
			int ih = (int)(h * 256);
			ih = CLAMP(0, 255, ih);
			float coef = 1.0f;
			// darken the lower part (text background) 
			if (y > CON_H * 2 - 27)
			{ coef = 0.5f; }
			Doryen::Color col = coef * gradientMap[ih];
			// add some noise
			col = col * TCODRandom::getInstance()->getFloat(0.95f, 1.05f);
			ground->putPixel(x, y, col);
		}
	}

	// for this demo, we want the weather to evolve quite rapidely
	weather.setChangeFactor(3.0f);

	bool endCredits = false;

	while (!console.isWindowClosed())
	{
		//	read keyboard
		Doryen::Key keyPressed = Doryen::Console::getKeyPressed();

		TCOD_mouse_t mouse = TCODMouse::getStatus();

		if (keyPressed.getKeyCode() == Doryen::KeyCode::PRINT_SCREEN)
		{
			// screenshot
			Doryen::Platform::saveScreenshot(NULL);
		}
		else if (keyPressed.isLeftAltPressed() &&
				 (keyPressed.getKeyCode() == Doryen::KeyCode::ENTER ||
				  keyPressed.getKeyCode() == Doryen::KeyCode::KP_ENTER))
		{
			// switch fullscreen
			Doryen::Console::setWindowInFullscreen();
		}
		// update the game
		update(Doryen::Platform::getLastFrameLength(), keyPressed, mouse);

		// render the game screen
		render();
		// render libtcod credits
		if (!endCredits)
		{ endCredits = Doryen::Console::renderCredits(4, 4, true); }
		// flush updates to screen
		Doryen::Console::root->flush();
	}
	return 0;
}

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

using namespace Doryen;

Noise noise1d(1);

Noise noise2d(2);

Weather weather;

float dayTime = 6 * 3600.0f; // starts at 6.00am
Doryen::Color lightningColor(220, 220, 255);

Doryen::Image* ground;

void update(float elapsed, const KeyCode& k, const std::uint32_t framePerSeconds)
{
	if (k == KeyCode::KP_ADD)
	{
		float d = weather.getIndicatorDelta();
		weather.setIndicatorDelta(d + elapsed * 0.1f);
	}
	else if (k == KeyCode::KP_SUB)
	{
		float d = weather.getIndicatorDelta();
		weather.setIndicatorDelta(d - elapsed * 0.1f);
	}
	else if (k == KeyCode::ENTER || k == KeyCode::KP_ENTER)
	{
		elapsed *= 20.0f;
	}

	dayTime += elapsed * 60 * 3; // 1 real sec = 3 min
	if (dayTime >= 24 * 3600)
	{ dayTime -= 24 * 3600; }
	weather.update(elapsed, framePerSeconds);
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

void render(Console& root)
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
			img.setPixel(x, y, Doryen::Color(r, g, b));
		}
	}
	img.blit2x(root, 0, 0);
	// rain drops
	for (int x = 0; x < CON_W; x++)
	{
		for (int y = 0; y < CON_H; y++)
		{
			if (weather.hasRainDrop())
			{
				float lightning = weather.getLightning(x * 2, y * 2);
				float cloudCoef = weather.getCloud(x * 2, y * 2);

				Doryen::Color col = Palette::BASE_DARKEST;
				col.multiply(cloudCoef);
				col.multiply(weather.getAmbientLightColor());

				if (lightning > 0.0f)
				{
					// For avoid edit the original color
					Color recoveryLightingColor = lightningColor;
					recoveryLightingColor.multiply(lightning);
					recoveryLightingColor.multiply(2);

					col = recoveryLightingColor;
				}

				root.writeChar(x, y, '/');
				root.setCharForeground(x, y, col);
			}
		}
	}
	root.setDefaultForeground(Palette::GRAY_WARN_1);

	root.write(5, CON_H - 12, "Doryen Weather System:");
	root.write(5, CON_H - 11, "- Wind with varying speed and direction");
	root.write(5, CON_H - 10, "- Rain");
	root.write(5, CON_H - 9, "- Lightnings");
	root.write(5, CON_H - 8, "- Day/Night cycle");
	root.write(5, CON_H - 7, format("Day time : {}", getDaytime()));
	root.write(5, CON_H - 6, format("Weather : {}", weather.getWeather()));

	root.write(5, CON_H - 4, "Weather evolves automatically");
	root.write(5, CON_H - 3, format("but you can alter it by holding + or - : {.1f}", weather.getIndicatorDelta()));
	root.write(5, CON_H - 2, "Accelerate time with ENTER");
}


/**
 * @brief Generate a smooth color map.
 *
 * You can define a color map from an array of color keys. Colors will be interpolated <br>
 * between the keys.
 *
 * 0 -> black
 * 4 -> red
 * 8 -> white
 *
 * Result :
 *
 * <table>
 * 	<tbody>
 * 		<tr><td>map[0]</td><td style="background-color: rgb(0, 0, 0); width: 50px;" /><td>black</td></tr>
 * 		<tr><td>map[1]</td><td style="background-color: rgb(63, 0, 0);" /></tr>
 * 		<tr><td>map[2]</td><td style="background-color: rgb(127, 0, 0);" /></tr>
 * 		<tr><td>map[3]</td><td style="background-color: rgb(191, 0, 0);" /></tr>
 * 		<tr><td>map[4]</td><td style="background-color: rgb(255, 0, 0);" /><td>red</td></tr>
 * 		<tr><td>map[5]</td><td style="background-color: rgb(255, 63, 63);" /></tr>
 * 		<tr><td>map[6]</td><td style="background-color: rgb(255, 127, 127);" /></tr>
 * 		<tr><td>map[7]</td><td style="background-color: rgb(255, 191, 191);" /></tr>
 * 		<tr><td>map[8]</td><td style="background-color: rgb(255, 255, 255);" /><td>white</td></tr>
 * 	</tbody>
 * </table>
 *
 * If you want to fill the map array, keyIndex[0] must be 0 and keyIndex[nbKey-1] <br>
 * is the number of elements in map minus 1 but you can also use the function to <br>
 * fill only a part of the map array.
 *
 * @param map	An array of colors to be filled by the function.
 * @param nbKey	Number of color keys
 * @param keyColor	Array of nbKey colors containing the color of each key
 * @param keyIndex	Array of nbKey integers containing the index of each key.
 *
 * @example
 * @code
 * int idx[] = { 0, 4, 8 }; // indexes of the keys
 *
 * // Colors : Black, Red, White
 * Doryen::TCODColor col[] = { {0, 0, 0}, {255, 0, 0}, {255, 255, 255} };
 * Doryen::TCODColor map[9];
 * Doryen::TCODColor::genMap(map,3,col,idx);
 * @endcode
 */
void generateSmoothColorMap(Color* map, int nbKey, Color const* keyColor, int const* keyIndex)
{
	for (int segment = 0; segment < nbKey - 1; segment++)
	{
		int idxStart = keyIndex[segment];
		int idxEnd = keyIndex[segment + 1];
		int idx;
		for (idx = idxStart; idx <= idxEnd; idx++)
		{
			map[idx] = Color::lerp(keyColor[segment], keyColor[segment + 1],
					(float)(idx - idxStart) / (idxEnd - idxStart));
		}
	}
}

int main(int argc, char* argv[])
{
	// initialize the game window
	Doryen::Console console = Doryen::Console();

	console.initRoot(CON_W, CON_H, "Weather system v 0.1.0", false);
	console.showCursor(true);
	console.setFramePerSeconds(25);

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
	generateSmoothColorMap(gradientMap, 6, colors, keys);
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

			// For avoid edit the original color
			Color gradinatMapIndex = gradientMap[ih];
			gradinatMapIndex.multiply(coef);
			Color col = gradinatMapIndex;
			// add some noise
			col.multiply(Random::Number::nextFloat(0.95f, 1.05f));
			ground->setPixel(x, y, col);
		}
	}

	// for this demo, we want the weather to evolve quite rapidely
	weather.setChangeFactor(3.0f);

	bool endCredits = false;

	while (console.isRunning())
	{
		//	read keyboard
		const KeyCode keyPressed = console.getKeyPressed().getKeyCode();

		if (keyPressed == KeyCode::PRINT_SCREEN)
		{
			// screenshot
			Doryen::Platform::saveScreenshot(NULL);
		}
		else if (keyPressed == KeyCode::SPACE)
		{
			// switch fullscreen
			//Doryen::Console::setWindowInFullscreen();
		}

		// update the game
		update(console.getLastFrameLength(), keyPressed, console.getFramePerSeconds());

		// render the game screen
		render(console);

		// flush updates to screen
		console.draw();
	}
	return 0;
}

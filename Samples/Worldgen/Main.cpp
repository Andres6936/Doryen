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
#include "Main.hpp"

using namespace Doryen;

#define WIDTH 80
#define HEIGHT 50

Noise<1> noise1d{};

Noise<2> noise2d{};

WorldGenerator worldGen;

// world map panning
float wx = 0, wy = 0, curwx = 0, curwy = 0;

// mouse coordinates in world map
float mx = 0, my = 0;

void update(float elapsed, const KeyCode& k, const Mouse& mouse)
{
	// destination wanted
	wx = (worldGen.getWidth() - 2 * WIDTH) * mouse.getPositionCellX() / WIDTH;
	wy = (worldGen.getHeight() - 2 * HEIGHT) * mouse.getPositionCellY() / HEIGHT;
	curwx += (wx - curwx) * elapsed;
	curwy += (wy - curwy) * elapsed;
	mx = curwx + mouse.getPositionCellX() * 2;
	my = curwy + mouse.getPositionCellY() * 2;
	worldGen.updateClouds(elapsed);
}

Color getMapShadedColor(float worldX, float worldY, bool clouds)
{
	// sun color
	static Color sunCol(255, 255, 200);
	float wx = std::clamp(worldX, 0.0f, (float)worldGen.getWidth() - 1);
	float wy = std::clamp(worldY, 0.0f, (float)worldGen.getHeight() - 1);
	// apply cloud shadow
	float cloudAmount = clouds ? worldGen.getCloudThickness(wx, wy) : 0.0f;
	Color col = worldGen.getInterpolatedColor(worldX, worldY);
	// apply sun light
	float intensity = worldGen.getInterpolatedIntensity(wx, wy);
	intensity = std::min(intensity, 1.5f - cloudAmount);
	int cr = (int)(intensity * (int)(col.r) * sunCol.r / 255);
	int cg = (int)(intensity * (int)(col.g) * sunCol.g / 255);
	int cb = (int)(intensity * (int)(col.b) * sunCol.b / 255);
	Color col2;
	col2.r = std::clamp(cr, 0, 255);
	col2.g = std::clamp(cg, 0, 255);
	col2.b = std::clamp(cb, 0, 255);
	col2.r = std::max(col2.r, col.r / 2);
	col2.g = std::max(col2.g, col.g / 2);
	col2.b = std::max(col2.b, col.b / 2);
	return col2;
}

void render(Console& console)
{
	// subcell resolution image
	static Image map(WIDTH * 2, HEIGHT * 2);
	// compute the map image
	for (int px = 0; px < 2 * WIDTH; px++)
	{
		for (int py = 0; py < 2 * HEIGHT; py++)
		{
			// world texel coordinate (with fisheye distorsion)
			float wx = px + curwx;
			float wy = py + curwy;
			map.setPixel(px, py, getMapShadedColor(wx, wy, true));
		}
	}

	map.blit2x(console, 0, 0);

	console.setForegroundColor(Palette::GRAY_WARN_1);
	static const char* biomeNames[] = {
			"Tundra", "Cold desert", "Grassland", "Boreal forest",
			"Temperate forest", "Tropical/Montane forest",
			"Hot desert", "Savanna", "Tropical dry forest", "Tropical evergreen forest",
			"Thorn forest"
	};
	if (worldGen.isOnSea(mx, my))
	{
		// some information are irrelevant on sea
		console.write(5, 47, format("Alt {5d}m\n\nMove the mouse to scroll the map",
				(int)worldGen.getRealAltitude(mx, my))
		);
	}
	else
	{
		console.write(5, 47,
				format("Alt {5d}m  Prec {3d}cm/sq. m/y  Temp {d} deg C\nBiome : {s}\nMove the mouse to scroll the map",
						(int)worldGen.getRealAltitude(mx, my),
						(int)worldGen.getPrecipitations(mx, my),
						(int)worldGen.getTemperature(mx, my),
						biomeNames[worldGen.getBiome(mx, my)])
		);
	}
}

int main(int argc, char* argv[])
{
	// initialize the game window
	Console console{};

	console.setWindowTitle("World generator v 0.1.0");
	console.setFramePerSeconds(25);
	console.showCursor(true);

	bool endCredits = false;

	// generate the world with all data (rain, temperature and so on...)
	worldGen.generate();

	// compute light intensity on ground (depends on light direction and ground slope)
	static float lightDir[3] = {
			1.0f, 1.0f, 0.0f
	};

	worldGen.computeSunLight(lightDir);

	while (console.isRunning())
	{
		//	read keyboard
//		TCOD_key_t k=Console::checkForKeypress(TCOD_KEY_PRESSED|TCOD_KEY_RELEASED);
//		TCOD_mouse_t mouse=TCODMouse::getStatus();

		const KeyCode k = console.getKeyPressed().getKeyCode();
		const Mouse mouse = console.getMouseEvent();

		// update the game
		update(console.getLastFrameLength(), k, mouse);

		// render the game screen
		render(console);

		// flush updates to screen
		console.draw();
	}
	return 0;
}

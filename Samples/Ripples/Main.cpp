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
#include "Doryen/Doryen.hpp"

using namespace Doryen;

Noise noise2d(2);
Noise noise3d(3);
RippleManager* rippleManager;

Doryen::Image* ground, * ground2;

Doryen::Color mapGradient[256];
#define MAX_COLOR_KEY 8

// TCOD's land color map
float sandHeight = 0.3f;
float grassHeight = 0.5f;
float snowHeight=0.9f;

static int keyIndex[MAX_COLOR_KEY] = {0,
	(int)(sandHeight*255),
	(int)(sandHeight*255)+4,
	(int)(grassHeight*255),
	(int)(grassHeight*255)+10,
	(int)(snowHeight*255),
	(int)(snowHeight*255)+10,
	255
};

static Doryen::Color keyColor[MAX_COLOR_KEY] = {
        Doryen::Color( 10, 10, 90 ),    // deep water
        Doryen::Color( 30, 30, 170 ), // water-sand transition
        Doryen::Color( 114, 150, 71 ),// sand
        Doryen::Color( 80, 120, 10 ),// sand-grass transition
        Doryen::Color( 17, 109, 7 ), // grass
        Doryen::Color( 120, 220, 120 ), // grass-snow transisiton
        Doryen::Color( 208, 208, 239 ), // snow
        Doryen::Color( 255, 255, 255 )
};

void update(float elapsed, const Mouse& mouse)
{
	if (mouse.isPressedLeftButton())
		rippleManager->startRipple(mouse.getPositionCellX() * 2, mouse.getPositionCellY() * 2);
	rippleManager->updateRipples(elapsed);
}


void render(Console& console) {
	// copy ground into ground2. damn libtcod should have that...
	for (int x = 0; x < CON_W * 2; x++)
	{
		for (int y = 0; y < CON_H * 2; y++)
		{
			ground2->setPixel(x, y, ground->getPixel(x, y));
		}
	}
	rippleManager->renderRipples(ground, ground2);
	ground2->blit2x(console, 0, 0);

	console.setDefaultForeground(Palette::GRAY_WARN_1);
	console.write(3, 49, "Click in water to trigger ripples");
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

int main (int argc, char *argv[])
{
	// initialize the game window
	Doryen::Console console = Doryen::Console();
	console.initRoot(CON_W, CON_H, "Water ripples v 0.1.0", false);
	console.showCursor(true);
	console.setFramePerSeconds(25);

	bool endCredits = false;

	// create a 2d noise
	Doryen::Heightmap hm(CON_W * 2, CON_H * 2);
	hm.addFbm(&noise2d, 3.0f, 3.0f, 0, 0, 7.0f, 1.0f, 0.5f);
	hm.normalize();

	// apply a color map to create a ground image
	generateSmoothColorMap(mapGradient, MAX_COLOR_KEY, keyColor, keyIndex);
	ground = new Doryen::Image(CON_W * 2, CON_H * 2);
	ground2 = new Doryen::Image(CON_W * 2, CON_H * 2);
	// create a Doryen::Map defining water zones. Walkable = water
	Doryen::Map waterMap(CON_W * 2, CON_H * 2);

	for (int x = 0; x < CON_W * 2; x++)
	{
		for (int y = 0; y < CON_H * 2; y++)
		{
			float h = hm.getValue(x, y);
			bool isWater = h < sandHeight;
			waterMap.setProperties(x, y, isWater, isWater);
			int ih = (int)(h * 256);
			ih = CLAMP(0, 255, ih);
			ground->setPixel(x, y, mapGradient[ih]);
		}
	}
	rippleManager = new RippleManager(&waterMap);

    while ( console.isRunning( ))
	{
		const Key k = console.getKeyPressed();
		const Mouse mouse = console.getMouseEvent();

		if (k.isLeftAltPressed() and k.getKeyCode() == KeyCode::ENTER)
		{
			// switch fullscreen
			Doryen::Console::setWindowInFullscreen();
		}
		// update the game
		update(console.getLastFrameLength(), mouse);

		// render the game screen
		render(console);

		// flush updates to screen
		console.draw();
	}
	return 0;	
}

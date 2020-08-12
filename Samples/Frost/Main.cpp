#include "Doryen/Doryen.hpp"

#include "FrostManager.hpp"
#include "Frost.hpp"

using namespace Doryen;

int keys[4] = { 0, 60, 200, 255 };

Doryen::Color keyCols[4] = { Palette::GRAY_WARN_90, Palette::PRIMARY_DARK,
							 Palette::PRIMARY_VIVID, Palette::PRIMARY_LIGHTER };


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


int main()
{
	Console console = Console();

	console.initRoot(80, 50, "Frost Test", false);
	console.setFramePerSeconds(25);

	FrostManager frostManager = FrostManager(160, 100, console);

	generateSmoothColorMap(frostManager.getFrostColor(), sizeof(keys) / sizeof(int), keyCols, keys);

	while (console.isRunning())
	{
		frostManager.render();

		console.draw();

		if (console.getKeyPressed().getKeyCode() == KeyCode::BACKSPACE) frostManager.clear();

		const Mouse mouse = console.getMouseEvent();

		if (mouse.isPressedLeftButton())
		{
			frostManager.addFrost(mouse.getPositionCellX() * 2, mouse.getPositionCellY() * 2);
		}

		frostManager.update(console.getLastFrameLength());
	}

    return 0;
}
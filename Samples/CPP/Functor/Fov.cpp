// Joan Andrés (@Andres6936) Github.

#include <iostream>
#include <cstring>

#include "Fov.hpp"

using namespace Doryen;

Functor::FOV::FOV(std::string _name, Console& _console) : ISample(_name, _console)
{
	prepareInstanceOfMap();
}

void Functor::FOV::prepareInstanceOfMap()
{
	const std::uint16_t width = sample.getWidth();
	const std::uint16_t height = sample.getHeight();

	map = { width, height };

	// See documentation of the variable dungeon for see how
	// get the size of dungeon {width and height}.
	if (map.getWidth() not_eq std::strlen(dungeon.at(0)) or
		map.getHeight() not_eq dungeon.size())
	{
		std::cerr << "The width or height of map not correspond with "
				  << "the width or height of console.\n";
		return;
	}

	for (int y = 0; y < sample.getHeight(); y++)
	{
		for (int x = 0; x < sample.getWidth(); x++)
		{
			if (dungeon[y][x] == ' ')
			{
				map.setProperties(x, y, true, true);// ground
			}
			else if (dungeon[y][x] == '=')
			{ map.setProperties(x, y, true, false); } // window
		}
	}
}

void Functor::FOV::drawTextHelp()
{
	sample.setDefaultForeground(Doryen::Palette::GRAY_WARN_1);
	sample.print(1, 0, "IJKL : move around");
	sample.print(1, 1, format("T : torch fx {}", torch ? "ON " : "OFF"));
	sample.print(1, 2, format("W : light walls {}", light_walls ? "ON " : "OFF"));
	sample.print(1, 3, format("+-: algo {}", algo_names[algonum]));
}

void Functor::FOV::drawPlayer()
{
	sample.setDefaultForeground(Doryen::Palette::GRAY_WARN_1);
	sample.putChar(playerX, playerY, '@', BackgroundFlag::SET);
}

void Functor::FOV::drawDungeon()
{
	// torch position & intensity variation
	float dx = 0.0f, dy = 0.0f, di = 0.0f;

	if (torch)
	{
		// slightly change the perlin noise parameter
		torchx += 0.2f;
		// randomize the light position between -1.5 and 1.5
		float tdx = torchx + 20.0f;
		dx = noise.get(&tdx) * 1.5f;
		tdx += 30.0f;
		dy = noise.get(&tdx) * 1.5f;
		// randomize the light intensity between -0.2 and 0.2
		di = 0.2f * noise.get(&torchx);
	}

	for (int y = 0; y < sample.getHeight(); y++)
	{
		for (int x = 0; x < sample.getWidth(); x++)
		{
			bool visible = map.isInFov(x, y);
			bool wall = dungeon[y][x] == '#';
			if (!visible)
			{
				sample.setCharBackground(x, y, wall ? Palette::PRIMARY_DARK : Palette::PRIMARY_VIVID,
						BackgroundFlag::SET);
			}
			else
			{
				Doryen::Color light;
				if (!torch)
				{
					if (wall)
					{
						light = lightWall;
					}
					else
					{
						light = lightGround;
					}
				}
				else
				{
					// torch flickering fx
					Doryen::Color base = (wall ? Palette::PRIMARY_DARK : Palette::PRIMARY_VIVID);
					light = (wall ? lightWall : lightGround);
					// cell distance to torch (squared)
					float r = (float)((x - playerX + dx) * (x - playerX + dx) +
									  (y - playerY + dy) * (y - playerY + dy));
					if (r < SQUARED_TORCH_RADIUS)
					{
						// l = 1.0 at player position, 0.0 at a radius of 10 cells
						float l = (SQUARED_TORCH_RADIUS - r) / SQUARED_TORCH_RADIUS + di;
						l = CLAMP(0.0f, 1.0f, l);
						// interpolate the color
						base = Doryen::Color::lerp(base, light, l);
					}
					light = base;
				}
				sample.setCharBackground(x, y, light, Doryen::BackgroundFlag::SET);
			}
		}
	}
}

void Functor::FOV::render(KeyCode key, const Mouse& mouse)
{
	static bool recomputeFov = true; // the player moved. must recompute fov

	static Doryen::Color darkWall(0, 0, 100);
	static Doryen::Color darkGround(50, 50, 150);

	static bool first = true;

	sample.clear();

	drawPlayer();
	drawTextHelp();

	if (first)
	{
		first = false;

		// we draw the foreground only the first time.
		// during the player movement, only the @ is redrawn.
		// the rest impacts only the background color
		// draw the help text & player @
		sample.clear();

		// draw windows
		for (int y = 0; y < sample.getHeight(); y++)
		{
			for (int x = 0; x < sample.getWidth(); x++)
			{
				if (dungeon[y][x] == '=')
				{
					sample.putChar(x, y, TCOD_CHAR_DHLINE, Doryen::BackgroundFlag::NONE);
				}
			}
		}
	}
	if (recomputeFov)
	{
		// calculate the field of view from the player position
		recomputeFov = false;

		if (torch)
		{
			map.computeFov(playerX, playerY, (int)(TORCH_RADIUS), light_walls,
					(TCOD_fov_algorithm_t)algonum);
		}
		else
		{
			map.computeFov(playerX, playerY, 0, light_walls, (TCOD_fov_algorithm_t)algonum);
		}
	}

	drawDungeon();

//    if ( key->c == 'I' || key->c == 'i' )
//    {
//        // player move north
//        if ( smap[ playerY - 1 ][ playerX ] == ' ' )
//        {
//			sampleConsole.putChar(playerX, playerY, ' ', Doryen::BackgroundFlag::NONE);
//			playerY--;
//			sampleConsole.putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);
//			recomputeFov = true;
//        }
//    }
//    else if ( key->c == 'K' || key->c == 'k' )
//    {
//        // player move south
//        if ( smap[ playerY + 1 ][ playerX ] == ' ' )
//        {
//			sampleConsole.putChar(playerX, playerY, ' ', Doryen::BackgroundFlag::NONE);
//			playerY++;
//			sampleConsole.putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);
//			recomputeFov = true;
//        }
//    }
//    else if ( key->c == 'J' || key->c == 'j' )
//    {
//        // player move west
//        if ( smap[ playerY ][ playerX - 1 ] == ' ' )
//        {
//			sampleConsole.putChar(playerX, playerY, ' ', Doryen::BackgroundFlag::NONE);
//			playerX--;
//			sampleConsole.putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);
//			recomputeFov = true;
//        }
//    }
//    else if ( key->c == 'L' || key->c == 'l' )
//    {
//        // player move east
//        if ( smap[ playerY ][ playerX + 1 ] == ' ' )
//        {
//			sampleConsole.putChar(playerX, playerY, ' ', Doryen::BackgroundFlag::NONE);
//			playerX++;
//			sampleConsole.putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);
//			recomputeFov = true;
//        }
//    }
//    else if ( key->c == 'T' || key->c == 't' )
//	{
//		// enable/disable the torch fx
//		torch = !torch;
//		sampleConsole.setDefaultForeground(Doryen::Color::white);
//		sampleConsole.print(1, 0, format("IJKL : move around\nT : torch fx {}\nW : light walls {}\n+-: algo {}",
//				torch ? "on " : "off", light_walls ? "on " : "off", algo_names[algonum]));
//		sampleConsole.setDefaultForeground(Doryen::Color::black);
//	}
//    else if ( key->c == 'W' || key->c == 'w' )
//	{
//		light_walls = !light_walls;
//		sampleConsole.setDefaultForeground(Doryen::Color::white);
//		sampleConsole.print(1, 0, format("IJKL : move around\nT : torch fx {}\nW : light walls {}\n+-: algo {}",
//				torch ? "on " : "off", light_walls ? "on " : "off", algo_names[algonum]));
//		sampleConsole.setDefaultForeground(Doryen::Color::black);
//		recomputeFov = true;
//	}

	if (key == KeyCode::KP_ADD || key == KeyCode::KP_SUB)
	{
		algonum += key == KeyCode::KP_ADD ? 1 : -1;
		algonum = CLAMP(0, NB_FOV_ALGORITHMS - 1, algonum);
		sample.setDefaultForeground(Doryen::Palette::GRAY_WARN_1);
		sample.print(1, 0, format("IJKL : move around\nT : torch fx {}\nW : light walls {}\n+-: algo {}",
				torch ? "on " : "off", light_walls ? "on " : "off", algo_names[algonum]));
		sample.setDefaultForeground(Doryen::Palette::GRAY_WARN_90);
		recomputeFov = true;
	}
}

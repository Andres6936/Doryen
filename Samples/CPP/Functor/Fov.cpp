// Joan Andrés (@Andres6936) Github.

#include "Fov.hpp"

using namespace Doryen;

Functor::FOV::FOV(std::string _name, const Console& _console) : ISample(_name), sample(_console)
{

}

void Functor::FOV::render(KeyCode key, const Mouse& mouse)
{
	static const char* smap[] = {
			"##############################################",
			"#######################      #################",
			"#####################    #     ###############",
			"######################  ###        ###########",
			"##################      #####             ####",
			"################       ########    ###### ####",
			"###############      #################### ####",
			"################    ######                  ##",
			"########   #######  ######   #     #     #  ##",
			"########   ######      ###                  ##",
			"########                                    ##",
			"####       ######      ###   #     #     #  ##",
			"#### ###   ########## ####                  ##",
			"#### ###   ##########   ###########=##########",
			"#### ##################   #####          #####",
			"#### ###             #### #####          #####",
			"####           #     ####                #####",
			"########       #     #### #####          #####",
			"########       #####      ####################",
			"##############################################",
	};

	constexpr float TORCH_RADIUS = 10.0f;

	constexpr float SQUARED_TORCH_RADIUS = (TORCH_RADIUS * TORCH_RADIUS);

	// Player position X.
	static int playerX = 20;
	// Player position Y.
	static int playerY = 10;

	static bool recomputeFov = true; // the player moved. must recompute fov
	static bool torch = false; // torch fx on ?
	static Doryen::Map* map = NULL;
	static Doryen::Color darkWall(0, 0, 100);
	static Doryen::Color lightWall(130, 110, 50);
	static Doryen::Color darkGround(50, 50, 150);
	static Doryen::Color lightGround(200, 180, 50);
	static TCODNoise* noise = NULL;
	static bool light_walls = true;
	static int algonum = 0;
	static const char* algo_names[] = { "BASIC      ", "DIAMOND    ", "SHADOW     ",
										"PERMISSIVE0", "PERMISSIVE1", "PERMISSIVE2", "PERMISSIVE3", "PERMISSIVE4",
										"PERMISSIVE5", "PERMISSIVE6", "PERMISSIVE7", "PERMISSIVE8", "RESTRICTIVE" };

	static float torchx = 0.0f; // torch light position in the perlin noise

	if (!map)
	{
		// initialize the map for the fov toolkit
		map = new Doryen::Map(sample.getWidth(), sample.getHeight());

		for (int y = 0; y < sample.getHeight(); y++)
		{
			for (int x = 0; x < sample.getWidth(); x++)
			{
				if (smap[y][x] == ' ')
				{
					map->setProperties(x, y, true, true);// ground
				}
				else if (smap[y][x] == '=')
				{ map->setProperties(x, y, true, false); } // window
			}
		}
		// 1d noise used for the torch flickering
		noise = new TCODNoise(1);
	}

	static bool first = true;

	if (first)
	{
		first = false;

		Doryen::Platform::setFps(30); // fps limited to 30
		// we draw the foreground only the first time.
		// during the player movement, only the @ is redrawn.
		// the rest impacts only the background color
		// draw the help text & player @
		sample.clear();
		sample.setDefaultForeground(Doryen::Color::white);
		sample.print(1, 0, format("IJKL : move around\nT : torch fx {}\nW : light walls {}\n+-: algo {}",
				torch ? "on " : "off", light_walls ? "on " : "off", algo_names[algonum]));
		sample.setDefaultForeground(Doryen::Color::black);
		sample.putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);
		// draw windows
		for (int y = 0; y < sample.getHeight(); y++)
		{
			for (int x = 0; x < sample.getWidth(); x++)
			{
				if (smap[y][x] == '=')
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
			map->computeFov(playerX, playerY, (int)(TORCH_RADIUS), light_walls,
					(TCOD_fov_algorithm_t)algonum);
		}
		else
		{
			map->computeFov(playerX, playerY, 0, light_walls, (TCOD_fov_algorithm_t)algonum);
		}
	}

	// torch position & intensity variation
	float dx = 0.0f, dy = 0.0f, di = 0.0f;

	if (torch)
	{
		// slightly change the perlin noise parameter
		torchx += 0.2f;
		// randomize the light position between -1.5 and 1.5
		float tdx = torchx + 20.0f;
		dx = noise->get(&tdx) * 1.5f;
		tdx += 30.0f;
		dy = noise->get(&tdx) * 1.5f;
		// randomize the light intensity between -0.2 and 0.2
		di = 0.2f * noise->get(&torchx);
	}
	// draw the dungeon
	for (int y = 0; y < sample.getHeight(); y++)
	{
		for (int x = 0; x < sample.getWidth(); x++)
		{
			bool visible = map->isInFov(x, y);
			bool wall = smap[y][x] == '#';
			if (!visible)
			{
				sample.setCharBackground(x, y, wall ? darkWall : darkGround, Doryen::BackgroundFlag::SET);
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
					Doryen::Color base = (wall ? darkWall : darkGround);
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
		sample.setDefaultForeground(Doryen::Color::white);
		sample.print(1, 0, format("IJKL : move around\nT : torch fx {}\nW : light walls {}\n+-: algo {}",
				torch ? "on " : "off", light_walls ? "on " : "off", algo_names[algonum]));
		sample.setDefaultForeground(Doryen::Color::black);
		recomputeFov = true;
	}
}

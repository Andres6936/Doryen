// Joan Andrés (@Andres6936) Github.

#include <iostream>
#include <cstring>

#include "Fov.hpp"

using namespace Doryen;

Functor::FOV::FOV(Console& _console) : ISample("Field of view", _console)
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
	sample.setForegroundColor(Doryen::Palette::GRAY_WARN_1);
	sample.writeString(1, 0, "IJKL : move around");
	sample.writeString(1, 1, format("T : torch fx {}", torch ? "ON " : "OFF"));
	sample.writeString(1, 2, format("W : light walls {}", light_walls ? "ON " : "OFF"));
	sample.writeString(1, 3, format("+-: algo {}", typeFOVString()));
}

void Functor::FOV::drawPlayer()
{
	sample.setForegroundColor(Doryen::Palette::GRAY_WARN_1);
	sample.writeChar(playerX, playerY, '@', BlendModes::SET);
}

void Functor::FOV::drawDungeon()
{
	// torch position & intensity variation
	float dx = 0.0f, dy = 0.0f, di = 0.0f;

	if (torch)
	{
		// slightly change the perlin noise parameter
		torchx[0] += 0.2f;
		// randomize the light position between -1.5 and 1.5
		std::array<float, 1> tdx;

		tdx[0] = torchx[0] + 20.0f;

		dx = noise.get(tdx) * 1.5f;
		tdx[0] += 30.0f;
		dy = noise.get(tdx) * 1.5f;
		// randomize the light intensity between -0.2 and 0.2
		di = 0.2f * noise.get({ torchx });
	}

	for (int y = 0; y < sample.getHeight(); y++)
	{
		for (int x = 0; x < sample.getWidth(); x++)
		{
			bool visible = map.isVisibleFieldView(x, y);
			bool wall = dungeon[y][x] == '#';
			if (!visible)
			{
				sample.setCellBackgroundColor(x, y, wall ? Palette::PRIMARY_DARK : Palette::PRIMARY_VIVID,
						BlendModes::SET);
			}
			else
			{
				Doryen::Color light;
				if (!torch)
				{
					if (wall)
					{
						light = Palette::GOLD;
					}
					else
					{
						light = Palette::YELLOW;
					}
				}
				else
				{
					// torch flickering fx
					Doryen::Color base = (wall ? Palette::PRIMARY_DARK : Palette::PRIMARY_VIVID);
					light = (wall ? Palette::GOLD : Palette::YELLOW);
					// cell distance to torch (squared)
					float r = (float)((x - playerX + dx) * (x - playerX + dx) +
									  (y - playerY + dy) * (y - playerY + dy));
					if (r < SQUARED_TORCH_RADIUS)
					{
						// l = 1.0 at player position, 0.0 at a radius of 10 cells
						float l = (SQUARED_TORCH_RADIUS - r) / SQUARED_TORCH_RADIUS + di;
						l = std::clamp(l, 0.0f, 1.0f);
						// interpolate the color
						base = Doryen::Color::lerp(base, light, l);
					}
					light = base;
				}
				sample.setCellBackgroundColor(x, y, light, Doryen::BlendModes::SET);
			}

			// Draw the window
			if (dungeon[y][x] == '=') sample.writeChar(x, y, (char)205, BlendModes::SET);
		}
	}
}

void Functor::FOV::movePlayer(KeyCode key)
{

	if (key == KeyCode::I)
	{
		// player move north
		if (dungeon[playerY - 1][playerX] == ' ')
		{
			playerY--;
			recomputeFov = true;
		}
	}
	else if (key == KeyCode::K)
	{
		// player move south
		if (dungeon[playerY + 1][playerX] == ' ')
		{
			playerY++;
			recomputeFov = true;
		}
	}
	else if (key == KeyCode::J)
	{
		// player move west
		if (dungeon[playerY][playerX - 1] == ' ')
		{
			playerX--;
			recomputeFov = true;
		}
	}
	else if (key == KeyCode::L)
	{
		// player move east
		if (dungeon[playerY][playerX + 1] == ' ')
		{
			playerX++;
			recomputeFov = true;
		}
	}
	else if (key == KeyCode::T)
	{
		// enable/disable the torch fx
		torch = !torch;
	}
	else if (key == KeyCode::W)
	{
		light_walls = !light_walls;
		recomputeFov = true;
	}
}

void Functor::FOV::render(KeyCode key, const Mouse& mouse)
{
	static Doryen::Color darkWall(0, 0, 100);
	static Doryen::Color darkGround(50, 50, 150);

	sample.clear();

	drawPlayer();
	drawTextHelp();
	drawDungeon();

	if (recomputeFov)
	{
		// calculate the field of view from the player position
		recomputeFov = false;

		if (torch)
		{
			map.computeFov(playerX, playerY, (int)(TORCH_RADIUS), light_walls, algonum);
		}
		else
		{
			map.computeFov(playerX, playerY, 0, light_walls, algonum);
		}
	}

	movePlayer(key);

	if (key == KeyCode::KP_ADD || key == KeyCode::KP_SUB)
	{
		switchTypeFieldView();
		recomputeFov = true;
	}
}

void Functor::FOV::switchTypeFieldView()
{
	switch (algonum)
	{

	case TypeFOV::Basic:
		algonum = TypeFOV::Diamond;
		break;

	case TypeFOV::Diamond:
		algonum = TypeFOV::Shadow;
		break;

	case TypeFOV::Shadow:
		algonum = TypeFOV::Permissive;
		break;

	case TypeFOV::Permissive:
		algonum = TypeFOV::Basic;
		break;
	}
}

std::string Functor::FOV::typeFOVString()
{
	switch (algonum)
	{

	case TypeFOV::Basic:
		return "BASIC";

	case TypeFOV::Diamond:
		return "DIAMOND";

	case TypeFOV::Shadow:
		return "SHADOW";

	case TypeFOV::Permissive:
		return "PERMISSIVE";

	default:
		return "UNKNOWN";
	}
}

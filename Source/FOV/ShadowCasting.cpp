#include <algorithm>
#include <cmath>
#include "FOV/ShadowCasting.hpp"

void Doryen::ShadowRayCasting::operator()(Doryen::Map& map, int playerX, int playerY, int maxRadius, bool ligthWalls)
{
	for (int i = 0; i <= map.nbcells - 1; ++i)
	{
		map.cells[i].fov = false;
	}

	if (maxRadius == 0)
	{
		int maxRadiusX = std::max(map.width - playerX, playerX);
		int maxRadiusY = std::max(map.height - playerY, playerY);

		maxRadius = (int)std::sqrt(maxRadiusX * maxRadiusX + maxRadiusY * maxRadiusY) + 1;
	}

	int radiusDouble = maxRadius * maxRadius;

	static int mult[4][8] = {
			{ 1, 0, 0,  -1, -1, 0,  0,  1 },
			{ 0, 1, -1, 0,  0,  -1, 1,  0 },
			{ 0, 1, 1,  0,  0,  -1, -1, 0 },
			{ 1, 0, 0,  1,  -1, 0,  0,  -1 },
	};

	// Recursive Shadow Casting
	for (int oct = 0; oct < 8; ++oct)
	{
		castLight(map, playerX, playerY, 1, 1.0, 0.0, maxRadius,
				radiusDouble, mult[0][oct], mult[1][oct],
				mult[2][oct], mult[3][oct], 0, ligthWalls);
	}

	map.cells[playerX + map.width * playerY].fov = true;
}

void Doryen::ShadowRayCasting::castLight(Doryen::Map& map, int cx, int cy, int row, float start, float end, int radius,
		int r2, int xx, int xy, int yx, int yy, int id, bool light_walls)
{
	float new_start = 0.0f;
	if (start < end)
	{ return; }
	for (int j = row; j < radius + 1; j++)
	{
		int dx = -j - 1;
		int dy = -j;
		bool blocked = false;
		while (dx <= 0)
		{
			int X, Y;
			dx++;
			X = cx + dx * xx + dy * xy;
			Y = cy + dx * yx + dy * yy;
			if ((unsigned)X < (unsigned)map.width && (unsigned)Y < (unsigned)map.height)
			{
				float l_slope, r_slope;
				int offset;
				offset = X + Y * map.width;
				l_slope = (dx - 0.5f) / (dy + 0.5f);
				r_slope = (dx + 0.5f) / (dy - 0.5f);
				if (start < r_slope)
				{ continue; }
				else if (end > l_slope)
				{ break; }
				if (dx * dx + dy * dy <= r2
					&& (light_walls || map.cells[offset].transparent))
				{
					map.cells[offset].fov = 1;
				}
				if (blocked)
				{
					if (!map.cells[offset].transparent)
					{
						new_start = r_slope;
						continue;
					}
					else
					{
						blocked = false;
						start = new_start;
					}
				}
				else
				{
					if (!map.cells[offset].transparent && j < radius)
					{
						blocked = true;
						castLight(map, cx, cy, j + 1, start, l_slope, radius, r2, xx, xy, yx, yy, id + 1, light_walls);
						new_start = r_slope;
					}
				}
			}
		}
		if (blocked)
		{ break; }
	}
}

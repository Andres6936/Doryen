/*
* Umbra
* Copyright (c) 2009, 2010 Mingos, Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The names of Mingos or Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY MINGOS & JICE ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL MINGOS OR JICE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Dungeon.hpp"

using namespace Doryen;

// draw a vertical line
void Dungeon::vline(int x, int y1, int y2)
{
	int y = y1;
	int dy = (y1 > y2 ? -1 : 1);
	map.setProperties(x, y, true, true);
	if (y1 == y2) return;
	do
	{
		y += dy;
		map.setProperties(x, y, true, true);
	} while (y != y2);
}


// draw a vertical line up until we reach an empty space
void Dungeon::vline_up(int x, int y)
{
	while (y >= 0 && !map.isTransparent(x, y))
	{
		map.setProperties(x, y, true, true);
		y--;
	}
}

// draw a vertical line down until we reach an empty space
void Dungeon::vline_down(int x, int y)
{
	while (y < map.getHeight() && !map.isTransparent(x, y))
	{
		map.setProperties(x, y, true, true);
		y++;
	}
}

// draw a horizontal line
void Dungeon::hline(int x1, int y, int x2)
{
	int x = x1;
	int dx = (x1 > x2 ? -1 : 1);
	map.setProperties(x, y, true, true);
	if (x1 == x2) return;
	do
	{
		x += dx;
		map.setProperties(x, y, true, true);
	} while (x != x2);
}

// draw a horizontal line left until we reach an empty space
void Dungeon::hline_left(int x, int y)
{
	while (x >= 0 && !map.isTransparent(x, y))
	{
		map.setProperties(x, y, true, true);
		x--;
	}
}

// draw a horizontal line right until we reach an empty space
void Dungeon::hline_right(int x, int y)
{
	while (x < map.getWidth() && !map.isTransparent(x, y))
	{
		map.setProperties(x, y, true, true);
		x++;
	}
}

bool Dungeon::visitNode(Doryen::Algorithms::BinarySpacePartition* node, Map& map)
{
	if (node->isLeaf())
	{
		// calculate the room size
		int minx = node->x + 1;
		int maxx = node->x + node->w - 1;
		int miny = node->y + 1;
		int maxy = node->y + node->h - 1;
		if (!ROOM_WALLS)
		{
			if (minx > 1)
			{ minx--; }
			if (miny > 1)
			{ miny--; }
		}
		if (maxx == map.getWidth() - 1) maxx--;
		if (maxy == map.getHeight() - 1) maxy--;
		if (RANDOM_ROOM)
		{
			minx = Random::Number::nextInteger(minx, maxx - MIN_ROOM_SIZE + 1 < minx ? maxx : maxx - MIN_ROOM_SIZE + 1);
			miny = Random::Number::nextInteger(miny, maxy - MIN_ROOM_SIZE + 1 < miny ? maxy : maxy - MIN_ROOM_SIZE + 1);
			maxx = Random::Number::nextInteger(minx + MIN_ROOM_SIZE - 1 > maxx ? minx : minx + MIN_ROOM_SIZE - 1, maxx);
			maxy = Random::Number::nextInteger(miny + MIN_ROOM_SIZE - 1 > maxy ? miny : miny + MIN_ROOM_SIZE - 1 > maxy,
					maxy);
		}
		// keep walls on the map borders
		minx = MAX(1, minx);
		miny = MAX(1, miny);
		maxx = MIN(map.getWidth() - 2, maxx);
		maxy = MIN(map.getHeight() - 2, maxy);
		// resize the node to fit the room
//printf("node %dx%d %dx%d => room %dx%d %dx%d\n",node->x,node->y,node->w,node->h,minx,miny,maxx-minx+1,maxy-miny+1);
		node->x = minx;
		node->y = miny;
		node->w = maxx - minx + 1;
		node->h = maxy - miny + 1;
		// dig the room
		for (int x = minx; x <= maxx; x++)
		{
			for (int y = miny; y <= maxy; y++)
			{
				map.setProperties(x, y, true, true);
			}
		}
	} else
	{
//printf("lvl %d %dx%d %dx%d\n",node->level, node->x,node->y,node->w,node->h);
		// resize the node to fit its sons
		Doryen::Algorithms::BinarySpacePartition* left = node->getLeft();
		Doryen::Algorithms::BinarySpacePartition* right = node->getRight();
		node->x = MIN(left->x, right->x);
		node->y = MIN(left->y, right->y);
		node->w = MAX(left->x + left->w, right->x + right->w) - node->x;
		node->h = MAX(left->y + left->h, right->y + right->h) - node->y;
		// create a corridor between the two lower nodes
		if (node->horizontal)
		{
			// vertical corridor
			if (left->x + left->w - 1 < right->x || right->x + right->w - 1 < left->x)
			{
				// no overlapping zone. we need a Z shaped corridor
				int x1 = Random::Number::nextInteger(left->x, left->x + left->w - 1);
				int x2 = Random::Number::nextInteger(right->x, right->x + right->w - 1);
				int y = Random::Number::nextInteger(left->y + left->h, right->y);
				vline_up(x1, y - 1);
				hline(x1, y, x2);
				vline_down(x2, y + 1);
			} else
			{
				// straight vertical corridor
				int minx = MAX(left->x, right->x);
				int maxx = MIN(left->x + left->w - 1, right->x + right->w - 1);
				int x = Random::Number::nextInteger(minx, maxx);
				vline_down(x, right->y);
				vline_up(x, right->y - 1);
			}
		} else {
			// horizontal corridor
			if ( left->y+left->h -1 < right->y || right->y+right->h-1 < left->y )
			{
				// no overlapping zone. we need a Z shaped corridor
				int y1 = Random::Number::nextInteger(left->y,
						left->y + left->h - 1 < left->y ? left->y : left->y + left->h - 1);
				int y2 = Random::Number::nextInteger(right->y,
						right->y + right->h - 1 < right->y ? right->y : right->y + right->h - 1);

				int x = Random::Number::nextInteger(left->x + left->w, right->x);

				hline_left(x - 1, y1);
				vline(x, y1, y2);
				hline_right(x + 1, y2);
			} else
			{
				// straight horizontal corridor
				int miny = MAX(left->y, right->y);
				int maxy = MIN(left->y + left->h - 1, right->y + right->h - 1);
				int y = Random::Number::nextInteger(miny, maxy);
				hline_left(right->x - 1, y);
				hline_right(right->x, y);
			}
		}
	}
	return true;
}

void Dungeon::generateDungeon()
{
	auto const bsp = new Algorithms::BinarySpacePartition(0, 0, map.getWidth(), map.getHeight());
	// create the BSP tree
	bsp->splitRecursive(BSP_DEPTH, MIN_ROOM_SIZE + (ROOM_WALLS ? 1 : 0), MIN_ROOM_SIZE + (ROOM_WALLS ? 1 : 0), 1.5f,
			1.5f);
	// carve rooms and corridors
	bsp->traverseInvertedLevelOrder(this, map);
}

Map& Dungeon::getReferenceToMap()
{
	return map;
}

Geometry::Point2D<std::uint32_t> Dungeon::getCoordinateWalkableMoreClosest() const
{
	for (std::uint32_t x = 0; x < map.getWidth(); ++x)
	{
		for (std::uint32_t y = 0; y < map.getHeight(); ++y)
		{
			if (map.isWalkable(x, y))
			{
				return { x, y };
			}
		}
	}

	throw std::out_of_range("Not is possible find a position walkable in the current map");
}

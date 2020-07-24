#ifndef LIBTCOD_BSPLISTENER_HPP
#define LIBTCOD_BSPLISTENER_HPP

#include "Doryen/Doryen.hpp"
#include "SampleRenderer.hpp"
#include "Doryen/Algorithms/Generation/Dungeon/BinarySpacePartition.hpp"

using namespace Doryen;

static int bspDepth = 8;

static int minRoomSize = 4;

static bool randomRoom = false; // a room fills a random part of the node or the maximum available space ?
static bool roomWalls = true; // if true, there is always a wall on north & west side of a room

typedef char map_t[SAMPLE_SCREEN_WIDTH][SAMPLE_SCREEN_HEIGHT];

// the class building the dungeon from the bsp nodes
//#include <stdio.h>
class BspListener : public Doryen::Algorithms::BinarySpacePartitionCallback
{

public :

	bool visitNode(Doryen::Algorithms::BinarySpacePartition* node, Map& map)
	{
		if (node->isLeaf())
		{
			// calculate the room size
			int minx = node->x + 1;
			int maxx = node->x + node->w - 1;
			int miny = node->y + 1;
			int maxy = node->y + node->h - 1;
			if (!roomWalls)
			{
                if ( minx > 1 )
                { minx--; }
                if ( miny > 1 )
                { miny--; }
            }
            if ( maxx == SAMPLE_SCREEN_WIDTH - 1 )
            { maxx--; }
            if ( maxy == SAMPLE_SCREEN_HEIGHT - 1 )
            { maxy--; }
            if ( randomRoom )
			{
				minx = Random::Number::nextInteger(minx, maxx - minRoomSize + 1);
				miny = Random::Number::nextInteger(miny, maxy - minRoomSize + 1);
				maxx = Random::Number::nextInteger(minx + minRoomSize - 1, maxx);
				maxy = Random::Number::nextInteger(miny + minRoomSize - 1, maxy);
			}
            // resize the node to fit the room
//printf("node %dx%d %dx%d => room %dx%d %dx%d\n",node->x,node->y,node->w,node->h,minx,miny,maxx-minx+1,maxy-miny+1);
            node->x = minx;
            node->y = miny;
            node->w = maxx - minx + 1;
            node->h = maxy - miny + 1;
            // dig the room
            for ( int x = minx; x <= maxx; x++ )
            {
                for ( int y = miny; y <= maxy; y++ )
                {
					map.setProperties(x, y, true, true);
                }
            }
        }
        else
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
					vline_up(map, x1, y - 1);
					hline(map, x1, y, x2);
					vline_down(map, x2, y + 1);
				}
                else
                {
                    // straight vertical corridor
                    int minx = MAX( left->x, right->x );
					int maxx = MIN(left->x + left->w - 1, right->x + right->w - 1);
					int x = Random::Number::nextInteger(minx, maxx);
                    vline_down( map, x, right->y );
                    vline_up( map, x, right->y - 1 );
                }
            }
            else
            {
                // horizontal corridor
                if ( left->y + left->h - 1 < right->y || right->y + right->h - 1 < left->y )
				{
					// no overlapping zone. we need a Z shaped corridor
					int y1 = Random::Number::nextInteger(left->y, left->y + left->h - 1);
					int y2 = Random::Number::nextInteger(right->y, right->y + right->h - 1);
					int x = Random::Number::nextInteger(left->x + left->w, right->x);
					hline_left(map, x - 1, y1);
					vline(map, x, y1, y2);
					hline_right(map, x + 1, y2);
				}
                else
                {
                    // straight horizontal corridor
                    int miny = MAX( left->y, right->y );
					int maxy = MIN(left->y + left->h - 1, right->y + right->h - 1);
					int y = Random::Number::nextInteger(miny, maxy);
                    hline_left( map, right->x - 1, y );
                    hline_right( map, right->x, y );
                }
            }
        }
        return true;
    }

    // ***************************
// bsp sample
// ***************************

// draw a vertical line
	void vline(Map& map, int x, int y1, int y2)
	{
		int y = y1;
		int dy = (y1 > y2 ? -1 : 1);

		map.setProperties(x, y, true, true);

		if (y1 == y2)
		{ return; }
		do
		{
			y += dy;
			map.setProperties(x, y, true, true);
		} while (y != y2);
	}


// draw a vertical line up until we reach an empty space
	void vline_up(Map& map, int x, int y)
	{
		while (y >= 0 && not map.isWalkable(x, y))
		{
			map.setProperties(x, y, true, true);
			y--;
		}
	}

// draw a vertical line down until we reach an empty space
	void vline_down(Map& map, int x, int y)
	{
		while (y < SAMPLE_SCREEN_HEIGHT && not map.isWalkable(x, y))
		{
			map.setProperties(x, y, true, true);
			y++;
		}
	}

// draw a horizontal line
	void hline(Map& map, int x1, int y, int x2)
	{
		int x = x1;
		int dx = (x1 > x2 ? -1 : 1);

		map.setProperties(x, y, true, true);

		if (x1 == x2)
		{ return; }
		do
		{
			x += dx;
			map.setProperties(x, y, true, true);
		} while (x != x2);
	}

// draw a horizontal line left until we reach an empty space
	void hline_left(Map& map, int x, int y)
	{
		while (x >= 0 && not map.isWalkable(x, y))
		{
			map.setProperties(x, y, true, true);
			x--;
		}
	}

// draw a horizontal line right until we reach an empty space
	void hline_right(Map& map, int x, int y)
	{
		while (x < SAMPLE_SCREEN_WIDTH && not map.isWalkable(x, y))
		{
			map.setProperties(x, y, true, true);
			x++;
		}
	}

};

#endif //LIBTCOD_BSPLISTENER_HPP

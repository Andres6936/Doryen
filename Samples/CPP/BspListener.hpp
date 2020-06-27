#ifndef LIBTCOD_BSPLISTENER_HPP
#define LIBTCOD_BSPLISTENER_HPP

#include "Doryen/libtcod.hpp"
#include "SampleRenderer.hpp"
#include "Doryen/Algorithms/Generation/Dungeon/BinarySpacePartition.hpp"

static int bspDepth = 8;

static int minRoomSize = 4;

static bool randomRoom = false; // a room fills a random part of the node or the maximum available space ?
static bool roomWalls = true; // if true, there is always a wall on north & west side of a room

typedef char map_t[SAMPLE_SCREEN_WIDTH][SAMPLE_SCREEN_HEIGHT];

// the class building the dungeon from the bsp nodes
//#include <stdio.h>
class BspListener : public Doryen::Algorithms::ITCODBspCallback
{
public :
    bool visitNode(Doryen::Algorithms::BinarySpacePartition* node, void* userData)
    {
        map_t* map = (map_t*)userData;
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
                minx = TCODRandom::getInstance( )->getInt( minx, maxx - minRoomSize + 1 );
                miny = TCODRandom::getInstance( )->getInt( miny, maxy - minRoomSize + 1 );
                maxx = TCODRandom::getInstance( )->getInt( minx + minRoomSize - 1, maxx );
                maxy = TCODRandom::getInstance( )->getInt( miny + minRoomSize - 1, maxy );
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
                    ( *map )[ x ][ y ] = ' ';
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
                    int x1 = TCODRandom::getInstance( )->getInt( left->x, left->x + left->w - 1 );
                    int x2 = TCODRandom::getInstance( )->getInt( right->x, right->x + right->w - 1 );
                    int y = TCODRandom::getInstance( )->getInt( left->y + left->h, right->y );
                    vline_up( map, x1, y - 1 );
                    hline( map, x1, y, x2 );
                    vline_down( map, x2, y + 1 );
                }
                else
                {
                    // straight vertical corridor
                    int minx = MAX( left->x, right->x );
                    int maxx = MIN( left->x + left->w - 1, right->x + right->w - 1 );
                    int x = TCODRandom::getInstance( )->getInt( minx, maxx );
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
                    int y1 = TCODRandom::getInstance( )->getInt( left->y, left->y + left->h - 1 );
                    int y2 = TCODRandom::getInstance( )->getInt( right->y, right->y + right->h - 1 );
                    int x = TCODRandom::getInstance( )->getInt( left->x + left->w, right->x );
                    hline_left( map, x - 1, y1 );
                    vline( map, x, y1, y2 );
                    hline_right( map, x + 1, y2 );
                }
                else
                {
                    // straight horizontal corridor
                    int miny = MAX( left->y, right->y );
                    int maxy = MIN( left->y + left->h - 1, right->y + right->h - 1 );
                    int y = TCODRandom::getInstance( )->getInt( miny, maxy );
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
    void vline( map_t *map, int x, int y1, int y2 )
    {
        int y = y1;
        int dy = ( y1 > y2 ? -1 : 1 );
        ( *map )[ x ][ y ] = ' ';
        if ( y1 == y2 )
        { return; }
        do
        {
            y += dy;
            ( *map )[ x ][ y ] = ' ';
        }
        while ( y != y2 );
    }


// draw a vertical line up until we reach an empty space
    void vline_up( map_t *map, int x, int y )
    {
        while ( y >= 0 && ( *map )[ x ][ y ] != ' ' )
        {
            ( *map )[ x ][ y ] = ' ';
            y--;
        }
    }

// draw a vertical line down until we reach an empty space
    void vline_down( map_t *map, int x, int y )
    {
        while ( y < SAMPLE_SCREEN_HEIGHT && ( *map )[ x ][ y ] != ' ' )
        {
            ( *map )[ x ][ y ] = ' ';
            y++;
        }
    }

// draw a horizontal line
    void hline( map_t *map, int x1, int y, int x2 )
    {
        int x = x1;
        int dx = ( x1 > x2 ? -1 : 1 );
        ( *map )[ x ][ y ] = ' ';
        if ( x1 == x2 )
        { return; }
        do
        {
            x += dx;
            ( *map )[ x ][ y ] = ' ';
        }
        while ( x != x2 );
    }

// draw a horizontal line left until we reach an empty space
    void hline_left( map_t *map, int x, int y )
    {
        while ( x >= 0 && ( *map )[ x ][ y ] != ' ' )
        {
            ( *map )[ x ][ y ] = ' ';
            x--;
        }
    }

// draw a horizontal line right until we reach an empty space
    void hline_right( map_t *map, int x, int y )
    {
        while ( x < SAMPLE_SCREEN_WIDTH && ( *map )[ x ][ y ] != ' ' )
        {
            ( *map )[ x ][ y ] = ' ';
            x++;
        }
    }

};

#endif //LIBTCOD_BSPLISTENER_HPP

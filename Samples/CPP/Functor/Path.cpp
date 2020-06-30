// Joan Andrés (@Andres6936) Github.

#include <iostream>
#include <cstring>

#include "Path.hpp"

using namespace Doryen;

Functor::Path::Path(std::string _name, std::reference_wrapper<Console> _console) : ISample(_name, _console)
{
	prepareInstanceOfMap();
}


void Functor::Path::prepareInstanceOfMap()
{
	const std::uint16_t width = sample.get().getWidth();
	const std::uint16_t height = sample.get().getHeight();

	map = {width, height};

	// See documentation of the variable dungeon for see how
	// get the size of dungeon {width and height}.
	if (map.getWidth() not_eq std::strlen(dungeon.at(0)) or
		map.getHeight() not_eq dungeon.size())
	{
		std::cerr << "The width or height of map not correspond with "
				  << "the width or height of console.\n";

		// Complete pre-condition
		mapHaveSizeCorrect = false;
		return;
	}

	for (int y = 0; y < map.getHeight(); ++y)
	{
		for (int x = 0; x < map.getWidth(); ++x)
		{
			if (dungeon[y][x] == ' ')
			{
				map.setProperties(x, y, true, true);
			}
			else if (dungeon[y][x] == '=')
			{
				map.setProperties(x, y, true, false);
			}
		}
	}

	mapHaveSizeCorrect = true;
}

void Functor::Path::render(KeyCode key, const Mouse& mouse)
{
	static Doryen::Algorithms::Pathfinding::AStar* AStar = nullptr;

	static bool usingAstar = true;

	//static float dijkstraDist = 0;
	//static TCODDijkstra *dijkstra = nullptr;

	static bool recalculatePath = false;
	static float busy;

	int mouseX = 0;
	int mouseY = 0;

	if (mapHaveSizeCorrect)
	{
		AStar = new Doryen::Algorithms::Pathfinding::AStar(map);
		//dijkstra = new TCODDijkstra( map );
	}

	if (recalculatePath)
	{
		if (usingAstar)
		{
			if (dungeon[destinationY][destinationX] == ' ')
			{
				AStar->compute(playerX, playerY, destinationX, destinationY);
			}
		}
		else
		{
//            dijkstraDist = 0.0f;
//            // compute the distance grid
//            dijkstra->compute( playerX, playerY );
//            // get the maximum distance (needed for ground shading only)
//            for ( int y = 0; y < SAMPLE_SCREEN_HEIGHT; y++ )
//            {
//                for ( int x = 0; x < SAMPLE_SCREEN_WIDTH; x++ )
//                {
//                    float d = dijkstra->getDistance( x, y );
//                    if ( d > dijkstraDist )
//                    { dijkstraDist = d; }
//                }
//            }
//            // compute the path
//            dijkstra->setPath( destinationX, destinationY );
		}
		recalculatePath = false;
		busy = 0.2f;
	}

	sample.get().clear();

	// draw the dungeon
	for (int y = 0; y < sample.get().getHeight(); y++)
	{
		for (int x = 0; x < sample.get().getWidth(); x++)
		{
			if (dungeon[y][x] == '#')
			{
				sample.get().setCharBackground(x, y, DARK_WALL, Doryen::BackgroundFlag::SET);
			}
			else if (dungeon[y][x] == '=')
			{
				sample.get().putChar(x, y, TCOD_CHAR_DHLINE, Doryen::BackgroundFlag::NONE);
			}
			else
			{
				sample.get().setCharBackground(x, y, DARK_GROUND, Doryen::BackgroundFlag::SET);
			}
		}
	}

	sample.get().setDefaultForeground(Doryen::Color::white);

	sample.get().putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
	sample.get().putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);

	sample.get().print(1, 1, "IJKL / mouse :\nmove destination\nTAB : A*/dijkstra");
	sample.get().print(1, 4, "Using : A*");

	// draw the path
	if (usingAstar && AStar->findPath())
	{
		Doryen::Geometry::Point2D point;

		for (int i = 0; i < AStar->size(); i++)
		{
			try
			{
				point = AStar->getPoint2DAt(i);

				sample.get().setCharBackground(point.x, point.y, LIGHT_GROUND, Doryen::BackgroundFlag::SET);
			}
			catch (Doryen::Exceptions::IllegalMethodCall& e)
			{
				std::cout << e.showError();
			}
		}
	}
	else
	{
//        for ( int y = 0; y < SAMPLE_SCREEN_HEIGHT; y++ )
//        {
//            for ( int x = 0; x < SAMPLE_SCREEN_WIDTH; x++ )
//            {
//                bool wall = smap[ y ][ x ] == '#';
//                if ( !wall )
//                {
//                    float d = dijkstra->getDistance( x, y );
//                    sample.get().setCharBackground( x, y, Doryen::Color::lerp( lightGround, darkGround,
//                                                                                0.9f * d / dijkstraDist ),
//                                                     TCOD_BKGND_SET );
//                }
//            }
//        }
//
//        for ( int i = 0; i < dijkstra->size( ); i++ )
//        {
//            int x, y;
//            dijkstra->get( i, &x, &y );
//            sample.get().setCharBackground( x, y, lightGround, TCOD_BKGND_SET );
//        }
	}

	// move the creature
	busy -= Doryen::Platform::getLastFrameLength();

	if (busy <= 0.0f)
	{
		busy = 0.2f;
		if (usingAstar && AStar->findPath())
		{
			if (!AStar->isEmpty())
			{
				sample.get().putChar(playerX, playerY, ' ', Doryen::BackgroundFlag::NONE);

				try
				{
					Doryen::Geometry::Point2D point = AStar->walk();

					playerX = point.x;
					playerY = point.y;

					sample.get().putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);
				}
				catch (Doryen::Exceptions::IllegalMethodCall& e)
				{
					std::cout << e.showError();
				}
			}
		}
		else
		{
//            if ( !dijkstra->isEmpty( ))
//            {
//                sample.get().putChar( playerX, playerY, ' ', TCOD_BKGND_NONE );
//                dijkstra->walk( &playerX, &playerY );
//                sample.get().putChar( playerX, playerY, '@', TCOD_BKGND_NONE );
//                recalculatePath = true;
//            }
		}
	}

//    if (( key->c == 'I' || key->c == 'i' ) && destinationY > 0 )
//    {
//        // destination move north
//        destinationY--;
//		recalculatePath = true;
//		sample.get().putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
//    else if (( key->c == 'K' || key->c == 'k' ) && destinationY < SAMPLE_SCREEN_HEIGHT - 1 )
//    {
//        // destination move south
//        destinationY++;
//		recalculatePath = true;
//		sample.get().putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
//    else if (( key->c == 'J' || key->c == 'j' ) && destinationX > 0 )
//    {
//        // destination move west
//        destinationX--;
//		recalculatePath = true;
//		sample.get().putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
//    else if (( key->c == 'L' || key->c == 'l' ) && destinationX < SAMPLE_SCREEN_WIDTH - 1 )
//    {
//        // destination move east
//        destinationX++;
//		recalculatePath = true;
//		sample.get().putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
	if (key == KeyCode::TAB)
	{
		usingAstar = !usingAstar;

		if (usingAstar)
		{
			sample.get().print(1, 4, "Using : A*      ");
		}
		else
		{
			sample.get().print(1, 4, "Using : Dijkstra");
		}

		recalculatePath = true;
	}

	// sample screen position
	// TODO: Delete this dependency of variables
	constexpr short SAMPLE_SCREEN_X = 20;
	constexpr short SAMPLE_SCREEN_Y = 10;

	mouseX = mouse.getPositionCellX() - SAMPLE_SCREEN_X;
	mouseY = mouse.getPositionCellY() - SAMPLE_SCREEN_Y;

	if (mouseX >= 0 && mouseX < sample.get().getWidth() && mouseY >= 0 && mouseY < sample.get().getHeight() &&
		(destinationX != mouseX || destinationY != mouseY))
	{
		destinationX = mouseX;
		destinationY = mouseY;

		recalculatePath = true;
		sample.get().putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
	}
}

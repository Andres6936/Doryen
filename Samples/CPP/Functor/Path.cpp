// Joan Andrés (@Andres6936) Github.

#include <iostream>
#include <cstring>

#include "Path.hpp"

using namespace Doryen;

Functor::Path::Path(Console& _console) : ISample("Path finding", _console)
{
	prepareInstanceOfMap();
}

void Functor::Path::prepareInstanceOfMap()
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

	// TODO: Initialize the map Dijkstra
	AStar = Algorithms::Pathfinding::AStar(map);
}

void Functor::Path::drawDungeon()
{
	for (int y = 0; y < dungeon.size(); y++)
	{
		for (int x = 0; x < std::strlen(dungeon.at(0)); x++)
		{
			if (dungeon[y][x] == '#')
			{
				sample.setCharBackground(x, y, Palette::PRIMARY_DARK, BlendModes::SET);
			}
			else if (dungeon[y][x] == '=')
			{
				// The character 205 represent a double wall
				sample.writeChar(x, y, 205, BlendModes::NONE);
			}
			else
			{
				sample.setCharBackground(x, y, Palette::PRIMARY_VIVID, BlendModes::SET);
			}
		}
	}
}

void Functor::Path::render(KeyCode key, const Mouse& mouse)
{
	//static float dijkstraDist = 0;
	//static TCODDijkstra *dijkstra = nullptr;

	static bool recalculatePath = true;
	static float busy;

	if (recalculatePath)
	{
		if (usingAStar)
		{
			if (dungeon[destinationY][destinationX] == ' ')
			{
				AStar.compute(playerX, playerY, destinationX, destinationY);
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

	sample.clear();

	drawDungeon();
	drawDestinationPoint();
	drawHelpMessage();
	drawPlayer();

	// draw the path
	if (usingAStar and AStar.findPath())
	{
		Geometry::Point2D point;

		for (int i = 0; i < AStar.size(); i++)
		{
			try
			{
				point = AStar.getPoint2DAt(i);

				sample.setCharBackground(point.x, point.y, Palette::YELLOW, Doryen::BlendModes::SET);
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
//                    sample.setCharBackground( x, y, Doryen::Color::lerp( lightGround, darkGround,
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
//            sample.setCharBackground( x, y, lightGround, TCOD_BKGND_SET );
//        }
	}

	// move the creature
	busy -= sample.getLastFrameLength();

	if (busy <= 0.0f)
	{
		busy = 0.2f;
		if (usingAStar and AStar.findPath())
		{
			if (!AStar.isEmpty())
			{
				sample.writeChar(playerX, playerY, ' ', Doryen::BlendModes::NONE);

				try
				{
					Doryen::Geometry::Point2D point = AStar.walk();

					playerX = point.x;
					playerY = point.y;

					sample.writeChar(playerX, playerY, '@', Doryen::BlendModes::NONE);
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
//                sample.putChar( playerX, playerY, ' ', TCOD_BKGND_NONE );
//                dijkstra->walk( &playerX, &playerY );
//                sample.putChar( playerX, playerY, '@', TCOD_BKGND_NONE );
//                recalculatePath = true;
//            }
		}
	}

	recalculatePath = moveDestination(key);
	recalculatePath = changeAlgorithm(key);

	// sample screen position
	// TODO: Delete this dependency of variables
	constexpr short SAMPLE_SCREEN_X = 20;
	constexpr short SAMPLE_SCREEN_Y = 10;

	if (mouse.isPressedLeftButton())
	{
		const int MOUSE_X = mouse.getPositionCellX() - SAMPLE_SCREEN_X;
		const int MOUSE_Y = mouse.getPositionCellY() - SAMPLE_SCREEN_Y;

		if (MOUSE_X >= 0 and MOUSE_X < sample.getWidth() and
			MOUSE_Y >= 0 and MOUSE_Y < sample.getHeight())
		{
			destinationX = MOUSE_X;
			destinationY = MOUSE_Y;

			recalculatePath = true;
			sample.writeChar(destinationX, destinationY, '+', Doryen::BlendModes::NONE);
		}
	}
}

bool Functor::Path::moveDestination(KeyCode key)
{
	if (key == KeyCode::W and destinationY > 0)
	{
		// destination move north
		destinationY--;
		return true;
	}
	else if (key == KeyCode::S and destinationY < sample.getHeight() - 1)
	{
		// destination move south
		destinationY++;
		return true;
	}
	else if (key == KeyCode::A and destinationX > 0)
	{
		// destination move west
		destinationX--;
		return true;
	}
	else if (key == KeyCode::D and destinationX < sample.getWidth() - 1)
	{
		// destination move east
		destinationX++;
		return true;
	}

	return false;
}

void Functor::Path::drawDestinationPoint()
{
	sample.writeChar(destinationX, destinationY, '+', Doryen::BlendModes::NONE);
}

void Functor::Path::drawHelpMessage()
{
	sample.setDefaultForeground(Palette::GRAY_WARN_1);

	sample.write(1, 1, "WASD / Mouse :");
	sample.write(1, 2, "Move Destination");
	sample.write(1, 3, "TAB : A*/dijkstra");

	sample.write(1, 4, format("Using : {}", usingAStar ? "A*" : "Dijkstra"));
}

bool Functor::Path::changeAlgorithm(KeyCode key)
{
	if (key == KeyCode::TAB)
	{
		// Toggle variable
		usingAStar = not usingAStar;

		return true;
	}

	return false;
}

void Functor::Path::drawPlayer()
{
	sample.writeChar(playerX, playerY, '@', Doryen::BlendModes::NONE);
}

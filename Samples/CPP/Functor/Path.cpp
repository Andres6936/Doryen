// Joan Andrés (@Andres6936) Github.

#include <iostream>
#include "Path.hpp"

using namespace Doryen;

Functor::Path::Path(std::string _name, std::reference_wrapper<Console> _console) : ISample(_name), sample(_console)
{

}

void Functor::Path::render(KeyCode key, const Mouse& mouse)
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

	static int playerX = 20;
	static int playerY = 10;

	static int destinationX = 33;
	static int destinationY = 4;

	static Doryen::Map* map = nullptr;
	static Doryen::Color darkWall(0, 0, 100);
	static Doryen::Color darkGround(50, 50, 150);
	static Doryen::Color lightGround(200, 180, 50);

	static Doryen::Algorithms::Pathfinding::AStar* AStar = nullptr;

	static bool usingAstar = true;

	//static float dijkstraDist = 0;
	//static TCODDijkstra *dijkstra = nullptr;

	static bool recalculatePath = false;
	static float busy;

	int mouseX = 0;
	int mouseY = 0;

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
					map->setProperties(x, y, true, true); // ground
				}
				else if (smap[y][x] == '=')
				{
					map->setProperties(x, y, true, false); // window
				}
			}
		}

		AStar = new Doryen::Algorithms::Pathfinding::AStar(*map);
		//dijkstra = new TCODDijkstra( map );
	}

	if (recalculatePath)
	{
		if (usingAstar)
		{
			if (smap[destinationY][destinationX] == ' ')
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

	sample.clear();

	// draw the dungeon
	for (int y = 0; y < sample.getHeight(); y++)
	{
		for (int x = 0; x < sample.getWidth(); x++)
		{
			if (smap[y][x] == '#')
			{
				sample.setCharBackground(x, y, darkWall, Doryen::BackgroundFlag::SET);
			}
			else if (smap[y][x] == '=')
			{
				sample.putChar(x, y, TCOD_CHAR_DHLINE, Doryen::BackgroundFlag::NONE);
			}
			else
			{
				sample.setCharBackground(x, y, darkGround, Doryen::BackgroundFlag::SET);
			}
		}
	}

	sample.setDefaultForeground(Doryen::Color::white);

	sample.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
	sample.putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);

	sample.print(1, 1, "IJKL / mouse :\nmove destination\nTAB : A*/dijkstra");
	sample.print(1, 4, "Using : A*");

	// draw the path
	if (usingAstar && AStar->findPath())
	{
		Doryen::Geometry::Point2D point;

		for (int i = 0; i < AStar->size(); i++)
		{
			try
			{
				point = AStar->getPoint2DAt(i);

				sample.setCharBackground(point.x, point.y, lightGround, Doryen::BackgroundFlag::SET);
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
	busy -= Doryen::Platform::getLastFrameLength();

	if (busy <= 0.0f)
	{
		busy = 0.2f;
		if (usingAstar && AStar->findPath())
		{
			if (!AStar->isEmpty())
			{
				sample.putChar(playerX, playerY, ' ', Doryen::BackgroundFlag::NONE);

				try
				{
					Doryen::Geometry::Point2D point = AStar->walk();

					playerX = point.x;
					playerY = point.y;

					sample.putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);
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

//    if (( key->c == 'I' || key->c == 'i' ) && destinationY > 0 )
//    {
//        // destination move north
//        destinationY--;
//		recalculatePath = true;
//		sample.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
//    else if (( key->c == 'K' || key->c == 'k' ) && destinationY < SAMPLE_SCREEN_HEIGHT - 1 )
//    {
//        // destination move south
//        destinationY++;
//		recalculatePath = true;
//		sample.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
//    else if (( key->c == 'J' || key->c == 'j' ) && destinationX > 0 )
//    {
//        // destination move west
//        destinationX--;
//		recalculatePath = true;
//		sample.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
//    else if (( key->c == 'L' || key->c == 'l' ) && destinationX < SAMPLE_SCREEN_WIDTH - 1 )
//    {
//        // destination move east
//        destinationX++;
//		recalculatePath = true;
//		sample.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
	if (key == KeyCode::TAB)
	{
		usingAstar = !usingAstar;

		if (usingAstar)
		{
			sample.print(1, 4, "Using : A*      ");
		}
		else
		{
			sample.print(1, 4, "Using : Dijkstra");
		}

		recalculatePath = true;
	}

	// sample screen position
	// TODO: Delete this dependency of variables
	constexpr short SAMPLE_SCREEN_X = 20;
	constexpr short SAMPLE_SCREEN_Y = 10;

	mouseX = mouse.getPositionCellX() - SAMPLE_SCREEN_X;
	mouseY = mouse.getPositionCellY() - SAMPLE_SCREEN_Y;

	if (mouseX >= 0 && mouseX < sample.getWidth() && mouseY >= 0 && mouseY < sample.getHeight() &&
		(destinationX != mouseX || destinationY != mouseY))
	{
		destinationX = mouseX;
		destinationY = mouseY;

		recalculatePath = true;
		sample.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
	}
}

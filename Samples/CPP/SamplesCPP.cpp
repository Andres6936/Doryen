/*
 * libtcod CPP samples
 * This code demonstrates various usages of libtcod modules
 * It's in the public domain.
 */

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <functional>

#include "SampleRenderer.hpp"
#include "LineListener.hpp"
#include "BspListener.hpp"

#include "Doryen/Algorithms/Drawing/Bresenham.hpp"
#include "Doryen/Algorithms/Generation/Dungeon/BinarySpacePartition.hpp"

using namespace Doryen;

// a sample has a name and a rendering function
class Sample final
{

public:

	const std::string name;

	const std::function<void(bool first, KeyCode key, const Mouse& mouse)> render;

	Sample(const std::string& _name, const std::function<void(bool first, KeyCode key, const Mouse& mouse)>& _render) :
			name(_name), render(_render)
	{
	};
};

void render_colors(bool first, KeyCode key, const Mouse& mouse)
{

}

void render_offscreen(bool first, KeyCode key, const Mouse& mouse)
{

}

void render_lines(bool first, KeyCode key, const Mouse& mouse)
{

}

void render_noise(bool first, KeyCode key, const Mouse& mouse)
{

}

// ***************************
// fov sample
// ***************************
void render_fov(bool first, KeyCode key, const Mouse& mouse)
{

}

// ***************************
// image sample
// ***************************
void render_image(bool first, KeyCode key, const Mouse& mouse)
{
	static Doryen::Image* img = NULL, * circle = NULL;
	static Doryen::Color blue(0, 0, 255);
	static Doryen::Color green(0, 255, 0);
	if (img == NULL)
	{
		img = new Doryen::Image("Data/img/skull.png");
		img->setKeyColor(Doryen::Color::black);
		circle = new Doryen::Image("Data/img/circle.png");
	}
	if (first)
	{
        Doryen::Platform::setFps( 30 ); // fps limited to 30
    }
    sampleConsole.setDefaultBackground( Doryen::Color::black );
    sampleConsole.clear( );
    float x = SAMPLE_SCREEN_WIDTH / 2 + cosf( Doryen::Platform::getElapsedSeconds( )) * 10.0f;
    float y = ( float ) ( SAMPLE_SCREEN_HEIGHT / 2 );
    float scalex = 0.2f + 1.8f * ( 1.0f + cosf( Doryen::Platform::getElapsedSeconds( ) / 2 )) / 2.0f;
    float scaley = scalex;
    float angle = Doryen::Platform::getElapsedSeconds( );
    long elapsed = Doryen::Platform::getElapsedMilli( ) / 2000;
    if ( elapsed & 1 )
    {
        // split the color channels of circle.png
        // the red channel
		sampleConsole.setDefaultBackground(Doryen::Color::red);
		sampleConsole.rect(0, 3, 15, 15, false, Doryen::BackgroundFlag::SET);
		circle->blitRect(&sampleConsole, 0, 3, -1, -1, TCOD_BKGND_MULTIPLY);
        // the green channel
		sampleConsole.setDefaultBackground(green);
		sampleConsole.rect(15, 3, 15, 15, false, Doryen::BackgroundFlag::SET);
		circle->blitRect(&sampleConsole, 15, 3, -1, -1, TCOD_BKGND_MULTIPLY);
        // the blue channel
		sampleConsole.setDefaultBackground(blue);
		sampleConsole.rect(30, 3, 15, 15, false, Doryen::BackgroundFlag::SET);
		circle->blitRect(&sampleConsole, 30, 3, -1, -1, TCOD_BKGND_MULTIPLY);
	}
	else
	{
		// render circle.png with normal blitting
		circle->blitRect(&sampleConsole, 0, 3, -1, -1, TCOD_BKGND_SET);
		circle->blitRect(&sampleConsole, 15, 3, -1, -1, TCOD_BKGND_SET);
		circle->blitRect(&sampleConsole, 30, 3, -1, -1, TCOD_BKGND_SET);
	}

	img->blit(sampleConsole, { static_cast<int>(x), static_cast<int>(y) },
			BackgroundFlag::SET, scalex, scaley, angle);
}

// ***************************
// mouse sample
// ***************************/
void render_mouse(bool first, KeyCode key, const Mouse& mouse)
{
	static bool lbut = false, rbut = false, mbut = false;

	if (first)
	{
		Doryen::Platform::setFps(30); // fps limited to 30
		sampleConsole.setDefaultBackground(Doryen::Color::grey);
		sampleConsole.setDefaultForeground(Doryen::Color::lightYellow);
		TCODMouse::move(320, 200);
		TCODMouse::showCursor(true);
	}

	sampleConsole.clear();
	if (mouse.isPressedLeftButton())
	{ lbut = !lbut; }
	if (mouse.isPressedRightButton())
	{ rbut = !rbut; }
	if (mouse.isPressedMiddleButton())
	{ mbut = !mbut; }

	sampleConsole.print(1, 1, format("Mouse position : {4d}x{4d}\n",
			mouse.getX(), mouse.getY()));

	sampleConsole.print(1, 2,
			format("Mouse cell     : {4d}x{4d}", mouse.getPositionCellX(), mouse.getPositionCellY()));

	sampleConsole.print(1, 3,
			format("Mouse movement : {4d}x{4d}", mouse.getMovementRelativeX(), mouse.getMovementRelativeY()));

	sampleConsole.print(1, 4,
			format("Left button    : {} (toggle {})",
					mouse.isPressedLeftButton() ? " ON" : "OFF", lbut ? " ON" : "OFF"));

	sampleConsole.print(1, 5, format("Right button   : {} (toggle {})",
			mouse.isPressedRightButton() ? " ON" : "OFF", rbut ? " ON" : "OFF"));

	sampleConsole.print(1, 6, format("Middle button  : {} (toggle {})",
			mouse.isPressedMiddleButton() ? " ON" : "OFF", mbut ? " ON" : "OFF"));

//	sampleConsole.print(1, 7,format("Wheel          : {}",
//					mouse->wheel_up ? "UP" : (mouse->wheel_down ? "DOWN" : ""));

	sampleConsole.print(1, 10, "1 : Hide cursor\n2 : Show cursor");

//	if (key->c == '1')
//	{ TCODMouse::showCursor(false); }
//	else if (key->c == '2')
//	{ TCODMouse::showCursor(true); }
}

// ***************************
// path sample
// ***************************
void render_path(bool first, KeyCode key, const Mouse& mouse)
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

    static Doryen::Map *map = nullptr;
    static Doryen::Color darkWall( 0, 0, 100 );
    static Doryen::Color darkGround( 50, 50, 150 );
    static Doryen::Color lightGround( 200, 180, 50 );

    static Doryen::Algorithms::Pathfinding::AStar *AStar = nullptr;

    static bool usingAstar = true;

    //static float dijkstraDist = 0;
    //static TCODDijkstra *dijkstra = nullptr;

    static bool recalculatePath = false;
    static float busy;

    int mouseX = 0;
    int mouseY = 0;

    if ( !map )
    {
        // initialize the map for the fov toolkit
        map = new Doryen::Map( SAMPLE_SCREEN_WIDTH, SAMPLE_SCREEN_HEIGHT );

        for ( int y = 0; y < SAMPLE_SCREEN_HEIGHT; y++ )
        {
            for ( int x = 0; x < SAMPLE_SCREEN_WIDTH; x++ )
            {
                if ( smap[ y ][ x ] == ' ' )
                {
                    map->setProperties( x, y, true, true ); // ground
                }
                else if ( smap[ y ][ x ] == '=' )
                {
                    map->setProperties( x, y, true, false ); // window
                }
            }
        }

        AStar = new Doryen::Algorithms::Pathfinding::AStar( *map );
        //dijkstra = new TCODDijkstra( map );
    }

    if ( recalculatePath )
    {
        if ( usingAstar )
        {
            if ( smap[ destinationY ][ destinationX ] == ' ' )
            {
                AStar->compute( playerX, playerY, destinationX, destinationY );
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

    sampleConsole.clear( );

    // draw the dungeon
    for ( int y = 0; y < SAMPLE_SCREEN_HEIGHT; y++ )
    {
        for ( int x = 0; x < SAMPLE_SCREEN_WIDTH; x++ )
        {
            if ( smap[ y ][ x ] == '#' )
            {
				sampleConsole.setCharBackground(x, y, darkWall, Doryen::BackgroundFlag::SET);
            }
            else if ( smap[ y ][ x ] == '=' )
            {
				sampleConsole.putChar(x, y, TCOD_CHAR_DHLINE, Doryen::BackgroundFlag::NONE);
			}
			else
			{
				sampleConsole.setCharBackground(x, y, darkGround, Doryen::BackgroundFlag::SET);
			}
		}
	}

	sampleConsole.setDefaultForeground(Doryen::Color::white);

	sampleConsole.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
	sampleConsole.putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);

	sampleConsole.print(1, 1, "IJKL / mouse :\nmove destination\nTAB : A*/dijkstra");
	sampleConsole.print(1, 4, "Using : A*");

	// draw the path
	if (usingAstar && AStar->findPath())
	{
		Doryen::Geometry::Point2D point;

		for (int i = 0; i < AStar->size(); i++)
		{
			try
			{
				point = AStar->getPoint2DAt(i);

				sampleConsole.setCharBackground(point.x, point.y, lightGround, Doryen::BackgroundFlag::SET);
			}
			catch (Doryen::Exceptions::IllegalMethodCall& e)
            {
                std::cout << e.showError( );
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
//                    sampleConsole.setCharBackground( x, y, Doryen::Color::lerp( lightGround, darkGround,
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
//            sampleConsole.setCharBackground( x, y, lightGround, TCOD_BKGND_SET );
//        }
    }

    // move the creature
    busy -= Doryen::Platform::getLastFrameLength( );

    if ( busy <= 0.0f )
    {
        busy = 0.2f;
        if ( usingAstar && AStar->findPath( ))
        {
            if ( !AStar->isEmpty( ))
            {
				sampleConsole.putChar(playerX, playerY, ' ', Doryen::BackgroundFlag::NONE);

                try
                {
					Doryen::Geometry::Point2D point = AStar->walk();

                    playerX = point.x;
                    playerY = point.y;

					sampleConsole.putChar(playerX, playerY, '@', Doryen::BackgroundFlag::NONE);
                }
                catch ( Doryen::Exceptions::IllegalMethodCall &e )
                {
                    std::cout << e.showError( );
                }
            }
        }
        else
        {
//            if ( !dijkstra->isEmpty( ))
//            {
//                sampleConsole.putChar( playerX, playerY, ' ', TCOD_BKGND_NONE );
//                dijkstra->walk( &playerX, &playerY );
//                sampleConsole.putChar( playerX, playerY, '@', TCOD_BKGND_NONE );
//                recalculatePath = true;
//            }
		}
	}

//    if (( key->c == 'I' || key->c == 'i' ) && destinationY > 0 )
//    {
//        // destination move north
//        destinationY--;
//		recalculatePath = true;
//		sampleConsole.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
//    else if (( key->c == 'K' || key->c == 'k' ) && destinationY < SAMPLE_SCREEN_HEIGHT - 1 )
//    {
//        // destination move south
//        destinationY++;
//		recalculatePath = true;
//		sampleConsole.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
//    else if (( key->c == 'J' || key->c == 'j' ) && destinationX > 0 )
//    {
//        // destination move west
//        destinationX--;
//		recalculatePath = true;
//		sampleConsole.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
//    else if (( key->c == 'L' || key->c == 'l' ) && destinationX < SAMPLE_SCREEN_WIDTH - 1 )
//    {
//        // destination move east
//        destinationX++;
//		recalculatePath = true;
//		sampleConsole.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
//    }
	if (key == KeyCode::TAB)
	{
		usingAstar = !usingAstar;

		if (usingAstar)
		{
			sampleConsole.print(1, 4, "Using : A*      ");
		}
		else
		{
			sampleConsole.print(1, 4, "Using : Dijkstra");
		}

		recalculatePath = true;
	}

	mouseX = mouse.getPositionCellX() - SAMPLE_SCREEN_X;
	mouseY = mouse.getPositionCellY() - SAMPLE_SCREEN_Y;

	if (mouseX >= 0 && mouseX < SAMPLE_SCREEN_WIDTH && mouseY >= 0 && mouseY < SAMPLE_SCREEN_HEIGHT &&
		(destinationX != mouseX || destinationY != mouseY))
	{
		destinationX = mouseX;
		destinationY = mouseY;

		recalculatePath = true;
		sampleConsole.putChar(destinationX, destinationY, '+', Doryen::BackgroundFlag::NONE);
	}
}


void render_bsp(bool first, KeyCode key, const Mouse& mouse)
{
	static Algorithms::BinarySpacePartition* bsp = NULL;
	static bool generate = true;
	static bool refresh = false;
	static map_t map;
	static Color darkWall(0, 0, 100);
	static Color darkGround(50, 50, 150);
	static BspListener listener;

	if (generate || refresh)
	{
		// dungeon generation
		if (!bsp)
		{
			// create the bsp
			bsp = new Algorithms::BinarySpacePartition(
					0, 0, SAMPLE_SCREEN_WIDTH, SAMPLE_SCREEN_HEIGHT);
		}
        else
        {
            // restore the nodes size
            bsp->resize( 0, 0, SAMPLE_SCREEN_WIDTH, SAMPLE_SCREEN_HEIGHT );
        }
        memset( map, '#', sizeof( char ) * SAMPLE_SCREEN_WIDTH * SAMPLE_SCREEN_HEIGHT );
        if ( generate )
        {
            // build a new random bsp tree
            bsp->removeSons( );
            bsp->splitRecursive( NULL, bspDepth, minRoomSize + ( roomWalls ? 1 : 0 ),
                                 minRoomSize + ( roomWalls ? 1 : 0 ), 1.5f, 1.5f );
        }
        // create the dungeon from the bsp
        bsp->traverseInvertedLevelOrder( &listener, &map );
        generate = false;
        refresh = false;
    }
    sampleConsole.clear( );
    sampleConsole.setDefaultForeground(Doryen::Color::white);
	sampleConsole.print(1, 1,
			format("ENTER : rebuild bsp\nSPACE : rebuild dungeon\n+-: bsp depth {}\n*/: room size {}\n1 : random room size {}",
					bspDepth, minRoomSize,
					randomRoom ? "ON" : "OFF"));

    if ( randomRoom )
	{
		sampleConsole.print(1, 6, format("2 : room walls {}",
				roomWalls ? "ON" : "OFF"));
	}
    // render the level
    for ( int y = 0; y < SAMPLE_SCREEN_HEIGHT; y++ )
    {
        for ( int x = 0; x < SAMPLE_SCREEN_WIDTH; x++ )
        {
            bool wall = ( map[ x ][ y ] == '#' );
			sampleConsole.setCharBackground(x, y, wall ? darkWall : darkGround, Doryen::BackgroundFlag::SET);
        }
    }
	if (key == KeyCode::ENTER || key == KeyCode::PRINT_SCREEN)
	{
		generate = true;
	}
	else if (key == KeyCode::SPACE)
	{
		refresh = true;
	}
	else if (key == KeyCode::KP_ADD)
	{
		bspDepth++;
		generate = true;
	}
	else if (key == KeyCode::KP_SUB && bspDepth > 1)
	{
		bspDepth--;
		generate = true;
	}
	else if (key == KeyCode::KP_MUL)
	{
		minRoomSize++;
		generate = true;
	}
	else if (key == KeyCode::KP_DIV && minRoomSize > 2)
	{
		minRoomSize--;
		generate = true;
	}
	else if (key == KeyCode::K_1 or key == KeyCode::KP_1)
	{
		randomRoom = !randomRoom;
		if (!randomRoom)
		{ roomWalls = true; }
		refresh = true;
	}
	else if (key == KeyCode::K_2 or key == KeyCode::KP_2)
	{
		roomWalls = !roomWalls;
		refresh = true;
	}
}

/* ***************************
 * name generator sample
 * ***************************/
void render_name(bool first, KeyCode key, const Mouse& mouse)
{
	static int nbSets = 0;
	static int curSet = 0;
	static float delay = 0.0f;
	static TCODList<char*> names;
	static TCODList<char*> sets;
	int i;
	if (nbSets == 0)
	{
		TCODList<char*> files = Doryen::Platform::getDirectoryContent("Data/namegen", "*.cfg");
        // parse all the files
        for ( char **it = files.begin( ); it != files.end( ); it++ )
        {
            char tmp[256];
            sprintf( tmp, "Data/namegen/%s", *it );
            TCODNamegen::parse( tmp );
        }
        // get the sets list
        sets = TCODNamegen::getSets( );
        nbSets = sets.size( );
    }
    if ( first )
    {
        Doryen::Platform::setFps( 30 ); /* limited to 30 fps */
    }

    while ( names.size( ) >= 15 )
    {
        // remove the first element.
#ifndef TCOD_VISUAL_STUDIO
        char *nameToRemove = *( names.begin( ));
#endif
		names.remove(names.begin());
		// for some reason, this crashes on MSVC...
#ifndef TCOD_VISUAL_STUDIO
		free(nameToRemove);
#endif
	}

	sampleConsole.setDefaultBackground(Doryen::Color::lightBlue);
	sampleConsole.clear();
	sampleConsole.setDefaultForeground(Doryen::Color::white);
	sampleConsole.print(1, 1, format("{}\n\n+ : next generator\n- : prev generator", sets.get(curSet)));

	for (i = 0; i < names.size(); i++)
	{
		char* name = names.get(i);
		if (strlen(name) < SAMPLE_SCREEN_WIDTH)
		{
			sampleConsole.printEx(SAMPLE_SCREEN_WIDTH - 2, 2 + i, TCOD_BKGND_NONE, TCOD_RIGHT, name);
		}
	}

    delay += Doryen::Platform::getLastFrameLength( );
	if (delay >= 0.5f)
	{
		delay -= 0.5f;
		// add a new name to the list
		names.push(TCODNamegen::generate(sets.get(curSet), true));
	}
	if (key == KeyCode::KP_ADD)
	{
		curSet++;
		if (curSet == nbSets)
		{ curSet = 0; }
		names.push(strdup("======"));
	}
	else if (key == KeyCode::KP_SUB)
	{
		curSet--;
		if (curSet < 0)
		{ curSet = nbSets - 1; }
		names.push(strdup("======"));
	}
}

/* ***************************
 * SDL callback sample
 * ***************************/
#ifndef NO_SDL_SAMPLE


void render_sdl(bool first, KeyCode key, const Mouse& mouse)
{
	if (first)
	{
		Doryen::Platform::setFps(30); /* limited to 30 fps */
		// use noise sample as background. rendering is done in SampleRenderer
		sampleConsole.setDefaultBackground(Doryen::Color::lightBlue);
		sampleConsole.setDefaultForeground(Doryen::Color::white);
		sampleConsole.clear();
		sampleConsole.printRectEx(SAMPLE_SCREEN_WIDTH / 2, 3, SAMPLE_SCREEN_WIDTH, 0, TCOD_BKGND_NONE, TCOD_CENTER,
				"The SDL callback gives you access to the screen surface so that you can alter the pixels one by one using SDL API or any API on top of SDL. SDL is used here to blur the sample console.\n\nHit TAB to enable/disable the callback. While enabled, it will be active on other samples too.\n\nNote that the SDL callback only works with SDL renderer.");
	}
	if (key == KeyCode::TAB)
	{
		sdl_callback_enabled = !sdl_callback_enabled;
		if (sdl_callback_enabled)
		{
			Doryen::Platform::registerSDLRenderer(new SampleRenderer());
		}
		else
		{
			Doryen::Platform::registerSDLRenderer(NULL);
			// we want libtcod to redraw the sample console even if nothing has changed in it
            Doryen::Console::root->setDirty( SAMPLE_SCREEN_X, SAMPLE_SCREEN_Y, SAMPLE_SCREEN_WIDTH,
                                             SAMPLE_SCREEN_HEIGHT );
        }
    }
}

#endif

using namespace std::string_literals;

int main(int argc, char* argv[])
{
	std::array<Sample, 11> samples = {
			Sample{ "  True colors        "s, render_colors },
			Sample{ "  Offscreen console  "s, render_offscreen },
			Sample{ "  Line drawing       "s, render_lines },
			Sample{ "  Noise              "s, render_noise },
			Sample{ "  Field of view      "s, render_fov },
			Sample{ "  Path finding       "s, render_path },
			Sample{ "  Bsp toolkit        "s, render_bsp },
			Sample{ "  Image toolkit      "s, render_image },
			Sample{ "  Mouse support      "s, render_mouse },
			Sample{ "  Name generator     "s, render_name },
			Sample{ "  SDL callback       "s, render_sdl }
	};

	int curSample = 0; // index of the current sample
	bool first = true; // first time we render a sample

	const char* font = "Data/fonts/consolas10x10_gs_tc.png";
	int nbCharHoriz = 0, nbCharVertic = 0;
	int fullscreenWidth = 0;
	int fullscreenHeight = 0;
	TCOD_renderer_t renderer = TCOD_RENDERER_SDL;
	bool fullscreen = false;
    int fontFlags = TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_TCOD, fontNewFlags = 0;
    bool creditsEnd = false;
    int cur_renderer = 0;
    static const char *renderer_name[TCOD_NB_RENDERERS] = {
            "F1 GLSL   ", "F2 OPENGL ", "F3 SDL    "
    };

    // initialize the root console (open the game window)
    for ( int j = 1; j < argc; j++ )
    {
        if ( strcmp( argv[ j ], "-font" ) == 0 && j + 1 < argc )
        {
            j++;
            font = argv[ j ];
            fontFlags = 0;
        }
        else if ( strcmp( argv[ j ], "-font-nb-char" ) == 0 && j + 2 < argc )
        {
            j++;
            nbCharHoriz = atoi( argv[ j ] );
            j++;
            nbCharVertic = atoi( argv[ j ] );
            fontFlags = 0;
        }
        else if ( strcmp( argv[ j ], "-fullscreen-resolution" ) == 0 && j + 2 < argc )
        {
            j++;
            fullscreenWidth = atoi( argv[ j ] );
            j++;
            fullscreenHeight = atoi( argv[ j ] );
        }
        else if ( strcmp( argv[ j ], "-renderer" ) == 0 && j + 1 < argc )
        {
            j++;
            renderer = ( TCOD_renderer_t ) atoi( argv[ j ] );
        }
        else if ( strcmp( argv[ j ], "-fullscreen" ) == 0 )
        {
            fullscreen = true;
        }
        else if ( strcmp( argv[ j ], "-font-in-row" ) == 0 )
        {
            fontNewFlags |= TCOD_FONT_LAYOUT_ASCII_INROW;
            fontFlags = 0;
        }
        else if ( strcmp( argv[ j ], "-font-greyscale" ) == 0 )
        {
            fontNewFlags |= TCOD_FONT_TYPE_GREYSCALE;
            fontFlags = 0;
        }
        else if ( strcmp( argv[ j ], "-font-tcod" ) == 0 )
        {
            fontNewFlags |= TCOD_FONT_LAYOUT_TCOD;
            fontFlags = 0;
        }
        else if ( strcmp( argv[ j ], "-help" ) == 0 || strcmp( argv[ j ], "-?" ) == 0 )
        {
            printf( "options :\n" );
            printf( "-font <filename> : use a custom font\n" );
            printf( "-font-nb-char <nb_char_horiz> <nb_char_vertic> : number of characters in the font\n" );
            printf( "-font-in-row : the font layout is in row instead of columns\n" );
            printf( "-font-tcod : the font uses TCOD layout instead of ASCII\n" );
            printf( "-font-greyscale : antialiased font using greyscale bitmap\n" );
            printf( "-fullscreen : start in fullscreen\n" );
            printf( "-fullscreen-resolution <screen_width> <screen_height> : force fullscreen resolution\n" );
            printf( "-renderer <num> : set renderer. 0 : GLSL 1 : OPENGL 2 : SDL\n" );
            exit( 0 );
        }
		else
		{
			// ignore parameter
		}
	}

	if (fontFlags == 0)
	{ fontFlags = fontNewFlags; }


	if (fullscreenWidth > 0)
	{
		Doryen::Platform::forceFullscreenResolution(fullscreenWidth, fullscreenHeight);
	}

	Console console = Console();

	console.setCustomFont(font, fontFlags, nbCharHoriz, nbCharVertic);

	console.initRoot(80, 50, "libtcod C++ sample", fullscreen, renderer);

	while (console.isRunning())
	{
		if (!creditsEnd)
		{
			creditsEnd = Console::renderCredits(60, 43, false);
		}

		// print the list of samples
		for (int i = 0; i < samples.size(); i++)
		{
			if (i == curSample)
			{
				// set colors for currently selected sample
				console.setDefaultForeground(Doryen::Color::white);
				console.setDefaultBackground(Doryen::Color::lightBlue);
			}
			else
			{
				// set colors for other samples
				console.setDefaultForeground(Doryen::Color::grey);
				console.setDefaultBackground(Doryen::Color::black);
			}

			// print the sample name
			console.print(2, 46 - (samples.size() - i), samples[i].name);
		}

		// print the help message
		console.setDefaultForeground(Doryen::Color::grey);
		console.print(50, 46,
				format("last frame : {>2.0f} ms ({} fps)",
						console.getLastFrameLength() * 1000,
						console.getFramePerSeconds()));

		console.print(50, 47,
				format("elapsed : {>5d} ms {>4.2f}s",
						Doryen::Platform::getElapsedMilli(),
						Doryen::Platform::getElapsedSeconds()));

		console.print(2, 47,
				format("{c}{c} : select a sample",
						(char)TCOD_CHAR_ARROW_N,
						(char)TCOD_CHAR_ARROW_S));

		if (Console::isFullscreen())
		{
			console.print(2, 48, "ALT-ENTER : switch to windowed mode");
		}
		else
		{
			console.print(2, 48, "ALT-ENTER : switch to fullscreen mode");
		}

		const KeyCode _key = console.getKeyPressed().getKeyCode();
		const Mouse mouse = console.getMouseEvent();

		// render current sample
		samples[curSample].render(first, _key, mouse);
		first = false;

		// blit the sample console on the root console
		sampleConsole.blit({ 0, 0 }, console, { SAMPLE_SCREEN_X, SAMPLE_SCREEN_Y });

		// erase the renderer in debug mode (needed because the root console is not cleared each frame)
		console.print(1, 1, "        ");
#ifndef NO_SDL_SAMPLE
		if (sdl_callback_enabled)
		{
			// we want libtcod to redraw the sample console even if nothing has changed in it
			console.setDirty(SAMPLE_SCREEN_X, SAMPLE_SCREEN_Y, SAMPLE_SCREEN_WIDTH,
					SAMPLE_SCREEN_HEIGHT);
		}
#endif
		/* display renderer list and current renderer */
		cur_renderer = Doryen::Platform::getRenderer();

		console.setDefaultForeground(Doryen::Color::grey);
		console.setDefaultBackground(Doryen::Color::black);
		console.printEx(42, 46 - (TCOD_NB_RENDERERS + 1), TCOD_BKGND_SET, TCOD_LEFT, "Renderer :");

		for (int i = 0; i < TCOD_NB_RENDERERS; i++)
		{
			if (i == cur_renderer)
			{
				/* set colors for current renderer */
				console.setDefaultForeground(Doryen::Color::white);
				console.setDefaultBackground(Doryen::Color::lightBlue);
			}
			else
			{
				/* set colors for other renderer */
				console.setDefaultForeground(Doryen::Color::grey);
				console.setDefaultBackground(Doryen::Color::black);
			}
			console.printEx(42, 46 - (TCOD_NB_RENDERERS - i), TCOD_BKGND_SET, TCOD_LEFT,
					renderer_name[i]);
		}

		// update the game screen
		Doryen::Console::flush();

		if (_key == KeyCode::DOWN)
		{
			// down arrow : next sample
			curSample = (curSample + 1) % samples.size();
			first = true;
		}
		else if (_key == KeyCode::UP)
		{
			// up arrow : previous sample
			curSample--;
			if (curSample < 0)
			{ curSample = samples.size() - 1; }
			first = true;
		}
		else if (_key == KeyCode::ENTER and console.getKeyPressed().isLeftAltPressed())
		{
			// ALT-ENTER : switch fullscreen
			Doryen::Console::setWindowInFullscreen();
#ifdef TCOD_LINUX
		}
		else if (_key == KeyCode::PRINT_SCREEN)
		{
#else
			} else if ( key.vk == TCODK_PRINTSCREEN ) {
#endif
		}
		else if (_key == KeyCode::F1)
		{
			// switch renderers with F1,F2,F3
			Doryen::Platform::setRenderer(TCOD_RENDERER_GLSL);
		}
		else if (_key == KeyCode::F2)
		{
			Doryen::Platform::setRenderer(TCOD_RENDERER_OPENGL);
		}
		else if (_key == KeyCode::F3)
		{
			Doryen::Platform::setRenderer(TCOD_RENDERER_SDL);
		}
    }

    return 0;
}

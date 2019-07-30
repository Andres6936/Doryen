/*
* Copyright (c) 2010 Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "Main.hpp"

TCODNoise noise2d(2);
TCODNoise noise3d(3);
RippleManager *rippleManager;
TCODImage *ground, *ground2;

Doryen::Color mapGradient[256];
#define MAX_COLOR_KEY 8

// TCOD's land color map
float sandHeight=0.3f;
float grassHeight=0.5f;
float snowHeight=0.9f;

static int keyIndex[MAX_COLOR_KEY] = {0,
	(int)(sandHeight*255),
	(int)(sandHeight*255)+4,
	(int)(grassHeight*255),
	(int)(grassHeight*255)+10,
	(int)(snowHeight*255),
	(int)(snowHeight*255)+10,
	255
};

static Doryen::Color keyColor[MAX_COLOR_KEY] = {
        Doryen::Color( 10, 10, 90 ),    // deep water
        Doryen::Color( 30, 30, 170 ), // water-sand transition
        Doryen::Color( 114, 150, 71 ),// sand
        Doryen::Color( 80, 120, 10 ),// sand-grass transition
        Doryen::Color( 17, 109, 7 ), // grass
        Doryen::Color( 120, 220, 120 ), // grass-snow transisiton
        Doryen::Color( 208, 208, 239 ), // snow
        Doryen::Color( 255, 255, 255 )
};

void update(float elapsed, TCOD_key_t k, TCOD_mouse_t mouse) {
	if ( mouse.lbutton ) rippleManager->startRipple(mouse.cx*2,mouse.cy*2);
	rippleManager->updateRipples(elapsed);
}


void render() {
	// copy ground into ground2. damn libtcod should have that...
	for (int x=0; x < CON_W*2;x++) {
		for (int y=0; y < CON_H*2;y++) {
			ground2->putPixel(x,y,ground->getPixel(x,y));
		}
	}
	rippleManager->renderRipples(ground,ground2);
    ground2->blit2x( Doryen::Console::root, 0, 0 );
    Doryen::Console::root->setDefaultForeground( Doryen::Color::white );
    Doryen::Console::root->print( 3, 49, "Click in water to trigger ripples" );
}

int main (int argc, char *argv[]) {
	// initialize the game window
    Doryen::Console console = Doryen::Console( );
    console.initRoot( CON_W, CON_H, "Water ripples v"VERSION, false, TCOD_RENDERER_SDL );
    Doryen::Plataform::setFps( 25 );
	TCODMouse::showCursor(true);
	
	bool endCredits=false;
	
	// create a 2d noise
	TCODHeightMap hm(CON_W*2,CON_H*2);
	hm.addFbm(&noise2d,3.0f,3.0f,0,0,7.0f,1.0f,0.5f);
	hm.normalize();
	// apply a color map to create a ground image
    Doryen::Color::genMap( mapGradient, MAX_COLOR_KEY, keyColor, keyIndex );
	ground=new TCODImage(CON_W*2,CON_H*2);
	ground2=new TCODImage(CON_W*2,CON_H*2);
	// create a TCODMap defining water zones. Walkable = water
	TCODMap waterMap(CON_W*2,CON_H*2);

	for (int x=0; x < CON_W*2;x++) {
		for (int y=0; y < CON_H*2;y++) {
			float h=hm.getValue(x,y);
			bool isWater=h < sandHeight; 
			waterMap.setProperties(x,y,isWater,isWater);
			int ih=(int)(h*256);
			ih=CLAMP(0,255,ih);
			ground->putPixel(x,y,mapGradient[ih]);
		}
	}
	rippleManager = new RippleManager(&waterMap);

    while ( !Doryen::Console::isWindowClosed( ))
    {
		TCOD_key_t k;
		TCOD_mouse_t mouse;

        Doryen::Plataform::checkForEvent( TCOD_EVENT_KEY | TCOD_EVENT_MOUSE, &k, &mouse );

		if ( k.vk == TCODK_PRINTSCREEN ) {
			// screenshot
            if ( !k.pressed )
            { Doryen::Plataform::saveScreenshot( NULL ); }
            k.vk=TCODK_NONE;
		} else if ( k.lalt && (k.vk == TCODK_ENTER || k.vk == TCODK_KPENTER) ) {
			// switch fullscreen
            if ( !k.pressed )
            { Doryen::Console::setFullscreen( !Doryen::Console::isFullscreen( )); }
            k.vk=TCODK_NONE;
		}
		// update the game
        update( Doryen::Plataform::getLastFrameLength( ), k, mouse );

		// render the game screen
		render();
		// render libtcod credits
        if ( !endCredits )
        { endCredits = Doryen::Console::renderCredits( 4, 4, true ); }
        // flush updates to screen
        Doryen::Console::root->flush( );
	}
	return 0;	
}

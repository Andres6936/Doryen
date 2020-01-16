/*
* libtcod 1.5.1
* Copyright (c) 2008,2009,2010,2012 Jice & Mingos
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice or Mingos may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE AND MINGOS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE OR MINGOS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Platform/Platform.hpp"

void Doryen::Platform::setFps( int val )
{
    TCOD_sys_set_fps( val );
}

int Doryen::Platform::getFps( )
{
    return TCOD_sys_get_fps( );
}

float Doryen::Platform::getLastFrameLength( )
{
    return TCOD_sys_get_last_frame_length( );
}

void Doryen::Platform::sleepMilli( uint32 milliseconds )
{
    TCOD_sys_sleep_milli( milliseconds );
}

void Doryen::Platform::saveScreenshot( const char *filename )
{
    TCOD_sys_save_screenshot( filename );
}

uint32 Doryen::Platform::getElapsedMilli( )
{
	return TCOD_sys_elapsed_milli();
}

float Doryen::Platform::getElapsedSeconds( )
{
	return TCOD_sys_elapsed_seconds();
}

void Doryen::Platform::forceFullscreenResolution( int width, int height )
{
	TCOD_sys_force_fullscreen_resolution(width,height);
}

void Doryen::Platform::setRenderer( TCOD_renderer_t renderer )
{
	TCOD_sys_set_renderer(renderer);
}

TCOD_event_t Doryen::Platform::waitForEvent( int eventMask, TCOD_key_t *key, TCOD_mouse_t *mouse, bool flush )
{
	return TCOD_sys_wait_for_event(eventMask,key,mouse,flush);
}

TCOD_event_t Doryen::Platform::checkForEvent( int eventMask, TCOD_key_t *key, TCOD_mouse_t *mouse )
{
	return TCOD_sys_check_for_event(eventMask,key,mouse);
}

TCOD_renderer_t Doryen::Platform::getRenderer( )
{
	return TCOD_sys_get_renderer();
}

void Doryen::Platform::getCurrentResolution( int *w, int *h )
{
	TCOD_sys_get_current_resolution(w,h);
}

void Doryen::Platform::getFullscreenOffsets( int *offx, int *offy )
{
	TCOD_sys_get_fullscreen_offsets(offx,offy);
}

void Doryen::Platform::updateChar(int asciiCode, int fontx, int fonty, const Image* img, int x, int y)
{
	TCOD_sys_update_char(asciiCode, fontx, fonty, img->data, x, y);
}

void Doryen::Platform::getCharSize( int *w, int *h )
{
	TCOD_sys_get_char_size(w,h);
}

TCOD_list_t Doryen::Platform::getDirectoryContent( const char *path, const char *pattern )
{
	return TCOD_sys_get_directory_content(path,pattern);
}


// custom post-renderer
static ITCODSDLRenderer *renderer=NULL;
extern "C" void TCOD_CRenderer(void *sdl_surface) {
	if ( renderer ) renderer->render(sdl_surface);
}

void Doryen::Platform::registerSDLRenderer( ITCODSDLRenderer *renderer )
{
	::renderer = renderer;
	TCOD_sys_register_SDL_renderer(TCOD_CRenderer);
}

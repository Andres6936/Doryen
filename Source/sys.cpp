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
#include "libtcod.hpp"
#include <stdio.h>
#include <stdarg.h>

void Plataform::saveScreenshot( const char *filename )
{
	TCOD_sys_save_screenshot(filename);
}

void Plataform::sleepMilli( uint32 milliseconds )
{
	TCOD_sys_sleep_milli(milliseconds);
}

uint32 Plataform::getElapsedMilli( )
{
	return TCOD_sys_elapsed_milli();
}

float Plataform::getElapsedSeconds( )
{
	return TCOD_sys_elapsed_seconds();
}

void Plataform::forceFullscreenResolution( int width, int height )
{
	TCOD_sys_force_fullscreen_resolution(width,height);
}

void Plataform::setRenderer( TCOD_renderer_t renderer )
{
	TCOD_sys_set_renderer(renderer);
}

TCOD_event_t Plataform::waitForEvent( int eventMask, TCOD_key_t *key, TCOD_mouse_t *mouse, bool flush )
{
	return TCOD_sys_wait_for_event(eventMask,key,mouse,flush);
}

TCOD_event_t Plataform::checkForEvent( int eventMask, TCOD_key_t *key, TCOD_mouse_t *mouse )
{
	return TCOD_sys_check_for_event(eventMask,key,mouse);
}

TCOD_renderer_t Plataform::getRenderer( )
{
	return TCOD_sys_get_renderer();
}

void Plataform::setFps( int val )
{
	TCOD_sys_set_fps(val);
}

int Plataform::getFps( )
{
	return TCOD_sys_get_fps();
}

float Plataform::getLastFrameLength( )
{
	return TCOD_sys_get_last_frame_length();
}

void Plataform::getCurrentResolution( int *w, int *h )
{
	TCOD_sys_get_current_resolution(w,h);
}

void Plataform::getFullscreenOffsets( int *offx, int *offy )
{
	TCOD_sys_get_fullscreen_offsets(offx,offy);
}

void Plataform::updateChar( int asciiCode, int fontx, int fonty, const TCODImage *img, int x, int y )
{
	TCOD_sys_update_char(asciiCode,fontx,fonty,img->data,x,y);
}

void Plataform::getCharSize( int *w, int *h )
{
	TCOD_sys_get_char_size(w,h);
}

// filesystem stuff
bool Plataform::createDirectory( const char *path )
{
	return TCOD_sys_create_directory(path) != 0;
}

bool Plataform::deleteFile( const char *path )
{
	return TCOD_sys_delete_file(path) != 0;
}

bool Plataform::deleteDirectory( const char *path )
{
	return TCOD_sys_delete_directory(path) != 0;
}

bool Plataform::isDirectory( const char *path )
{
	return TCOD_sys_is_directory(path) != 0;
}

TCOD_list_t Plataform::getDirectoryContent( const char *path, const char *pattern )
{
	return TCOD_sys_get_directory_content(path,pattern);
}

bool Plataform::fileExists( const char *filename, ... )
{
	FILE * in;
	bool ret = false;
	char f[1024];
	va_list ap;
	va_start(ap,filename);
	vsprintf(f,filename,ap);
	va_end(ap);
	in = fopen(f,"rb");
	if (in != NULL) {
		ret = true;
		fclose(in);
	}
	return ret;
}

bool Plataform::readFile( const char *filename, unsigned char **buf, uint32 *size )
{
	return TCOD_sys_read_file(filename,buf,size) != 0;
}

bool Plataform::writeFile( const char *filename, unsigned char *buf, uint32 size )
{
	return TCOD_sys_write_file(filename,buf,size) != 0;
}

// clipboard stuff
void Plataform::setClipboard( const char *value )
{
	TCOD_sys_clipboard_set(value);
}

char *Plataform::getClipboard( )
{
	return TCOD_sys_clipboard_get();
}


// thread stuff
int Plataform::getNumCores( )
{
	return TCOD_sys_get_num_cores();
}

TCOD_thread_t Plataform::newThread( int (*func)( void * ), void *data )
{
	return TCOD_thread_new(func,data);
}

void Plataform::deleteThread( TCOD_thread_t th )
{
	TCOD_thread_delete(th);
}

void Plataform::waitThread( TCOD_thread_t th )
{
	TCOD_thread_wait(th);
}

// mutex
TCOD_mutex_t Plataform::newMutex( )
{
	return TCOD_mutex_new();
}

void Plataform::mutexIn( TCOD_mutex_t mut )
{
	TCOD_mutex_in(mut);
}

void Plataform::mutexOut( TCOD_mutex_t mut )
{
	TCOD_mutex_out(mut);
}

void Plataform::deleteMutex( TCOD_mutex_t mut )
{
	TCOD_mutex_delete(mut);
}

// semaphore
TCOD_semaphore_t Plataform::newSemaphore( int initVal )
{
	return TCOD_semaphore_new(initVal);
}

void Plataform::lockSemaphore( TCOD_semaphore_t sem )
{
	TCOD_semaphore_lock(sem);
}

void Plataform::unlockSemaphore( TCOD_semaphore_t sem )
{
	TCOD_semaphore_unlock(sem);
}

void Plataform::deleteSemaphore( TCOD_semaphore_t sem )
{
	TCOD_semaphore_delete(sem);
}

// condition
TCOD_cond_t Plataform::newCondition( )
{
	return TCOD_condition_new();
}

void Plataform::signalCondition( TCOD_cond_t cond )
{
	TCOD_condition_signal(cond);
}

void Plataform::broadcastCondition( TCOD_cond_t cond )
{
	TCOD_condition_broadcast(cond);
}

void Plataform::waitCondition( TCOD_cond_t cond, TCOD_mutex_t mut )
{
	TCOD_condition_wait(cond, mut);
}

void Plataform::deleteCondition( TCOD_cond_t cond )
{
	TCOD_condition_delete(cond);
}

// custom post-renderer
static ITCODSDLRenderer *renderer=NULL;
extern "C" void TCOD_CRenderer(void *sdl_surface) {
	if ( renderer ) renderer->render(sdl_surface);
}

void Plataform::registerSDLRenderer( ITCODSDLRenderer *renderer )
{
	::renderer = renderer;
	TCOD_sys_register_SDL_renderer(TCOD_CRenderer);
}

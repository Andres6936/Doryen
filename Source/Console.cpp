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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "libtcod.hpp"
#include "libtcod_int.h"

Doryen::Console *Doryen::Console::root = NULL;

Doryen::Console::Console( )
{
    TCOD_console_data_t *console = new TCOD_console_data_t;

    console->w = 80;
    console->h = 25;

    console->fore = TCOD_white;
    console->back = TCOD_black;
    console->fade = 255;
    console->buf = new char_t[console->w * console->h];
    console->oldbuf = new char_t[console->w * console->h];
    console->bkgnd_flag = TCOD_BKGND_NONE;
    console->alignment = TCOD_LEFT;

    for ( int j = 0; j < console->w * console->h; j++ )
    {
        console->buf[ j ].c = ' ';
        console->buf[ j ].cf = -1;
    }

    windowClose = false;

    for ( int i = 0; i < TCOD_COLCTRL_NUMBER; i++ )
    {
        controlBackground[ i ] = Doryen::Color( 0, 0, 0 ); // Black
        controlForeground[ i ] = Doryen::Color( 255, 255, 255 ); // White
    }

    data = console;
}

Doryen::Console::Console( int w, int h )
{
    if ( w > 0 && h > 0 )
    {
        TCOD_console_data_t *console = new TCOD_console_data_t;

        console->w = w;
        console->h = h;

        console->fore = TCOD_white;
        console->back = TCOD_black;
        console->fade = 255;
        console->buf = new char_t[console->w * console->h];
        console->oldbuf = new char_t[console->w * console->h];
        console->bkgnd_flag = TCOD_BKGND_NONE;
        console->alignment = TCOD_LEFT;

        for ( int j = 0; j < console->w * console->h; j++ )
        {
            console->buf[ j ].c = ' ';
            console->buf[ j ].cf = -1;
        }

        windowClose = false;

        for ( int i = 0; i < TCOD_COLCTRL_NUMBER; i++ )
        {
            controlBackground[ i ] = Doryen::Color( 0, 0, 0 ); // Black
            controlForeground[ i ] = Doryen::Color( 255, 255, 255 ); // White
        }

        data = console;
    }
    else
    {
        // Throw Error
    }
}

Doryen::Console::Console( const char *filename )
{
	data = TCOD_console_from_file(filename);
}

Doryen::Console::~Console( )
{
    // TCOD_console_delete(data);
}

void Doryen::Console::initRoot( int w, int h, const char *title, bool fullscreen, TCOD_renderer_t renderer )
{
    Doryen::Console *con = new Doryen::Console( );

    if ( w > 0 && h > 0 )
    {
        TCOD_console_data_t *console = new TCOD_console_data_t;

        console->w = w;
        console->h = h;

        TCOD_ctx.root = console;
        TCOD_ctx.renderer = renderer;

        console->fore = TCOD_white;
        console->back = TCOD_black;
        console->fade = 255;
        console->buf = new char_t[console->w * console->h];
        console->oldbuf = new char_t[console->w * console->h];
        console->bkgnd_flag = TCOD_BKGND_NONE;
        console->alignment = TCOD_LEFT;

        for ( int j = 0; j < console->w * console->h; j++ )
        {
            console->buf[ j ].c = ' ';
            console->buf[ j ].cf = -1;
        }

        if ( !TCOD_sys_init( console->w, console->h, console->buf,
                             console->oldbuf, fullscreen ))
        {
            // Throw Error
        }

        TCOD_sys_set_window_title( title );

        con->data = console;

        root = con;
    }
    else
    {
        // Throw Error
    }
}

bool Doryen::Console::loadAsc( const char *filename )
{
	return TCOD_console_load_asc(data,filename) != 0;
}

bool Doryen::Console::saveAsc( const char *filename ) const
{
	return TCOD_console_save_asc(data,filename) != 0;
}

bool Doryen::Console::saveApf( const char *filename ) const
{
	return TCOD_console_save_apf(data,filename) != 0;
}

bool Doryen::Console::loadApf( const char *filename )
{
	return TCOD_console_load_apf(data,filename) != 0;
}

void Doryen::Console::setCustomFont( const char *fontFile, int flags, int nbCharHoriz, int nbCharVertic )
{
	TCOD_console_set_custom_font(fontFile,flags,nbCharHoriz,nbCharVertic);
}

void Doryen::Console::setDirty( int x, int y, int w, int h )
{
	TCOD_console_set_dirty(x,y,w,h);
}

TCOD_key_t Doryen::Console::checkForKeypress( int flags )
{
	return TCOD_sys_check_for_keypress(flags);
}

TCOD_key_t Doryen::Console::waitForKeypress( bool flush )
{
	return TCOD_sys_wait_for_keypress(flush);
}

bool Doryen::Console::isWindowClosed( )
{
	return TCOD_console_is_window_closed() != 0;
}

int Doryen::Console::getWidth( ) const
{
	return TCOD_console_get_width(data);
}

int Doryen::Console::getHeight( ) const
{
	return TCOD_console_get_height(data);
}

void Doryen::Console::setColorControl( TCOD_colctrl_t con, const Doryen::Color &fore, const Doryen::Color &back )
{
	TCOD_color_t b={back.r,back.g,back.b},f={fore.r,fore.g,fore.b};
	TCOD_console_set_color_control(con,f,b);
}

Doryen::Color Doryen::Console::getDefaultBackground( ) const
{
	TCOD_color_t c= TCOD_console_get_default_background(data);
    Doryen::Color ret;
	ret.r=c.r;
	ret.g=c.g;
	ret.b=c.b;
	return ret;
}

Doryen::Color Doryen::Console::getDefaultForeground( ) const
{
    TCOD_color_t temp = TCOD_console_get_default_foreground( data );
    return Doryen::Color( temp.r, temp.g, temp.b );
}

void Doryen::Console::setDefaultBackground( Doryen::Color back )
{
	TCOD_color_t b={back.r,back.g,back.b};
	TCOD_console_set_default_background(data,b);
}

void Doryen::Console::setDefaultForeground( Doryen::Color fore )
{
	TCOD_color_t b={fore.r,fore.g,fore.b};
	TCOD_console_set_default_foreground(data,b);
}

void Doryen::Console::setWindowTitle( const char *title )
{
	TCOD_sys_set_window_title(title);
}

void Doryen::Console::setFullscreen( bool fullscreen )
{
	TCOD_console_set_fullscreen(fullscreen);
}

bool Doryen::Console::isFullscreen( )
{
	return TCOD_console_is_fullscreen() != 0;
}

void Doryen::Console::setBackgroundFlag( TCOD_bkgnd_flag_t bkgnd_flag )
{
	TCOD_console_set_background_flag(data,bkgnd_flag);
}

TCOD_bkgnd_flag_t Doryen::Console::getBackgroundFlag( ) const
{
	return TCOD_console_get_background_flag(data);
}

void Doryen::Console::setAlignment( TCOD_alignment_t alignment )
{
	TCOD_console_set_alignment(data,alignment);
}

TCOD_alignment_t Doryen::Console::getAlignment( ) const
{
	return TCOD_console_get_alignment(data);
}

void Doryen::Console::blit( const Doryen::Console *srcCon, int xSrc, int ySrc, int wSrc, int hSrc,
                            Doryen::Console *dstCon, int xDst, int yDst, float foreground_alpha,
                            float background_alpha )
{
	TCOD_console_blit(srcCon->data,xSrc,ySrc,wSrc,hSrc,dstCon->data,xDst,yDst,foreground_alpha, background_alpha);
}


void Doryen::Console::flush( )
{
	TCOD_console_flush();
}

void Doryen::Console::setFade( uint8 val, const Doryen::Color &fade )
{
	TCOD_color_t f= {fade.r,fade.g,fade.b};
	TCOD_console_set_fade(val,f);
}

uint8 Doryen::Console::getFade( )
{
	return TCOD_console_get_fade();
}

Doryen::Color Doryen::Console::getFadingColor( )
{
    TCOD_color_t temp = TCOD_console_get_fading_color( );
    return Doryen::Color( temp.r, temp.g, temp.b );
}

void Doryen::Console::putChar( int x, int y, int c, TCOD_bkgnd_flag_t flag )
{
	TCOD_console_put_char(data,x,y,c,flag);
}

void Doryen::Console::putCharEx( int x, int y, int c, const Doryen::Color &fore, const Doryen::Color &back )
{
	TCOD_color_t f={fore.r,fore.g,fore.b};
	TCOD_color_t b={back.r,back.g,back.b};
	TCOD_console_put_char_ex(data,x,y,c,f,b);
}

void Doryen::Console::clear( )
{
	TCOD_console_clear(data);
}

Doryen::Color Doryen::Console::getCharBackground( int x, int y ) const
{
    TCOD_color_t temp = TCOD_console_get_char_background( data, x, y );
    return Doryen::Color( temp.r, temp.g, temp.b );
}

void Doryen::Console::setCharForeground( int x, int y, const Doryen::Color &col )
{
	TCOD_color_t c={col.r,col.g,col.b};
	TCOD_console_set_char_foreground(data,x,y,c);
}

Doryen::Color Doryen::Console::getCharForeground( int x, int y ) const
{
    TCOD_color_t temp = TCOD_console_get_char_foreground( data, x, y );
    return Doryen::Color( temp.r, temp.g, temp.b );
}

int Doryen::Console::getChar( int x, int y ) const
{
	return TCOD_console_get_char(data,x,y);
}

void Doryen::Console::setCharBackground( int x, int y, const Doryen::Color &col, TCOD_bkgnd_flag_t flag )
{
	TCOD_color_t c={col.r,col.g,col.b};
	TCOD_console_set_char_background(data,x,y,c,flag);
}

void Doryen::Console::setChar( int x, int y, int c )
{
	TCOD_console_set_char(data,x,y,c);
}

void Doryen::Console::rect( int x, int y, int rw, int rh, bool clear, TCOD_bkgnd_flag_t flag )
{
	TCOD_console_rect(data,x,y,rw,rh,clear,flag);
}

void Doryen::Console::hline( int x, int y, int l, TCOD_bkgnd_flag_t flag )
{
	TCOD_console_hline(data,x,y,l,flag);
}

void Doryen::Console::vline( int x, int y, int l, TCOD_bkgnd_flag_t flag )
{
	TCOD_console_vline(data,x,y,l,flag);
}

void Doryen::Console::printFrame( int x, int y, int w, int h, bool empty, TCOD_bkgnd_flag_t flag, const char *fmt, ... )
{
	if ( fmt ) {
		va_list ap;
		va_start(ap,fmt);
		TCOD_console_print_frame(data,x,y,w,h,empty,flag,TCOD_console_vsprint(fmt,ap));
		va_end(ap);
	} else {
		TCOD_console_print_frame(data,x,y,w,h,empty,flag,NULL);
	}
}

void Doryen::Console::print( int x, int y, const char *fmt, ... )
{
	va_list ap;
	TCOD_console_data_t *dat=(TCOD_console_data_t *)data;
	TCOD_IFNOT ( dat != NULL ) return;
	va_start(ap,fmt);
	TCOD_console_print_internal(data,x,y,0,0,dat->bkgnd_flag,dat->alignment,
		TCOD_console_vsprint(fmt,ap),false,false);
	va_end(ap);
}

void Doryen::Console::printEx( int x, int y, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const char *fmt, ... )
{
	va_list ap;
	va_start(ap,fmt);
	TCOD_console_print_internal(data,x,y,0,0,flag,alignment,TCOD_console_vsprint(fmt,ap),false,false);
	va_end(ap);
}


/*
void Doryen::Console::printLine(int x, int y, TCOD_bkgnd_flag_t flag, TCOD_print_location_t location, const char *fmt, ...) {
	va_list ap;
	va_start(ap,fmt);
	switch(location)
	{
		case TCOD_PRINT_LEFT:
			TCOD_console_print(data,x,y,getWidth()-x,getHeight()-y,flag,LEFT,TCOD_console_vsprint(fmt,ap),false,false);
			break;
		case TCOD_PRINT_RIGHT:
			TCOD_console_print(data,x,y,x+1,getHeight()-y,flag,RIGHT,TCOD_console_vsprint(fmt,ap),false,false);
			break;
		case TCOD_PRINT_CENTER:
			TCOD_console_print(data,x,y,getWidth(),getHeight()-y,flag,CENTER,TCOD_console_vsprint(fmt,ap),false,false);
			break;
		default:
			TCOD_ASSERT(0);
			break;
	}
	va_end(ap);
}
*/

int Doryen::Console::printRect( int x, int y, int w, int h, const char *fmt, ... )
{
	va_list ap;
	TCOD_console_data_t *dat=(TCOD_console_data_t *)data;
	TCOD_IFNOT ( dat != NULL ) return 0;
	va_start(ap,fmt);
	int ret = TCOD_console_print_internal(data,x,y,w,h,dat->bkgnd_flag,dat->alignment,TCOD_console_vsprint(fmt,ap),true,false);
	va_end(ap);
	return ret;
}

int Doryen::Console::printRectEx( int x, int y, int w, int h, TCOD_bkgnd_flag_t flag,
                                  TCOD_alignment_t alignment, const char *fmt, ... )
{
	va_list ap;
	va_start(ap,fmt);
	int ret = TCOD_console_print_internal(data,x,y,w,h,flag,alignment,TCOD_console_vsprint(fmt,ap),true,false);
	va_end(ap);
	return ret;
}

int Doryen::Console::getHeightRect( int x, int y, int w, int h, const char *fmt, ... )
{
	va_list ap;
	va_start(ap,fmt);
	int ret = TCOD_console_print_internal(data,x,y,w,h,TCOD_BKGND_NONE,TCOD_LEFT,TCOD_console_vsprint(fmt,ap),true,true);
	va_end(ap);
	return ret;
}

void Doryen::Console::setKeyboardRepeat( int initialDelay, int interval )
{
	TCOD_console_set_keyboard_repeat(initialDelay,interval);
}

void Doryen::Console::disableKeyboardRepeat( )
{
	TCOD_console_disable_keyboard_repeat();
}

bool Doryen::Console::isKeyPressed( TCOD_keycode_t key )
{
	return TCOD_console_is_key_pressed(key) != 0;
}

void Doryen::Console::setKeyColor( const Doryen::Color &col )
{
	TCOD_color_t c={col.r,col.g,col.b};
	TCOD_console_set_key_color(data,c);
}

void Doryen::Console::credits( )
{
	TCOD_console_credits();
}

void Doryen::Console::resetCredits( )
{
	TCOD_console_credits_reset();
}

bool Doryen::Console::renderCredits( int x, int y, bool alpha )
{
	return TCOD_console_credits_render(x,y,alpha) != 0;
}

#ifndef NO_UNICODE

void Doryen::Console::mapStringToFont( const wchar_t *s, int fontCharX, int fontCharY )
{
	TCOD_console_map_string_to_font_utf(s, fontCharX, fontCharY);
}

void Doryen::Console::print( int x, int y, const wchar_t *fmt, ... )
{
	va_list ap;
	TCOD_console_data_t *dat=(TCOD_console_data_t *)data;
	TCOD_IFNOT ( dat != NULL ) return;
	va_start(ap,fmt);
	TCOD_console_print_internal_utf(data,x,y,0,0,dat->bkgnd_flag,dat->alignment,TCOD_console_vsprint_utf(fmt,ap),false,false);
	va_end(ap);
}

void
Doryen::Console::printEx( int x, int y, TCOD_bkgnd_flag_t flag, TCOD_alignment_t alignment, const wchar_t *fmt, ... )
{
	va_list ap;
	va_start(ap,fmt);
	TCOD_console_print_internal_utf(data,x,y,0,0,flag,alignment,TCOD_console_vsprint_utf(fmt,ap),false,false);
	va_end(ap);
}

int Doryen::Console::printRect( int x, int y, int w, int h, const wchar_t *fmt, ... )
{
	va_list ap;
	TCOD_console_data_t *dat=(TCOD_console_data_t *)data;
	TCOD_IFNOT ( dat != NULL ) return 0;
	va_start(ap,fmt);
	int ret = TCOD_console_print_internal_utf(data,x,y,w,h,dat->bkgnd_flag,dat->alignment,
		TCOD_console_vsprint_utf(fmt,ap),true,false);
	va_end(ap);
	return ret;
}

int Doryen::Console::printRectEx( int x, int y, int w, int h, TCOD_bkgnd_flag_t flag,
                                  TCOD_alignment_t alignment, const wchar_t *fmt, ... )
{
	va_list ap;
	va_start(ap,fmt);
	int ret = TCOD_console_print_internal_utf(data,x,y,w,h,flag,alignment,
		TCOD_console_vsprint_utf(fmt,ap),true,false);
	va_end(ap);
	return ret;
}

int Doryen::Console::getHeightRect( int x, int y, int w, int h, const wchar_t *fmt, ... )
{
	va_list ap;
	va_start(ap,fmt);
	int ret = TCOD_console_print_internal_utf(data,x,y,w,h,TCOD_BKGND_NONE,TCOD_LEFT,TCOD_console_vsprint_utf(fmt,ap),true,true);
	va_end(ap);
	return ret;
}

// ctrl = TCOD_COLCTRL_1...TCOD_COLCTRL_5 or TCOD_COLCTRL_STOP
#define NB_BUFFERS 10

#endif



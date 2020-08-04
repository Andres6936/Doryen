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

#ifndef _TCOD_CONSOLE_H
#define _TCOD_CONSOLE_H

#include "console_types.h"
#include "Color.h"

typedef void* TCOD_console_t;

typedef unsigned char uint8;

void TCOD_console_set_window_title(const char* title);


void TCOD_console_set_default_background(TCOD_console_t con,TCOD_color_t col);

void TCOD_console_set_default_foreground(TCOD_console_t con,TCOD_color_t col);

void TCOD_console_set_char_background(TCOD_console_t con,int x, int y, TCOD_color_t col, TCOD_bkgnd_flag_t flag);

void TCOD_console_set_char_foreground(TCOD_console_t con,int x, int y, TCOD_color_t col);

void TCOD_console_set_char(TCOD_console_t con,int x, int y, int c);

void TCOD_console_put_char(TCOD_console_t con,int x, int y, int c, TCOD_bkgnd_flag_t flag);


TCOD_color_t TCOD_console_get_default_background(TCOD_console_t con);

TCOD_color_t TCOD_console_get_default_foreground(TCOD_console_t con);

TCOD_color_t TCOD_console_get_char_background(TCOD_console_t con,int x, int y);

TCOD_color_t TCOD_console_get_char_foreground(TCOD_console_t con,int x, int y);

int TCOD_console_get_char(TCOD_console_t con,int x, int y);

void TCOD_console_set_fade(uint8 val, TCOD_color_t fade);

uint8 TCOD_console_get_fade();

TCOD_color_t TCOD_console_get_fading_color();

void TCOD_console_flush();


void TCOD_console_set_keyboard_repeat(int initial_delay, int interval);

void TCOD_console_disable_keyboard_repeat();

bool TCOD_console_is_key_pressed(TCOD_keycode_t key);

int TCOD_console_get_width(TCOD_console_t con);

int TCOD_console_get_height(TCOD_console_t con);

void TCOD_console_credits();

void TCOD_console_credits_reset();

#endif

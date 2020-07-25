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

#ifndef _TCOD_SYS_H
#define _TCOD_SYS_H

#include <Doryen/list.h>
#include <Doryen/Image.h>
#include <Doryen/mouse_types.h>
#include <Doryen/console_types.h>

typedef unsigned char uint8;
typedef unsigned int uint32;

uint32 TCOD_sys_elapsed_milli();

float TCOD_sys_elapsed_seconds();

void TCOD_sys_sleep_milli(uint32 val);

void TCOD_sys_save_screenshot(const char* filename);

void TCOD_sys_force_fullscreen_resolution(int width, int height);

void TCOD_sys_set_fps(int val);

int TCOD_sys_get_fps();

float TCOD_sys_get_last_frame_length();

void TCOD_sys_get_current_resolution(int *w, int *h);

void TCOD_sys_update_char(int asciiCode, int fontx, int fonty, TCOD_image_t img, int x, int y);

void TCOD_sys_get_char_size(int *w, int *h);

void *TCOD_sys_get_sdl_window();

typedef enum {
    TCOD_EVENT_KEY_NONE = 0,
  TCOD_EVENT_KEY_PRESS=1,
  TCOD_EVENT_KEY_RELEASE=2,
  TCOD_EVENT_KEY=TCOD_EVENT_KEY_PRESS|TCOD_EVENT_KEY_RELEASE,
  TCOD_EVENT_MOUSE_MOVE=4,
  TCOD_EVENT_MOUSE_PRESS=8,
  TCOD_EVENT_MOUSE_RELEASE=16,
  TCOD_EVENT_MOUSE=TCOD_EVENT_MOUSE_MOVE|TCOD_EVENT_MOUSE_PRESS|TCOD_EVENT_MOUSE_RELEASE,
  TCOD_EVENT_ANY=TCOD_EVENT_KEY|TCOD_EVENT_MOUSE,
} TCOD_event_t;

TCOD_event_t TCOD_sys_check_for_event(int eventMask, TCOD_key_t *key, TCOD_mouse_t *mouse);

/* filesystem stuff */

TCOD_list_t TCOD_sys_get_directory_content(const char *path, const char *pattern);


/* clipboard */
void TCOD_sys_clipboard_set(const char *value);

char *TCOD_sys_clipboard_get();

typedef void *TCOD_semaphore_t;
typedef void *TCOD_mutex_t;

/* dynamic library */
typedef void *TCOD_library_t;

/* SDL renderer callback */
typedef void (*SDL_renderer_t) (void *sdl_surface);

#endif

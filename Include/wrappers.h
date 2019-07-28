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
#ifndef WRAPPERS_H
#define WRAPPERS_H

/* wrappers to ease other languages integration */
typedef unsigned int colornum_t;

/* color module */
TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API

/* console module */
/* TCODLIB_API void TCOD_console_set_custom_font_wrapper(const char *fontFile,
                        int char_width, int char_height, int nb_char_horiz,
                        int nb_char_vertic, bool chars_by_row,
                        colornum_t key_color); */

TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API
TCODLIB_API uint32 TCOD_console_check_for_keypress_bitfield (int flags);
TCODLIB_API uint32 TCOD_console_wait_for_keypress_bitfield (bool flush);

TCODLIB_API
        TCODLIB_API

TCODLIB_API

        TCODLIB_API

TCODLIB_API
        TCODLIB_API

TCODLIB_API
TCODLIB_API void console_set_key_color_wrapper (TCOD_console_t con, colornum_t c);

/* image module */

TCODLIB_API
        TCODLIB_API

TCODLIB_API
        TCODLIB_API

TCODLIB_API

/* mouse module */
        TCODLIB_API
TCODLIB_API int TCOD_mouse_get_x();
TCODLIB_API int TCOD_mouse_get_y();
TCODLIB_API int TCOD_mouse_get_cx();
TCODLIB_API int TCOD_mouse_get_cy();
TCODLIB_API int TCOD_mouse_get_dx();
TCODLIB_API int TCOD_mouse_get_dy();
TCODLIB_API int TCOD_mouse_get_dcx();
TCODLIB_API int TCOD_mouse_get_dcy();
TCODLIB_API uint32 TCOD_mouse_get_lbutton();
TCODLIB_API uint32 TCOD_mouse_get_mbutton();
TCODLIB_API uint32 TCOD_mouse_get_rbutton();
TCODLIB_API uint32 TCOD_mouse_get_lbutton_pressed();
TCODLIB_API uint32 TCOD_mouse_get_mbutton_pressed();
TCODLIB_API uint32 TCOD_mouse_get_rbutton_pressed();

/* parser module */
TCODLIB_API

/* namegen module */
        TCODLIB_API

TCODLIB_API

/* sys module */
        TCODLIB_API

TCODLIB_API

#endif	/* WRAPPERS_H */


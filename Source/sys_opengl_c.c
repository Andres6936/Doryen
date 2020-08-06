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

/*
 * This renderer is mostly copied and pasted from Antagonist's SkyFire GLSL roguelike engine
 */

#include "Doryen/libtcod.h"
#include "Doryen/libtcod_int.h"

#ifndef NO_OPENGL

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define CHECKGL(GLcall)                                    \
    GLcall;                                                    \
    if(!_CheckGL_Error( #GLcall, __FILE__, __LINE__))            \
    return false;

#ifdef NDEBUG
#define DBGCHECKGL(GLcall) GLcall
#else
#define DBGCHECKGL CHECKGL
#endif

typedef  enum 
{
	Character,
	ForeCol,
	BackCol,
	ConsoleDataEnumSize
} ConsoleDataEnum;
/* JBR04152012 - Made Character a 4 byte value here to support extended characters like other renderers.
   Seems like it should be possible to make it a two byte value using GL_UNSIGNED_SHORT_5_6_5_REV in updateTex, 
   but I can't seem to get the math right in the shader code, it always loses precision somewhere,
   resulting in incorrect characters. */
const int ConsoleDataAlignment[3] = {4, 3, 3 };

static const char *TCOD_con_vertex_shader =
#ifndef NDEBUG
"#version 110\n"
#endif
"uniform vec2 termsize; "

"void main(void) "
"{ "

"   gl_Position = gl_Vertex; "

"   gl_TexCoord[0] = gl_MultiTexCoord0; "
"   gl_TexCoord[0].x = gl_TexCoord[0].x*termsize.x; "
"   gl_TexCoord[0].y = gl_TexCoord[0].y*termsize.y; "
"} "
;

static const char *TCOD_con_pixel_shader =
#ifndef NDEBUG
"#version 110\n"
#endif
"uniform sampler2D font; "
"uniform sampler2D term; "
"uniform sampler2D termfcol; "
"uniform sampler2D termbcol; "

"uniform float fontw; "
"uniform vec2 fontcoef; "
"uniform vec2 termsize; "
"uniform vec2 termcoef; "

"void main(void) "
"{ "
"   vec2 rawCoord = gl_TexCoord[0].xy; "                           /* varying from [0, termsize) in x and y */
"   vec2 conPos = floor(rawCoord); "                               /* console integer position */
"   vec2 pixPos = fract(rawCoord); "                               /* pixel offset within console position */
"   pixPos = vec2(pixPos.x*fontcoef.x,pixPos.y*fontcoef.y); "      /* Correct pixel offset for font tex location */

"   vec2 address = vec2(conPos.x*termcoef.x,conPos.y*termcoef.y); "
"	address=address+vec2(0.001, 0.001); "
"   vec4 charvec = texture2D(term,address);"
"   float inchar = (charvec.r * 256.0) + (charvec.g * 256.0 * 256.0);"          /* character */
"   vec4 tcharfcol = texture2D(termfcol, address); "           /* front color */
"   vec4 tcharbcol = texture2D(termbcol, address); "           /* back color */

"   vec4 tchar = vec4(mod(floor(inchar),floor(fontw)),floor(inchar/fontw), 0.0, 0.0); "  /* 1D index to 2D index map for character */

"   gl_FragColor = texture2D(font, vec2((tchar.x*fontcoef.x),(tchar.y*fontcoef.y))+pixPos.xy); "   /* magic func: finds pixel value in font file */
"   gl_FragColor=gl_FragColor.a*tcharfcol+(1.0-gl_FragColor.a)*tcharbcol; "      /* Coloring stage */
"} "
;

bool _CheckGL_Error(const char* GLcall, const char* file, const int line)
{
    GLenum errCode;
    if((errCode = glGetError())!=GL_NO_ERROR)
    {
		printf("OPENGL ERROR #%i: in file %s on line %i\n",errCode,file, line);
        printf("OPENGL Call: %s\n",GLcall);
        return false;
    }
    return true;
}

/* console size (power of 2 and cells) */
static int POTconwidth, POTconheight, conwidth, conheight;
/* programs and shaders handles */
static GLuint conProgram, conVertShader, conFragShader;
/* font texture handle */
static GLuint font_tex;
/* font power of 2 size and pixels */
static int POTfontwidth,POTfontheight, fontwidth,fontheight;
/* console data */
static GLuint Tex[ConsoleDataEnumSize];
static unsigned char *data[ConsoleDataEnumSize];
static bool dirty[ConsoleDataEnumSize];

/* extension function pointers */
static PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB=0;
static PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB=0;
static PFNGLSHADERSOURCEARBPROC glShaderSourceARB=0;
static PFNGLCOMPILESHADERARBPROC glCompileShaderARB=0;
static PFNGLGETINFOLOGARBPROC glGetInfoLogARB=0;
static PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB=0;
static PFNGLATTACHOBJECTARBPROC glAttachObjectARB=0;
static PFNGLLINKPROGRAMARBPROC glLinkProgramARB=0;
static PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB=0;
static PFNGLUNIFORM2FARBPROC glUniform2fARB=0;
static PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB=0;
static PFNGLUNIFORM1FARBPROC glUniform1fARB=0;
static PFNGLUNIFORM1IARBPROC glUniform1iARB=0;
#ifdef TCOD_WINDOWS
static PFNGLACTIVETEXTUREPROC glActiveTexture=0;
#endif
                                        
/* call after creating window */

static GLuint loadShader(const char *txt, GLuint type) {
	int success;
	int infologLength = 0;
	int charsWritten = 0;
    char *infoLog;
	GLuint v = glCreateShaderObjectARB(type);
	glShaderSourceARB(v, 1, &txt, 0);
	glCompileShaderARB(v);

	glGetObjectParameterivARB(v, GL_COMPILE_STATUS, &success);
	if(success!=GL_TRUE)
	{
	    /* something went wrong */
		glGetObjectParameterivARB(v, GL_INFO_LOG_LENGTH,&infologLength);
		if(infologLength>0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetInfoLogARB(v, infologLength, &charsWritten, infoLog);
			printf("GLSL ERROR : %s\n",infoLog);
			free(infoLog);
		}
		return 0;
	}

	return v;
}

static bool loadProgram(const char *vertShaderCode, const char *fragShaderCode,
	GLuint *vertShader, GLuint *fragShader, GLuint *prog) {
	/* Create and load Program and Shaders */
	int success;
	*prog = DBGCHECKGL(glCreateProgramObjectARB());

	*vertShader = loadShader(vertShaderCode, GL_VERTEX_SHADER);
	if ( *vertShader == 0 ) return false;
	glAttachObjectARB(*prog, *vertShader);

	*fragShader = loadShader(fragShaderCode, GL_FRAGMENT_SHADER);
	if ( *fragShader == 0 ) return false;
	glAttachObjectARB(*prog, *fragShader);

	glLinkProgramARB(*prog);

	glGetObjectParameterivARB(*prog, GL_LINK_STATUS, &success);
	if(success!=GL_TRUE)
	{
		/* something went wrong */
		int infologLength = 0;
		int charsWritten = 0;
		char *infoLog;
		glGetObjectParameterivARB(*prog, GL_INFO_LOG_LENGTH,&infologLength);
		if (infologLength > 0)
	    {
	        infoLog = (char *)malloc(infologLength);
	        glGetInfoLogARB(*prog, infologLength, &charsWritten, infoLog);
			printf("OPENGL ERROR: Program link Error");
			printf("%s\n",infoLog);
	        free(infoLog);
	    }
		return false;
	}
	return true;
}

static void updateChar(ConsoleDataEnum dataType, int BufferPos, unsigned char *c, int length, int offset) {
	int i;
	dirty[dataType] = true;		/* Set dirty so Texture gets updated next frame */

	for(i = 0; i<length; i++) {
		data[dataType][BufferPos*ConsoleDataAlignment[dataType] + i + offset] = c[i];
	}

}

void TCOD_opengl_swap() {
	SDL_GL_SwapBuffers();
}

#endif


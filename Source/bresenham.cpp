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

#include <bresenham.hpp>

#include "libtcod.hpp"

// ********** bresenham line drawing **********
void Doryen::Line::init( int xFrom, int yFrom, int xTo, int yTo )
{
    origx = xFrom;
    origy = yFrom;
    destx = xTo;
    desty = yTo;
    deltax = xTo - xFrom;
    deltay = yTo - yFrom;

    if ( deltax > 0 )
    {
        stepx = 1;
    }
    else if ( deltax < 0 )
    {
        stepx = -1;
    }
    else
    {
        stepx = 0;
    }

    if ( deltay > 0 )
    {
        stepy = 1;
    }
    else if ( deltay < 0 )
    {
        stepy = -1;
    }
    else
    {
        stepy = 0;
    }

    if ( stepx * deltax > stepy * deltay )
    {
        e = stepx * deltax;

        deltax = deltax * 2;
        deltay = deltay * 2;
    }
    else
    {
        e = stepy * deltay;

        deltax = deltax * 2;
        deltay = deltay * 2;
    }
}

bool Doryen::Line::step( int *xCur, int *yCur )
{
    if ( stepx * deltax > stepy * deltay )
    {
        if ( origx == destx )
        {
            return true;
        }

        origx = origx + stepx;
        e = e - stepy * deltay;

        if ( e < 0 )
        {
            origy = origy + stepy;
            e = e + stepx * deltax;
        }
    }
    else
    {
        if ( origy == desty )
        {
            return true;
        }

        origy = origy + stepy;
        e = e - stepx * deltax;

        if ( e < 0 )
        {
            origx = origx + stepx;
            e = e + stepy * deltay;
        }
    }

    *xCur = origx;
    *yCur = origy;

    return false;
}

bool Doryen::Line::line(int xFrom, int yFrom, int xTo, int yTo, LineListener& plistener)
{
	static Doryen::Line lineTemp = Doryen::Line();

	lineTemp.init(xFrom, yFrom, xTo, yTo);


	while (!lineTemp.step(&xFrom, &yFrom))
	{
		if (!plistener.drawPoint(xFrom, yFrom))
		{
			return false;
		}
    }

    return true;
}

Doryen::Line::Line( )
{
    stepx = 0;
    stepy = 0;
    e = 0;
    deltax = 0;
    deltay = 0;
    origx = 0;
    origy = 0;
    destx = 0;
    desty = 0;
}



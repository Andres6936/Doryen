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

#include <Doryen/Algorithms/Drawing/Bresenham.hpp>

// ********** bresenham line drawing **********
void Doryen::Algorithms::Line::init(int xFrom, int yFrom, int xTo, int yTo)
{
	delta = { xTo - xFrom, yTo - yFrom };
	origin = { xFrom, yFrom };
	destination = { xTo, yTo };

	if (delta.x > 0)
	{
		stepCoordinate.x = 1;
	}
	else if (delta.x < 0)
	{
		stepCoordinate.x = -1;
	}
	else
	{
		stepCoordinate.x = 0;
	}

	if (delta.y > 0)
	{
		stepCoordinate.y = 1;
	}
	else if (delta.y < 0)
	{
		stepCoordinate.y = -1;
	}
	else
	{
		stepCoordinate.y = 0;
	}

	if (stepCoordinate.x * delta.x > stepCoordinate.y * delta.y)
	{
		e = stepCoordinate.x * delta.x;

		delta.x = delta.x * 2;
		delta.y = delta.y * 2;
	}
	else
	{
		e = stepCoordinate.y * delta.y;

		delta.x = delta.x * 2;
		delta.y = delta.y * 2;
	}
}

bool Doryen::Algorithms::Line::step(int* xCur, int* yCur)
{
	if (stepCoordinate.x * delta.x > stepCoordinate.y * delta.y)
	{
		if (origin.x == destination.x)
		{
			return true;
		}

		origin.x = origin.x + stepCoordinate.x;
		e = e - stepCoordinate.y * delta.y;

		if (e < 0)
		{
			origin.y = origin.y + stepCoordinate.y;
			e = e + stepCoordinate.x * delta.x;
		}
	}
    else
	{
		if (origin.y == destination.y)
		{
			return true;
		}

		origin.y = origin.y + stepCoordinate.y;
		e = e - stepCoordinate.x * delta.x;

		if (e < 0)
		{
			origin.x = origin.x + stepCoordinate.x;
			e = e + stepCoordinate.y * delta.y;
		}
	}

	*xCur = origin.x;
	*yCur = origin.y;

	return false;
}

bool Doryen::Algorithms::Line::line(int xFrom, int yFrom, int xTo, int yTo, Doryen::Algorithms::LineListener& plistener)
{
    static Doryen::Algorithms::Line lineTemp = Doryen::Algorithms::Line();

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

Doryen::Algorithms::Line::Line()
{
    e = 0;
}



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

#ifndef _TCOD_BRESENHAM_HPP
#define _TCOD_BRESENHAM_HPP

#include <Doryen/Geometry/Point2D.hpp>

namespace Doryen
{

	namespace Algorithms
	{

		class LineListener
		{

		public :

			virtual bool drawPoint(int x, int y) = 0;

			virtual ~LineListener()
			{

			};
		};

		/**
		 * @brief This toolkit is a very simple and lightweight implementation of the
		 * bresenham line drawing algorithm. It allows you to follow straight paths
		 * on your map very easily.
		 */
		class Line
		{

		private:

			Geometry::Point2D<> stepCoordinate{};
			Geometry::Point2D<> delta{};
			Geometry::Point2D<> origin{};
			Geometry::Point2D<> destination{};

			int e;

		public :

			Line();

			/**
			 * @brief Initializing the line.
			 *
			 * First, you have to initialize the toolkit with your starting and ending
			 * coordinates.
			 *
			 * @param xFrom Coordinates of the line's starting point.
			 * @param yFrom Coordinates of the line's starting point.
			 * @param xTo Coordinates of the line's ending point.
			 * @param yTo Coordinates of the line's ending point.
			 */
			void init(int xFrom, int yFrom, int xTo, int yTo);

			/**
			 * @brief Walking the line.
			 *
			 * You can then step through each cell with this function. It returns true
			 * when you reach the line's ending point.
			 *
			 * @param xCur the coordinates of the next cell on the line are stored
			 * here when the function returns.
			 *
			 * @param yCur the coordinates of the next cell on the line are stored
			 * here when the function returns
			 *
			 * @return true when you reach the line's ending point.
			 */
			bool step(int* xCur, int* yCur);

			/**
			 * @brief Callback-based function.
			 *
			 * The function returns false if the line has been interrupted by the
			 * callback (it returned false before the last point).
			 *
			 * @param xFrom Coordinates of the line's starting point.
			 * @param yFrom Coordinates of the line's starting point.
			 * @param xTo Coordinates of the line's ending point.
			 * @param yTo Coordinates of the line's ending point.
			 *
			 * @param plistener Callback called for each line's point.
			 * The function stops if the callback returns false.
			 *
			 * @return
			 */
			bool line(int xFrom, int yFrom, int xTo, int yTo, LineListener& plistener);
		};
	}

}

#endif

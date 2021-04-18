/*
* Photon reactor
* Copyright (c) 2011 Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The names of Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY JICE ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL JICE BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cmath>
#include "StandardShader.hpp"

void StandardShader::compute()
{
	std::fill(lightmap.begin(), lightmap.end(), Color{});

	for (Light& light : lights)
	{
		// compute the potential visible set for this light
		const std::uint32_t MIN_X = std::max(0, (int)light.coordinate.x - light.radius);
		const std::uint32_t MIN_Y = std::max(0, (int)light.coordinate.y - light.radius);

		const std::uint32_t MAX_X = std::min(light.coordinate.x + light.radius, (unsigned)map.getWidth() - 1);
		const std::uint32_t MAX_Y = std::min(light.coordinate.y + light.radius, (unsigned)map.getHeight() - 1);

		float offset = 1.0f / (1.0f + (float)(light.radius * light.radius) / 20);
		float factor = 1.0f / (1.0f - offset);
		// compute the light's fov
		Doryen::Map lmap(MAX_X - MIN_X + 1, MAX_Y - MIN_Y + 1);

		for (int x = MIN_X; x <= MAX_X; x++)
		{
			for (int y = MIN_Y; y <= MAX_Y; y++)
			{
				lmap.setProperties(x - MIN_X, y - MIN_Y, map.isTransparent(x, y), map.isWalkable(x, y));
			}
		}

		lmap.computeFov(light.coordinate.x - MIN_X, light.coordinate.y - MIN_Y, light.radius);
		// compute the light's contribution		
		//double invSquaredRadius=1.0 / (l.radius*l.radius);
		for (int x = MIN_X; x <= MAX_X; x++)
		{
			for (int y = MIN_Y; y <= MAX_Y; y++)
			{
				if (lmap.isVisibleFieldView(x - MIN_X, y - MIN_Y))
				{
					int squaredDist =
							(light.coordinate.x - x) * (light.coordinate.x - x) +
							(light.coordinate.y - y) * (light.coordinate.y - y);
					// basic
					//double coef = 1.0-squaredDist*invSquaredRadius;
					// invsqr1
					//double coef=(1.0f/(1.0f+(float)(squaredDist)));
					// invsqr2
					double coef = (1.0f / (1.0f + (float)(squaredDist) / 20) - offset) * factor;

					// Save a copy for avoid edit the original color
					const Color copy = light.col;

					light.col.multiply(coef);

					lightmap[x + y * map.getWidth()].add(light.col);

					// Recovery the original color
					light.col.copy(copy);
				}
			}
		}
	}
}

void StandardShader::calculateShaders()
{

}


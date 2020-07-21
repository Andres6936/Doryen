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

#pragma once

#include "Doryen/libtcod.hpp"

using namespace Doryen;

class Shader
{

protected :

	class Light
	{

	public:

		int radius = 0;

		Color col{};

		Geometry::Point2D<std::uint32_t> coordinate{};
	};

	/**
	 * Object that generate a source of light.
	 */
	std::vector<Light> lights;

	std::vector<Color> lightmap;

	Map& map;

public :

	Shader(Map& _map) noexcept;

	virtual ~Shader() = default;

	// add a new light. return its ID
	virtual int addLight(const Geometry::Point2D<uint32_t>& _coordinate, int radius, const Doryen::Color& col);

	// update the light with given ID
	virtual void updateLight(int id, int x, int y, int radius, const Doryen::Color& col);

	virtual void compute() = 0;

	virtual void calculateShaders() = 0;

	// get the computed light color
	virtual const Doryen::Color& getLightColor(int x, int y);
};

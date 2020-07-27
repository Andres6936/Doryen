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

#include "PhotonShader.hpp"

PhotonShader::PhotonShader(Map& map) noexcept: Shader(map)
{

}

int PhotonShader::addLight(const Geometry::Point2D<uint32_t>& _coordinate, int radius, const Doryen::Color& col)
{
	if (radius > maxRadius) maxRadius = radius;
	return Shader::addLight(_coordinate, radius, col);
}

// compute form factor of cell x,y relative to all its surrounding cells
void PhotonShader::computeFormFactor(int x, int y)
{
	int ominx = x - maxRadius;
	int ominy = y - maxRadius;
	int omaxx = x + maxRadius;
	int omaxy = y + maxRadius;
	int minx = MAX(ominx, 0);
	int miny = MAX(ominy, 0);
	int maxx = MIN(omaxx, map.getWidth() - 1);
	int maxy = MIN(omaxy, map.getHeight() - 1);
	int maxDiameter = 2 * maxRadius + 1;

	auto cellFormFactor = formFactor.begin() + (x + y * map.getWidth()) * maxDiameter * maxDiameter;

	map.computeFov(x, y, maxRadius);
	int squareRad = (maxRadius * maxRadius);
	//float invRad=1.0/squareRad;
	double curFfSum = 0;
	float offset = 1.0f / (1.0f + (float)(maxRadius * maxRadius) / 20);
	float factor = 1.0f / (1.0f - offset);
	for (int cx = minx, cdx = minx - ominx; cx <= maxx; cx++, cdx++)
	{
		for (int cy = miny, cdy = miny - ominy; cy <= maxy; cy++, cdy++)
		{
			if (map.isVisibleFieldView(cx, cy))
			{
				int dist = (maxRadius - cdx) * (maxRadius - cdx) + (maxRadius - cdy) * (maxRadius - cdy);
				if (dist <= squareRad)
				{
					//float value = (1.0f-dist*invRad) ;
					//float value =1.0f/(1.0f+(float)(dist)/20);
					float value = (1.0f / (1.0f + (float)(dist) / 20) - offset) * factor;
					curFfSum += value;
					cellFormFactor[cdx + cdy * maxDiameter] = value;
				}
			}
		}
	}
	// scale so that the sum of all form factors for cell x,y is 1.0
	ffSum[x + y * map.getWidth()] = curFfSum;
	if (curFfSum > 1E-8)
	{
		curFfSum = 1.0 / curFfSum;
		for (int cx = minx, cdx = minx - ominx; cx <= maxx; cx++, cdx++)
		{
			for (int cy = miny, cdy = miny - ominy; cy <= maxy; cy++, cdy++)
			{
				cellFormFactor[cdx + cdy * maxDiameter] *= curFfSum;
			}
		}
	}
}

void PhotonShader::computeLightContribution(int lx, int ly, int lradius, const FColor &lcol, float reflectivity)
{
	const std::int32_t O_MIN_X = lx - lradius;
	const std::int32_t O_MIN_Y = ly - lradius;
	const std::int32_t O_MAX_X = lx + lradius;
	const std::int32_t O_MAX_Y = ly + lradius;

	const std::int32_t MIN_X = std::max(O_MIN_X, 0);
	const std::int32_t MIN_Y = std::max(O_MIN_Y, 0);

	const std::int32_t MAX_X = std::min(O_MAX_X, map.getWidth() - 1);
	const std::int32_t MAX_Y = std::min(O_MAX_Y, map.getHeight() - 1);

	int maxDiameter = 2 * maxRadius + 1;

	auto cellFormFactor = formFactor.begin() + (lx + ly * map.getWidth()) * maxDiameter * maxDiameter;

	// compute the light's contribution
#define MIN_FACTOR (1.0f/255.0f)
	int width = map.getWidth();

	for (int y = MIN_Y, cdy = MIN_Y - O_MIN_Y; y <= MAX_Y; y++, cdy++)
	{
		CellData* cellData = &data[MIN_X + y * width];

		auto cellFormRow = std::next(cellFormFactor, MIN_X - O_MIN_X + cdy * maxDiameter);

		for (int x = MIN_X; x <= MAX_X; x++, cellData++, cellFormRow++)
		{
			float cellff = *cellFormRow;
			if (cellff > MIN_FACTOR)
			{
				cellData->incoming.r += lcol.r * cellff;
				cellData->incoming.g += lcol.g * cellff;
				cellData->incoming.b += lcol.b * cellff;
			}
		}
	}
}

void PhotonShader::propagate()
{
	lightsCoord.clear();

	for (int i = 0; i < data.size(); ++i)
	{
		data[i].emission.r = data[i].incoming.r * reflectivity;
		data[i].emission.g = data[i].incoming.g * reflectivity;
		data[i].emission.b = data[i].incoming.b * reflectivity;
		data[i].outgoing.r += data[i].incoming.r * selfIllumination;
		data[i].outgoing.g += data[i].incoming.g * selfIllumination;
		data[i].outgoing.b += data[i].incoming.b * selfIllumination;
		data[i].incoming.r = 0;
		data[i].incoming.g = 0;
		data[i].incoming.b = 0;

		if (data[i].emission.r > 0 || data[i].emission.g > 0 || data[i].emission.b > 0)
		{
			lightsCoord.push(Coord(i % map.getWidth(), i / map.getWidth()));
		}
	}
}

void PhotonShader::compute()
{
	// turn off all lights
	int size = map.getWidth() * map.getHeight();
	std::fill(data.begin(), data.end(), CellData{});

	// first pass. lights only
	for (Light& l : lights)
	{
		int off = l.coordinate.x + l.coordinate.y * map.getWidth();
		CellData* cellData = &data[off];
		float sum = ffSum[off];
		cellData->emission.r = l.col.r * sum;
		cellData->emission.g = l.col.r * sum;
		cellData->emission.b = l.col.r * sum;
		computeLightContribution(l.coordinate.x, l.coordinate.y, l.radius, cellData->emission, 0.5f);
	}

	// other passes. all lit cells act as lights
	int pass=1;
	while ( pass < nbPass )
	{
		propagate();

		//for (int y=0; y < map.getHeight(); y++ ) {
		//	for (int x=0; x < map.getWidth(); x++, cellData++ ) {
		for (Coord& c : lightsCoord)
		{
			CellData cellData = data.at(c.x + c.y * map.getWidth());

			computeLightContribution(c.x, c.y, maxRadius, cellData.emission, reflectivity);
		}
		pass++;
	}

	propagate();

	for (int c = 0; c < data.size(); ++c)
	{
		lightmap[c].r = (uint8)MIN(255, data[c].outgoing.r);
		lightmap[c].g = (uint8)MIN(255, data[c].outgoing.g);
		lightmap[c].b = (uint8)MIN(255, data[c].outgoing.b);
	}
}

void PhotonShader::calculateShaders()
{
	int size = map.getWidth() * map.getHeight();
	int maxDiameter = 2 * maxRadius + 1;

	// initialize data
	formFactor.resize(size * maxDiameter * maxDiameter);
	std::fill(formFactor.begin(), formFactor.end(), 0.0f);

	ffSum = new float[size];

	data.resize(size);

	// compute form factors
	for (int y = 0; y < map.getHeight(); y++)
	{
		for (int x = 0; x < map.getWidth(); x++)
		{
			computeFormFactor(x, y);
		}
	}
}

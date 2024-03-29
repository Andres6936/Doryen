/*
* Copyright (c) 2009 Jice
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Jice may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Jice ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Jice BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// world generator
// this was mostly generated with libtcod 1.4.2 heightmap tool !

#include <cmath>
#include <cstdio>
#include <cstring>
#include "Main.hpp"
#include "Doryen/Algorithms/Drawing/Bresenham.hpp"
#include "Doryen/Algorithms/Generation/Heightmap.hpp"
#include "UtilWorldgen.hpp"


using namespace Doryen;

// temperature / precipitation Biome diagram (Whittaker diagram)
EBiome biomeDiagram[5][5] = {
		// artic/alpine climate (below -5�C)
		{ TUNDRA,      TUNDRA,       TUNDRA,              TUNDRA,                    TUNDRA, },
		// cold climate (-5 / 5 �C)
		{ COLD_DESERT, GRASSLAND,    BOREAL_FOREST,       BOREAL_FOREST,             BOREAL_FOREST, },
		// temperate climate (5 / 15 �C)
		{ COLD_DESERT, GRASSLAND,    TEMPERATE_FOREST,    TEMPERATE_FOREST,          TROPICAL_MONTANE_FOREST, },
		// warm climate (15 - 20�C)
		{ HOT_DESERT,  SAVANNA,      TROPICAL_DRY_FOREST, TROPICAL_EVERGREEN_FOREST, TROPICAL_EVERGREEN_FOREST, },
		// tropical climate (above 20 �C)
		{ HOT_DESERT,  THORN_FOREST, TROPICAL_DRY_FOREST, TROPICAL_EVERGREEN_FOREST, TROPICAL_EVERGREEN_FOREST, },
};

static const float sandHeight = 0.12f;

static const float grassHeight = 0.16f; //0.315f;
static const float rockHeight = 0.655f;

static const float snowHeight = 0.905f; //0.785f;
// TCOD's land color map
static const int MAX_COLOR_KEY = 10;

#define COLOR_KEY_MAX_SEA ((int)(sandHeight*255)-1)
#define COLOR_KEY_MIN_LAND ((int)(sandHeight*255))

static const int keyIndex[MAX_COLOR_KEY] = { 0,
											 COLOR_KEY_MAX_SEA,
											 COLOR_KEY_MIN_LAND,
											 (int)(grassHeight * 255),
											 (int)(grassHeight * 255) + 10,
											 (int)(rockHeight * 255),
											 (int)(rockHeight * 255) + 10,
											 (int)(snowHeight * 255),
											 (int)(snowHeight * 255) + 10,
											 255
};

static const Doryen::Color keyColor[MAX_COLOR_KEY] = {
		Doryen::Color(0, 0, 50),    // deep water
		Doryen::Color(20, 20, 200), // water-sand transition
		Doryen::Color(134, 180, 101),// sand
		Doryen::Color(80, 120, 10),// sand-grass transition
		Doryen::Color(17, 109, 7), // grass
		Doryen::Color(30, 85, 12), // grass-rock transisiton
		Doryen::Color(64, 70, 20), // rock
		Doryen::Color(120, 140, 40), // rock-snow transisiton
		Doryen::Color(208, 208, 239), // snow
		Doryen::Color(255, 255, 255)
};

// altitude color map
static const int MAX_ALT_KEY = 8;

static const int altIndexes[MAX_ALT_KEY] = {
		0, 15, (int)(sandHeight * 255), (int)(sandHeight * 255) + 1,
		80, 130, 195, 255
};

static const float altitudes[MAX_ALT_KEY] = {
		-2000, -1000, -100, 0, 500, 1000, 2500, 4000    // in meters
};

static const Doryen::Color altColors[MAX_ALT_KEY] = {
		Doryen::Color(24, 165, 255), // -2000
		Doryen::Color(132, 214, 255), // -1000
		Doryen::Color(247, 255, 255), // -100
		Doryen::Color(49, 149, 44), // 0
		Doryen::Color(249, 209, 151), // 500
		Doryen::Color(165, 148, 24), // 1000
		Doryen::Color(153, 110, 6), // 2500
		Doryen::Color(172, 141, 138), // 4000
};

// precipitation color map
static const int MAX_PREC_KEY = 19;

static const int precIndexes[MAX_PREC_KEY] = {
		4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 50, 60, 70, 80, 100, 120, 140, 160, 255
};

static const float precipitations[MAX_PREC_KEY] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 15, 18, 20, 25, 30, 35, 40  // cm / m� / year
};

static const Doryen::Color precColors[MAX_PREC_KEY] = {
		Doryen::Color(128, 0, 0), // < 4
		Doryen::Color(173, 55, 0), // 4-8
		Doryen::Color(227, 102, 0), // 8-12
		Doryen::Color(255, 149, 0), // 12-16
		Doryen::Color(255, 200, 0), // 16-20
		Doryen::Color(255, 251, 0), // 20-24
		Doryen::Color(191, 255, 0), // 24-28
		Doryen::Color(106, 251, 0), // 28-32
		Doryen::Color(25, 255, 48), // 32-36
		Doryen::Color(48, 255, 141), // 36-40
		Doryen::Color(28, 255, 232), // 40-50
		Doryen::Color(54, 181, 255), // 50-60
		Doryen::Color(41, 71, 191), // 60-70
		Doryen::Color(38, 0, 255), // 70-80
		Doryen::Color(140, 0, 255), // 80-100
		Doryen::Color(221, 0, 255), // 100-120
		Doryen::Color(255, 87, 255), // 120-140
		Doryen::Color(255, 173, 255), // 140-160
		Doryen::Color(255, 206, 255), // > 160
};

// temperature color map
static const int MAX_TEMP_KEY = 7;

static const int tempIndexes[MAX_TEMP_KEY] = { 0, 42, 84, 126, 168, 210, 255 };

static const int temperatures[MAX_TEMP_KEY] = { -30, -20, -10, 0, 10, 20, 30 };

static const Doryen::Color tempKeyColor[MAX_TEMP_KEY] = {
		Doryen::Color(180, 8, 130), // -30 �C
		Doryen::Color(32, 1, 139), // -20 �C
		Doryen::Color(0, 65, 252),// -10 �C
		Doryen::Color(37, 255, 236),// 0 �C
		Doryen::Color(255, 255, 1), // 10 �C
		Doryen::Color(255, 29, 4), // 20 �C
		Doryen::Color(80, 3, 0), // 30 �C
};


int WorldGenerator::getWidth() const
{
	return HM_WIDTH;
}

int WorldGenerator::getHeight() const
{
	return HM_HEIGHT;
}

float WorldGenerator::getAltitude(int x, int y) const
{
	return heightmap->getValue(x, y);
}

float WorldGenerator::getRealAltitude(float x, float y) const
{
	int ih = (int)(256 * getInterpolatedAltitude(x, y));
	int idx;
	ih = std::clamp(ih, 0, 255);
	for (idx = 0; idx < MAX_ALT_KEY - 1; idx++)
	{
		if (altIndexes[idx + 1] > ih)
		{ break; }
	}
	float alt = altitudes[idx] + (altitudes[idx + 1] - altitudes[idx]) * (ih - altIndexes[idx]) /
								 (altIndexes[idx + 1] - altIndexes[idx]);
	return alt;
}

float WorldGenerator::getPrecipitations(float x, float y) const
{
	int iprec = (int)(256 * precipitation->getValue((int)x, (int)y));
	int idx;
	iprec = std::clamp(iprec, 0, 255);
	for (idx = 0; idx < MAX_PREC_KEY - 1; idx++)
	{
		if (precIndexes[idx + 1] > iprec)
		{ break; }
	}
	float prec = precipitations[idx] + (precipitations[idx + 1] - precipitations[idx]) * (iprec - precIndexes[idx]) /
									   (precIndexes[idx + 1] - precIndexes[idx]);
	return prec;
}

float WorldGenerator::getTemperature(float x, float y) const
{
	return temperature->getValue((int)x, (int)y);
}

EBiome WorldGenerator::getBiome(float x, float y) const
{
	return biomeMap[(int)x + (int)y * HM_WIDTH];
}

float WorldGenerator::getInterpolatedAltitude(float x, float y) const
{
	return heightmap->getInterpolatedValue(x, y);
}

void WorldGenerator::getInterpolatedNormal(float x, float y, float n[3]) const
{
	return heightmapWithoutErosion->getNormal(x, y, n, sandHeight);
}

float WorldGenerator::getSandHeight() const
{
	return sandHeight;
}

bool WorldGenerator::isOnSea(float x, float y) const
{
	return getInterpolatedAltitude(x, y) <= sandHeight;
}

void WorldGenerator::addHill(int nbHill, float baseRadius, float radiusVar, float height)
{
	for (int i = 0; i < nbHill; i++)
	{
		float hillMinRadius = baseRadius * (1.0f - radiusVar);
		float hillMaxRadius = baseRadius * (1.0f + radiusVar);
		float radius = Random::Number::nextFloat(hillMinRadius, hillMaxRadius);
		int xh = Random::Number::nextInteger(0, HM_WIDTH - 1);
		int yh = Random::Number::nextInteger(0, HM_HEIGHT - 1);
		heightmap->addHill((float)xh, (float)yh, radius, height);
	}
}

void WorldGenerator::setLandMass(float landMass, float waterLevel)
{
	// fix land mass. We want a proportion of landMass above sea level
#ifndef NDEBUG
	float t0 = console.getElapsedSeconds();
#endif
	int heightcount[256];
	std::memset(heightcount, 0, sizeof(heightcount));
	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			float h = heightmap->getValue(x, y);
			int ih = (int)(h * 255);
			ih = std::clamp(ih, 0, 255);
			heightcount[ih]++;
		}
	}
	int i = 0, totalcount = 0;
	while (totalcount < HM_WIDTH * HM_HEIGHT * (1.0f - landMass))
	{
		totalcount += heightcount[i];
		i++;
	}
	float newWaterLevel = i / 255.0f;
	float landCoef = (1.0f - waterLevel) / (1.0f - newWaterLevel);
	float waterCoef = waterLevel / newWaterLevel;
	// water level should be rised/lowered to newWaterLevel
	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			float h = heightmap->getValue(x, y);
			if (h > newWaterLevel)
			{
				h = waterLevel + (h - newWaterLevel) * landCoef;
			}
			else
			{
				h = h * waterCoef;
			}
			heightmap->setValue(x, y, h);
		}
	}
#ifndef NDEBUG
	float t1 = console.getElapsedSeconds();
	printf("  Landmass... %g\n", t1 - t0);
#endif
}

void WorldGenerator::buildBaseMap()
{
	float timeStart = console.getElapsedSeconds();

	addHill(600, 16.0 * HM_WIDTH / 200, 0.7, 0.3);
	heightmap->normalize();
	float timeEnd = console.getElapsedSeconds();
	printf("\tHills... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	heightmap->addFbm(noise, 2.20 * HM_WIDTH / 400, 2.20 * HM_WIDTH / 400, 0, 0, 10.0f, 1.0, 2.05);
	heightmap->normalize();
	heightmapWithoutErosion->copy(heightmap);
	timeEnd = console.getElapsedSeconds();
	printf("\tFbm... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	setLandMass(0.6f, sandHeight);

	// Fix land/mountain ratio using x^3 curve above sea level
	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			float h = heightmap->getValue(x, y);
			if (h >= sandHeight)
			{
				float coef = (h - sandHeight) / (1.0f - sandHeight);
				h = sandHeight + coef * coef * coef * (1.0f - sandHeight);
				heightmap->setValue(x, y, h);
			}
		}
	}

	timeEnd = console.getElapsedSeconds();
	printf("\tFlatten plains... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;


	// Compute Clouds.
	std::array<float, 2> f{};

	for (int x = 0; x < HM_WIDTH; x++)
	{
		f[0] = 6.0f * ((float)(x) / HM_WIDTH);

		for (int y = 0; y < HM_HEIGHT; y++)
		{
			f[1] = 6.0f * ((float)(y) / HM_HEIGHT);
			noise->setType(TypeNoise::Simplex);
			clouds[x][y] = 0.5f * (1.0f + 0.8f * noise->getFbm(f, 4.0f));
		}
	}

	timeEnd = console.getElapsedSeconds();
	printf("\tInit clouds... %g\n", timeEnd - timeStart);
}

// function blurring the heightmap
void WorldGenerator::smoothMap()
{
	// 3x3 kernel for smoothing operations
	static const int smoothKernelSize = 9;
	static const int smoothKernelDx[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	static const int smoothKernelDy[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	static const float smoothKernelWeight[9] = { 2, 8, 2, 8, 20, 8, 2, 8, 2 };

#ifndef NDEBUG
	float t0 = console.getElapsedSeconds();
#endif
	heightmap->kernelTransform(smoothKernelSize, smoothKernelDx, smoothKernelDy, smoothKernelWeight,
			-1000, 1000);
	heightmapWithoutErosion->kernelTransform(smoothKernelSize, smoothKernelDx, smoothKernelDy,
			smoothKernelWeight,
			-1000, 1000);
	heightmap->normalize();
#ifndef NDEBUG
	float t1 = console.getElapsedSeconds();
	printf("  Blur... %g\n", t1 - t0);
#endif
}

static const int dirx[9] = { 0, -1, 0, 1, -1, 1, -1, 0, 1 };

static const int diry[9] = { 0, -1, -1, -1, 0, 0, 1, 1, 1 };

static const float dircoef[9] = { 1.0f, 1.0f / 1.414f, 1.0f, 1.0f / 1.414f, 1.0f, 1.0f, 1.0f / 1.414f, 1.0f,
								  1.0f / 1.414f };

static const int oppdir[9] = { 0, 8, 7, 6, 5, 4, 3, 2, 1 };

// erosion parameters
#define EROSION_FACTOR 0.01f
#define SEDIMENTATION_FACTOR 0.01f
#define MAX_EROSION_ALT 0.9f
#define MUDSLIDE_COEF 0.4f

void WorldGenerator::erodeMap()
{
	Doryen::Heightmap newMap(HM_WIDTH, HM_HEIGHT);
	for (int i = 5; i != 0; i--)
	{
		// compute flow and slope maps
		MapData* md = mapData;
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			for (int x = 0; x < HM_WIDTH; x++)
			{
				float h = heightmap->getValue(x, y);
				float hmin = h, hmax = h;
				int minDir = 0, maxDir = 0;
				for (int i = 1; i < 9; i++)
				{
					int ix = x + dirx[i];
					int iy = y + diry[i];
					if (IN_RECTANGLE(ix, iy, HM_WIDTH, HM_HEIGHT))
					{
						float h2 = heightmap->getValue(ix, iy);
						if (h2 < hmin)
						{
							hmin = h2;
							minDir = i;
						}
						else if (h2 > hmax)
						{
							hmax = h2;
							maxDir = i;
						}
					}
				}
				md->flowDir = minDir;
				float slope = hmin - h; // this is negative
				slope *= dircoef[minDir];
				md->slope = slope;
				md++;
			}
		}

		md = mapData;
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			for (int x = 0; x < HM_WIDTH; x++)
			{
				float sediment = 0.0f;
				bool end = false;
				int ix = x, iy = y;
				uint8 oldFlow = md->flowDir;
				MapData* md2 = md;
				while (!end)
				{
					float h = heightmap->getValue(ix, iy);
					if (h < sandHeight - 0.01f)
					{ break; }
					if (md2->flowDir == oppdir[oldFlow])
					{
						h += SEDIMENTATION_FACTOR * sediment;
						heightmap->setValue(ix, iy, h);
						end = true;
					}
					else
					{
						// remember, slope is negative
						h += precipitation->getValue(ix, iy) * EROSION_FACTOR * md2->slope;
						h = std::max(h, sandHeight);
						sediment -= md2->slope;
						heightmap->setValue(ix, iy, h);
						oldFlow = md2->flowDir;
						ix += dirx[oldFlow];
						iy += diry[oldFlow];
						md2 = &mapData[ix + iy * HM_WIDTH];
					}
				}
				md++;
			}
		}
		printf("  Erosion pass %d\n", i);

		// mudslides (smoothing)
		float sandCoef = 1.0f / (1.0f - sandHeight);
		for (int x = 0; x < HM_WIDTH; x++)
		{
			for (int y = 0; y < HM_HEIGHT; y++)
			{
				float h = heightmap->getValue(x, y);
				if (h < sandHeight - 0.01f || h >= MAX_EROSION_ALT)
				{
					newMap.setValue(x, y, h);
					continue;
				}
				float sumDelta1 = 0.0f, sumDelta2 = 0.0f;
				int nb1 = 1, nb2 = 1;
				for (int i = 1; i < 9; i++)
				{
					int ix = x + dirx[i];
					int iy = y + diry[i];
					if (IN_RECTANGLE(ix, iy, HM_WIDTH, HM_HEIGHT))
					{
						float ih = heightmap->getValue(ix, iy);
						if (ih < h)
						{
							if (i == 1 || i == 3 || i == 6 || i == 8)
							{
								// diagonal neighbour
								sumDelta1 += (ih - h) * 0.4f;
								nb1++;
							}
							else
							{
								// adjacent neighbour
								sumDelta2 += (ih - h) * 1.6f;
								nb2++;
							}
						}
					}
				}
				// average height difference with lower neighbours
				float dh = sumDelta1 / nb1 + sumDelta2 / nb2;
				dh *= MUDSLIDE_COEF;
				float hcoef = (h - sandHeight) * sandCoef;
				dh *= (1.0f - hcoef * hcoef * hcoef); // less smoothing at high altitudes

				newMap.setValue(x, y, h + dh);
			}
		}
		heightmap->copy(&newMap);
	}

}

// interpolated cloud thickness
float WorldGenerator::getCloudThickness(float x, float y) const
{
	x += cloudDx;
	int ix = (int)x;
	int iy = (int)y;
	int ix1 = std::min(HM_WIDTH - 1, ix + 1);
	int iy1 = std::min(HM_HEIGHT - 1, iy + 1);
	float fdx = x - ix;
	float fdy = y - iy;
	float v1 = clouds[ix][iy];
	float v2 = clouds[ix1][iy];
	float v3 = clouds[ix][iy1];
	float v4 = clouds[ix1][iy1];
	float vx1 = ((1.0f - fdx) * v1 + fdx * v2);
	float vx2 = ((1.0f - fdx) * v3 + fdx * v4);
	float v = ((1.0f - fdy) * vx1 + fdy * vx2);
	return v;
}

Doryen::Color WorldGenerator::getMapColor(float h)
{
	int colorIdx;
	if (h < sandHeight)
	{ colorIdx = (int)(h / sandHeight * COLOR_KEY_MAX_SEA); }
	else
	{ colorIdx = COLOR_KEY_MIN_LAND + (int)((h - sandHeight) / (1.0f - sandHeight) * (255 - COLOR_KEY_MIN_LAND)); }
	colorIdx = std::clamp(colorIdx, 0, 255);
	return mapGradient[colorIdx];
}

void WorldGenerator::computeSunLight(float lightDir[3])
{
	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			worldint[x + y * HM_WIDTH] = getMapIntensity(x + 0.5f, y + 0.5f, lightDir);
		}
	}
}


float WorldGenerator::getMapIntensity(float worldX, float worldY, float lightDir[3])
{
	// sun color & direction
	static const Doryen::Color sunCol(255, 255, 160);
	float normal[3];
	float wx = std::clamp(worldX, 0.0f, (float)HM_WIDTH - 1);
	float wy = std::clamp(worldY, 0.0f, (float)HM_HEIGHT - 1);
	// apply sun light
	getInterpolatedNormal(wx, wy, normal);
	normal[2] *= 3.0f;
	float intensity = 0.75f
					  - (normal[0] * lightDir[0] + normal[1] * lightDir[1] + normal[2] * lightDir[2]) * 0.75f;
	intensity = std::clamp(intensity, 0.75f, 1.5f);
	return intensity;
}

Doryen::Color WorldGenerator::getInterpolatedColor(float worldX, float worldY)
{
	return getInterpolatedColor(&imageWorldmap, worldX, worldY);
}

Doryen::Color WorldGenerator::getInterpolatedColor(Doryen::Image* img, float x, float y)
{
	const auto[w, h] = img->getSize().unpack();
	float wx = std::clamp(x, 0.0f, (float)w - 1);
	float wy = std::clamp(y, 0.0f, (float)h - 1);
	int iwx = (int)wx;
	int iwy = (int)wy;
	float dx = wx - iwx;
	float dy = wy - iwy;

	Doryen::Color colNW = img->getPixel(iwx, iwy);
	Doryen::Color colNE = (iwx < w - 1 ? img->getPixel(iwx + 1, iwy) : colNW);
	Doryen::Color colSW = (iwy < h - 1 ? img->getPixel(iwx, iwy + 1) : colNW);
	Doryen::Color colSE = (iwx < w - 1 && iwy < h - 1 ? img->getPixel(iwx + 1, iwy + 1) : colNW);
	Doryen::Color colN = Doryen::Color::lerp(colNW, colNE, dx);
	Doryen::Color colS = Doryen::Color::lerp(colSW, colSE, dx);
	Doryen::Color col = Doryen::Color::lerp(colN, colS, dy);
	return col;
}


float WorldGenerator::getInterpolatedIntensity(float worldX, float worldY)
{
	return getInterpolatedFloat(worldint, worldX, worldY, HM_WIDTH, HM_HEIGHT);
}

void WorldGenerator::updateClouds(float elapsedTime)
{
	cloudTotalDx += elapsedTime * 5;
	cloudDx += elapsedTime * 5;
	if (cloudDx >= 1.0f)
	{
		int colsToTranslate = (int)cloudDx;
		cloudDx -= colsToTranslate;
		// translate the cloud map
		for (int x = colsToTranslate; x < HM_WIDTH; x++)
		{
			for (int y = 0; y < HM_HEIGHT; y++)
			{
				clouds[x - colsToTranslate][y] = clouds[x][y];
			}
		}
		// compute a new column
		std::array<float, 2> f{};
		float cdx = (int)cloudTotalDx;
		for (int x = HM_WIDTH - colsToTranslate; x < HM_WIDTH; x++)
		{
			for (int y = 0; y < HM_HEIGHT; y++)
			{
				f[0] = 6.0f * ((float)(x + cdx) / HM_WIDTH);
				f[1] = 6.0f * ((float)(y) / HM_HEIGHT);
				// clouds[x][y] = 0.5f * (1.0f + 0.8f * noise->getFbmSimplex(f,4.0f));
				noise->setType(TypeNoise::Simplex);
				clouds[x][y] = 0.5f * (1.0f + 0.8f * noise->getFbm(f, 4.0f));
			}
		}
	}
}

void WorldGenerator::generateRivers()
{
	static int riverId = 0;
	// the source
	int sx, sy;
	// the destination
	int dx, dy;
	// get a random point near the coast
	sx = Random::Number::nextInteger(0, HM_WIDTH - 1);
	sy = Random::Number::nextInteger(HM_HEIGHT / 5, 4 * HM_HEIGHT / 5);
	float h = heightmap->getValue(sx, sy);
	while (h < sandHeight - 0.02 || h >= sandHeight)
	{
		sx++;
		if (sx == HM_WIDTH)
		{
			sx = 0;
			sy++;
			if (sy == HM_HEIGHT)
			{ sy = 0; }
		}
		h = heightmap->getValue(sx, sy);
	}
	std::vector<int> tree;
	std::vector<int> randPt;
	tree.push_back(sx + sy * HM_WIDTH);
	riverId++;
	dx = sx;
	dy = sy;
	for (int i = 0; i < Random::Number::nextInteger(50, 200); i++)
	{
		int rx = Random::Number::nextInteger(sx - 200, sx + 200);
		int ry = Random::Number::nextInteger(sy - 200, sy + 200);
//	    if ( IN_RECTANGLE(rx,ry,HM_WIDTH,HM_HEIGHT) ) {
//	        float h=heightmap->getValue(rx,ry);
//	        if ( h >= sandHeight ) {
		randPt.push_back(rx + ry * HM_WIDTH);
//	        }
//	    }
	}
	for (int i = 0; i < randPt.size(); i++)
	{
		int rx = randPt.at(i) % HM_WIDTH;
		int ry = randPt.at(i) / HM_WIDTH;

		float minDist = 1E10;
		int bestx = -1, besty = -1;
		for (int j = 0; j < tree.size(); j++)
		{
			int tx = tree.at(j) % HM_WIDTH;
			int ty = tree.at(j) / HM_WIDTH;
			float dist = (tx - rx) * (tx - rx) + (ty - ry) * (ty - ry);
			if (dist < minDist)
			{
				minDist = dist;
				bestx = tx;
				besty = ty;
			}
		}

		Doryen::Algorithms::Line objLine = Doryen::Algorithms::Line();
		objLine.init(bestx, besty, rx, ry);

		int len = 3, cx = bestx, cy = besty;
		MapData* md = &mapData[cx + cy * HM_WIDTH];
		if (md->riverId == riverId)
		{ md->riverId = 0; }
		do
		{
			md = &mapData[cx + cy * HM_WIDTH];
			if (md->riverId > 0)
			{ return; }
			float h = heightmap->getValue(cx, cy);
			if (h >= sandHeight)
			{
				md->riverId = riverId;
				precipitation->setValue(cx, cy, 1.0f);
			}
			if (cx == 0 || cx == HM_WIDTH - 1 || cy == 0 || cy == HM_HEIGHT - 1)
			{ len = 0; }
			else if (objLine.step(&cx, &cy))
			{ len = 0; }
			len--;
		} while (len > 0);
		int newNode = cx + cy * HM_WIDTH;
		if (newNode != bestx + besty * HM_WIDTH)
		{
			tree.push_back(newNode);
		}
	}
}

EClimate WorldGenerator::getClimateFromTemp(float temp)
{
	if (temp <= -5)
	{ return ARTIC_ALPINE; }
	if (temp <= 5)
	{ return COLD; }
	if (temp <= 15)
	{ return TEMPERATE; }
	if (temp <= 20)
	{ return WARM; }
	return TROPICAL;
}

float WorldGenerator::getInterpolatedFloat(float* arr, float x, float y, int width, int height)
{
	float wx = std::clamp(x, 0.0f, (float)width - 1);
	float wy = std::clamp(y, 0.0f, (float)height - 1);
	int iwx = (int)wx;
	int iwy = (int)wy;
	float dx = wx - iwx;
	float dy = wy - iwy;

	float iNW = arr[iwx + iwy * width];
	float iNE = (iwx < width - 1 ? arr[iwx + 1 + iwy * width] : iNW);
	float iSW = (iwy < height - 1 ? arr[iwx + (iwy + 1) * width] : iNW);
	float iSE = (iwx < width - 1 && iwy < height - 1 ? arr[iwx + 1 + (iwy + 1) * width] : iNW);
	float iN = (1.0f - dx) * iNW + dx * iNE;
	float iS = (1.0f - dx) * iSW + dx * iSE;
	return (1.0f - dy) * iN + dy * iS;
}

void WorldGenerator::computePrecipitations()
{
	static const float waterAdd = 0.03f;
	static const float slopeCoef = 2.0f;
	static const float basePrecip = 0.01f; // precipitation coef when slope == 0

	float timeStart = console.getElapsedSeconds();

	// north/south winds
	for (int diry = -1; diry <= 1; diry += 2)
	{
		for (int x = 0; x < HM_WIDTH; x++)
		{
			std::array<float, 1> noisex = { (float)(x) * 5 / HM_WIDTH };
			// float waterAmount=(1.0f+noise1d.getFbmSimplex(&noisex,3.0f));
			noise1d.setType(TypeNoise::Simplex);
			float waterAmount = (1.0f + noise1d.getFbm(noisex, 3.0f));
			int starty = (diry == -1 ? HM_HEIGHT - 1 : 0);
			int endy = (diry == -1 ? -1 : HM_HEIGHT);
			for (int y = starty; y != endy; y += diry)
			{
				float h = heightmap->getValue(x, y);
				if (h < sandHeight)
				{
					waterAmount += waterAdd;
				}
				else if (waterAmount > 0.0f)
				{
					float slope;
					if ((unsigned)(y + diry) < (unsigned)HM_HEIGHT)
					{ slope = heightmap->getValue(x, y + diry) - h; }
					else
					{ slope = h - heightmap->getValue(x, y - diry); }
					if (slope >= 0.0f)
					{
						float precip = waterAmount * (basePrecip + slope * slopeCoef);
						precipitation->setValue(x, y, precipitation->getValue(x, y) + precip);
						waterAmount -= precip;
						waterAmount = std::max(0.0f, waterAmount);
					}
				}
			}
		}
	}

	float timeEnd = console.getElapsedSeconds();
	printf("\tNorth/south winds... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	// east/west winds
	for (int dirx = -1; dirx <= 1; dirx += 2)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			std::array<float, 1> noisey = { (float)(y) * 5 / HM_HEIGHT };
			// float waterAmount=(1.0f+noise1d.getFbmSimplex(&noisey,3.0f));
			noise1d.setType(TypeNoise::Simplex);
			float waterAmount = (1.0f + noise1d.getFbm(noisey, 3.0f));
			int startx = (dirx == -1 ? HM_WIDTH - 1 : 0);
			int endx = (dirx == -1 ? -1 : HM_WIDTH);
			for (int x = startx; x != endx; x += dirx)
			{
				float h = heightmap->getValue(x, y);
				if (h < sandHeight)
				{
					waterAmount += waterAdd;
				}
				else if (waterAmount > 0.0f)
				{
					float slope;
					if ((unsigned)(x + dirx) < (unsigned)HM_WIDTH)
					{ slope = heightmap->getValue(x + dirx, y) - h; }
					else
					{ slope = h - heightmap->getValue(x - dirx, y); }
					if (slope >= 0.0f)
					{
						float precip = waterAmount * (basePrecip + slope * slopeCoef);
						precipitation->setValue(x, y, precipitation->getValue(x, y) + precip);
						waterAmount -= precip;
						waterAmount = std::max(0.0f, waterAmount);
					}
				}
			}
		}
	}
	timeEnd = console.getElapsedSeconds();
	printf("\tEast/west winds... %g\n", timeEnd - timeStart);
	timeStart = timeEnd;

	float min, max;
	precipitation->getMinMax(&min, &max);

	// latitude impact
	for (int y = HM_HEIGHT / 4; y < 3 * HM_HEIGHT / 4; y++)
	{
		// latitude (0 : equator, -1/1 : pole)
		float lat = (float)(y - HM_HEIGHT / 4) * 2 / HM_HEIGHT;
		float coef = sinf(2 * 3.1415926 * lat);
		for (int x = 0; x < HM_WIDTH; x++)
		{
			std::array<float, 2> f = { (float)(x) / HM_WIDTH, (float)(y) / HM_HEIGHT };
			// float xcoef = coef + 0.5f*noise2d.getFbmSimplex(f,3.0f);
			noise2d.setType(TypeNoise::Simplex);
			float xcoef = coef + 0.5f * noise2d.getFbm(f, 3.0f);
			float precip = precipitation->getValue(x, y);
			precip += (max - min) * xcoef * 0.1f;
			precipitation->setValue(x, y, precip);
		}
	}
	timeEnd = console.getElapsedSeconds();
	printf("\tlatitude... %g\n", timeEnd - timeStart);
	timeStart = timeEnd;

	// very fast blur by scaling down and up
	static const int factor = 8;
	static const int smallWidth = (HM_WIDTH + factor - 1) / factor;
	static const int smallHeight = (HM_HEIGHT + factor - 1) / factor;
	float* lowResMap = new float[smallWidth * smallHeight];
	std::memset(lowResMap, 0, sizeof(float) * smallWidth * smallHeight);

	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			float v = precipitation->getValue(x, y);
			int ix = x / factor;
			int iy = y / factor;
			lowResMap[ix + iy * smallWidth] += v;
		}
	}

	float coef = 1.0f / factor;

	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			float v = getInterpolatedFloat(lowResMap, x * coef, y * coef, smallWidth, smallHeight);
			precipitation->setValue(x, y, v);
		}
	}

	delete[] lowResMap;
}

void WorldGenerator::smoothPrecipitations()
{
	float t0 = console.getElapsedSeconds();

	// better quality polishing blur using a 5x5 kernel
	// faster than TCODHeightmap kernelTransform function
	Doryen::Heightmap temphm(HM_WIDTH, HM_HEIGHT);
	temphm.copy(precipitation);
	for (int i = 4; i != 0; i--)
	{
		for (int x = 0; x < HM_WIDTH; x++)
		{
			int minx = x - 2;
			int maxx = x + 2;
			int miny = 0;
			int maxy = 2;
			float sum = 0.0f;
			int count = 0;
			minx = std::max(0, minx);
			maxx = std::min(HM_WIDTH - 1, maxx);
			// compute the kernel sum at x,0
			for (int ix = minx; ix <= maxx; ix++)
			{
				for (int iy = miny; iy <= maxy; iy++)
				{
					sum += precipitation->getValue(ix, iy);
					count++;
				}
			}
			temphm.setValue(x, 0, sum / count);
			for (int y = 1; y < HM_HEIGHT; y++)
			{
				if (y - 2 >= 0)
				{
					// remove the top-line sum
					for (int ix = minx; ix <= maxx; ix++)
					{
						sum -= precipitation->getValue(ix, y - 2);
						count--;
					}
				}
				if (y + 2 < HM_HEIGHT)
				{
					// add the bottom-line sum
					for (int ix = minx; ix <= maxx; ix++)
					{
						sum += precipitation->getValue(ix, y + 2);
						count++;
					}
				}
				temphm.setValue(x, y, sum / count);
			}
		}
	}
	precipitation->copy(&temphm);

	float t1 = console.getElapsedSeconds();
	printf("  Blur... %g\n", t1 - t0);
	t0 = t1;

	precipitation->normalize();
	t1 = console.getElapsedSeconds();
	printf("  Normalization... %g\n", t1 - t0);
	t0 = t1;

}

void WorldGenerator::computeTemperaturesAndBiomes()
{
	// temperature shift with altitude : -25�C at 6000 m
	// mean temp at sea level : 25�C at lat 0  5�C at lat 45 -25�C at lat 90 (sinusoide)
	float sandCoef = 1.0f / (1.0f - sandHeight);
	float waterCoef = 1.0f / sandHeight;
	for (int y = 0; y < HM_HEIGHT; y++)
	{
		float lat = (float)(y - HM_HEIGHT / 2) * 2 / HM_HEIGHT;
		float latTemp =
				0.5f * (1.0f + std::pow(sin(3.1415926 * (lat + 0.5f)), 5)); // between 0 and 1
		if (latTemp > 0.0f)
		{ latTemp = sqrt(latTemp); }
		latTemp = -30 + latTemp * 60;
		for (int x = 0; x < HM_WIDTH; x++)
		{
			float h0 = heightmap->getValue(x, y);
			float h = h0 - sandHeight;
			if (h < 0.0f)
			{ h *= waterCoef; }
			else
			{ h *= sandCoef; }
			float altShift = -35 * h;
			float temp = latTemp + altShift;
			temperature->setValue(x, y, temp);
			float humid = precipitation->getValue(x, y);
			// compute biome
			EClimate climate = getClimateFromTemp(temp);
			int iHumid = (int)(humid * 5);
			iHumid = std::min(4, iHumid);
			EBiome biome = biomeDiagram[climate][iHumid];
			biomeMap[x + y * HM_WIDTH] = biome;
		}
	}
	float min, max;
	temperature->getMinMax(&min, &max);
	printf("Temperatures min/max: %g / %g\n", min, max);
}

Doryen::Color WorldGenerator::getBiomeColor(EBiome biome, int x, int y)
{
	static const Doryen::Color biomeColors[] = {
			// TUNDRA,
			Doryen::Color(200, 240, 255),
			// COLD_DESERT,
			Doryen::Color(180, 210, 210),
			// GRASSLAND,
			Doryen::Palette::CYAN,
			// BOREAL_FOREST,
			Doryen::Color(14, 93, 43),
			// TEMPERATE_FOREST,
			Doryen::Color(44, 177, 83),
			// TROPICAL_MONTANE_FOREST,
			Doryen::Color(185, 232, 164),
			// HOT_DESERT,
			Doryen::Color(235, 255, 210),
			// SAVANNA,
			Doryen::Color(255, 205, 20),
			// TROPICAL_DRY_FOREST,
			Doryen::Color(60, 130, 40),
			// TROPICAL_EVERGREEN_FOREST,
			Doryen::Palette::GREEN,
			// THORN_FOREST,
			Doryen::Color(192, 192, 112),
	};
	int r = 0, g = 0, b = 0, count = 1;
	r += biomeColors[biome].getRed();
	g += biomeColors[biome].g;
	b += biomeColors[biome].b;
	for (int i = 0; i < 4; i++)
	{
		int ix = x + Random::Number::nextInteger(-10, 10);
		int iy = y + Random::Number::nextInteger(-10, 10);
		if (IN_RECTANGLE(ix, iy, HM_WIDTH, HM_HEIGHT))
		{
			Doryen::Color c = biomeColors[biomeMap[ix + iy * HM_WIDTH]];
			r += c.getRed() + Random::Number::nextInteger(-10, 10);
			g += c.g + Random::Number::nextInteger(-10, 10);
			b += c.b + Random::Number::nextInteger(-10, 10);
			count++;
		}
	}
	r /= count;
	g /= count;
	b /= count;
	r = std::clamp(r, 0, 255);
	g = std::clamp(g, 0, 255);
	b = std::clamp(b, 0, 255);
	return Doryen::Color(r, g, b);
}

void WorldGenerator::computeColors()
{
	// alter map color using temperature & precipitation maps
	MapData* md = mapData;
	for (int y = 0; y < HM_HEIGHT; y++)
	{
		for (int x = 0; x < HM_WIDTH; x++)
		{
			float h = heightmap->getValue(x, y);
			float temp = temperature->getValue(x, y);
			EBiome biome = biomeMap[x + y * HM_WIDTH];
			Doryen::Color c;
			if (h < sandHeight)
			{ c.copy(getMapColor(h)); }
			else
			{
				c.copy(getMapColor(h));
				c.copy(Doryen::Color::lerp(c, getBiomeColor(biome, x, y), 0.5f));
			}

			// snow near poles
			temp += 10 * (clouds[HM_WIDTH - 1 - x][HM_HEIGHT - 1 - y]); // cheap 2D noise ;)
			if (temp < -10.0f && h < sandHeight)
			{
				imageWorldmap.setPixel(x, y,
						Doryen::Color::lerp(Doryen::Palette::GRAY_WARN_1, c,
								0.3f));
			}
			else if (temp < -8.0f && h < sandHeight)
			{
				imageWorldmap.setPixel(x, y,
						Doryen::Color::lerp(Doryen::Palette::GRAY_WARN_1, c,
								0.3f + 0.7f * (10.0f +
											   temp) /
									   2.0f));
			}
			else if (temp < -2.0f && h >= sandHeight)
			{ imageWorldmap.setPixel(x, y, Doryen::Palette::GRAY_WARN_1); }
			else if (temp < 2.0f && h >= sandHeight)
			{
				//Doryen::Color snow = mapGradient[(int)(snowHeight*255) + (int)((255 - (int)(snowHeight*255)) * (0.6f-temp)/0.4f)];
				c.copy(Doryen::Color::lerp(Doryen::Palette::GRAY_WARN_1, c, (temp + 2) / 4.0f));
				imageWorldmap.setPixel(x, y, c);
			}
			else
			{
				imageWorldmap.setPixel(x, y, c);
			}
			md++;
		}
	}

	md = mapData;
	for (int y = 0; y < HM_HEIGHT; y++)
	{
		for (int x = 0; x < HM_WIDTH; x++)
		{
			if (md->riverId > 0)
			{
				Doryen::Color c = imageWorldmap.getPixel(x, y);
				c.copy(Doryen::Color::lerp(c, Doryen::Palette::BLUE, 0.3f));
				imageWorldmap.setPixel(x, y, c);
			}
			md++;
		}
	}
	// blur
	static const int dx[] = { 0, -1, 0, 1, 0 };
	static const int dy[] = { 0, 0, -1, 0, 1 };
	static const int coef[] = { 1, 2, 2, 2, 2 };
	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			int r = 0, g = 0, b = 0, count = 0;
			for (int i = 0; i < 5; i++)
			{
				int ix = x + dx[i];
				int iy = y + dy[i];
				if (IN_RECTANGLE(ix, iy, HM_WIDTH, HM_HEIGHT))
				{
					Doryen::Color c = imageWorldmap.getPixel(ix, iy);
					r += coef[i] * c.getRed();
					g += coef[i] * c.g;
					b += coef[i] * c.b;
					count += coef[i];
				}
			}
			r /= count;
			g /= count;
			b /= count;
			imageWorldmap.setPixel(x, y, Doryen::Color(r, g, b));
		}
	}
	drawCoasts(&imageWorldmap);
}

/**
 * @brief Generate a smooth color map.
 *
 * You can define a color map from an array of color keys. Colors will be interpolated <br>
 * between the keys.
 *
 * 0 -> black
 * 4 -> red
 * 8 -> white
 *
 * Result :
 *
 * <table>
 * 	<tbody>
 * 		<tr><td>map[0]</td><td style="background-color: rgb(0, 0, 0); width: 50px;" /><td>black</td></tr>
 * 		<tr><td>map[1]</td><td style="background-color: rgb(63, 0, 0);" /></tr>
 * 		<tr><td>map[2]</td><td style="background-color: rgb(127, 0, 0);" /></tr>
 * 		<tr><td>map[3]</td><td style="background-color: rgb(191, 0, 0);" /></tr>
 * 		<tr><td>map[4]</td><td style="background-color: rgb(255, 0, 0);" /><td>red</td></tr>
 * 		<tr><td>map[5]</td><td style="background-color: rgb(255, 63, 63);" /></tr>
 * 		<tr><td>map[6]</td><td style="background-color: rgb(255, 127, 127);" /></tr>
 * 		<tr><td>map[7]</td><td style="background-color: rgb(255, 191, 191);" /></tr>
 * 		<tr><td>map[8]</td><td style="background-color: rgb(255, 255, 255);" /><td>white</td></tr>
 * 	</tbody>
 * </table>
 *
 * If you want to fill the map array, keyIndex[0] must be 0 and keyIndex[nbKey-1] <br>
 * is the number of elements in map minus 1 but you can also use the function to <br>
 * fill only a part of the map array.
 *
 * @param map	An array of colors to be filled by the function.
 * @param nbKey	Number of color keys
 * @param keyColor	Array of nbKey colors containing the color of each key
 * @param keyIndex	Array of nbKey integers containing the index of each key.
 *
 * @example
 * @code
 * int idx[] = { 0, 4, 8 }; // indexes of the keys
 *
 * // Colors : Black, Red, White
 * Doryen::TCODColor col[] = { {0, 0, 0}, {255, 0, 0}, {255, 255, 255} };
 * Doryen::TCODColor map[9];
 * Doryen::TCODColor::genMap(map,3,col,idx);
 * @endcode
 */
void generateSmoothColorMap(Color* map, int nbKey, Color const* keyColor, int const* keyIndex)
{
	for (int segment = 0; segment < nbKey - 1; segment++)
	{
		int idxStart = keyIndex[segment];
		int idxEnd = keyIndex[segment + 1];
		int idx;
		for (idx = idxStart; idx <= idxEnd; idx++)
		{
			map[idx].copy(Color::lerp(keyColor[segment], keyColor[segment + 1],
					(float)(idx - idxStart) / (idxEnd - idxStart)));
		}
	}
}

void WorldGenerator::generate()
{
	float timeTotalStart = console.getElapsedSeconds();
	float timeStart = console.getElapsedSeconds();

	generateSmoothColorMap(mapGradient, MAX_COLOR_KEY, keyColor, keyIndex);

	noise = new Noise<2>();

	float timeEnd = console.getElapsedSeconds();
	printf("Initialization... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	buildBaseMap();
	timeEnd = console.getElapsedSeconds();
	printf("Heightmap construction... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	computePrecipitations();
	timeEnd = console.getElapsedSeconds();
	printf("Precipitation map... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	erodeMap();
	timeEnd = console.getElapsedSeconds();
	printf("Erosion... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	smoothMap();
	timeEnd = console.getElapsedSeconds();
	printf("Smooth... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	setLandMass(0.6f, sandHeight);

	for (int i = 0; i < HM_WIDTH * HM_HEIGHT / 3000; i++)
	{
		generateRivers();
	}

	timeEnd = console.getElapsedSeconds();
	printf("Rivers... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	smoothPrecipitations();
	timeEnd = console.getElapsedSeconds();
	printf("Smooth precipitations... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	computeTemperaturesAndBiomes();
	timeEnd = console.getElapsedSeconds();
	printf("Temperature map... %g\n", timeEnd - timeStart);

	timeStart = timeEnd;

	computeColors();
	timeEnd = console.getElapsedSeconds();
	printf("Color map... %g\n", timeEnd - timeStart);

	timeEnd = console.getElapsedSeconds();
	printf("TOTAL TIME... %g\n", timeEnd - timeTotalStart);
}

void WorldGenerator::drawCoasts(Doryen::Image* img)
{
	// detect coasts
	for (int x = 0; x < HM_WIDTH - 1; x++)
	{
		for (int y = 0; y < HM_HEIGHT - 1; y++)
		{
			float h = heightmap->getValue(x, y);
			float h2 = heightmap->getValue(x + 1, y);
			if ((h < sandHeight && h2 >= sandHeight)
				|| (h2 < sandHeight && h >= sandHeight))
			{
				img->setPixel(x, y, Doryen::Palette::GRAY_WARN_90);
			}
			else
			{
				h = heightmap->getValue(x, y);
				h2 = heightmap->getValue(x, y + 1);
				if ((h < sandHeight && h2 >= sandHeight)
					|| (h2 < sandHeight && h >= sandHeight))
				{
					img->setPixel(x, y, Doryen::Palette::GRAY_WARN_90);
				}
			}
		}
	}
}

void WorldGenerator::saveBiomeMap(const char* filename)
{
	static Doryen::Image* legend = NULL;
	static int legendHeight, legendWidth;
	static const Doryen::Color biomeColors[] = {
			// TUNDRA,
			Doryen::Color(88, 234, 250),
			// COLD_DESERT,
			Doryen::Color(129, 174, 170),
			// GRASSLAND,
			Doryen::Palette::CYAN,
			// BOREAL_FOREST,
			Doryen::Color(14, 93, 43),
			// TEMPERATE_FOREST,
			Doryen::Color(44, 177, 83),
			// TROPICAL_MONTANE_FOREST,
			Doryen::Color(185, 232, 164),
			// HOT_DESERT,
			Doryen::Color(229, 247, 184),
			// SAVANNA,
			Doryen::Palette::ORANGE,
			// TROPICAL_DRY_FOREST,
			Doryen::Palette::YELLOW,
			// TROPICAL_EVERGREEN_FOREST,
			Doryen::Palette::GRAY_WARN_90,
			// THORN_FOREST,
			Doryen::Color(192, 192, 112),
	};
	if (legend == NULL)
	{
		legend = new Doryen::Image("Data/img/legend_biome.png");

		const auto[width, height] = legend->getSize().unpack();

		legendWidth = width;
		legendHeight = height;
	}
	if (filename == NULL)
	{ filename = "world_biome.png"; }
	Doryen::Image img(std::max(HM_WIDTH, legendWidth), HM_HEIGHT + legendHeight);
	// draw biome map
	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			float h = heightmap->getValue(x, y);
			if (h < sandHeight)
			{ img.setPixel(x, y, Doryen::Color(100, 100, 255)); }
			else
			{ img.setPixel(x, y, biomeColors[biomeMap[x + y * HM_WIDTH]]); }
		}
	}
	drawCoasts(&img);
	// blit legend
	int legendx = std::max(HM_WIDTH, legendWidth) / 2 - legendWidth / 2;
	for (int x = 0; x < legendWidth; x++)
	{
		for (int y = 0; y < legendHeight; y++)
		{
			img.setPixel(legendx + x, HM_HEIGHT + y, legend->getPixel(x, y));
		}
	}
	// fill legend colors
	for (int i = 0; i < 6; i++)
	{
		for (int x = 17; x < 47; x++)
		{
			for (int y = 4 + i * 14; y < 14 + i * 14; y++)
			{
				img.setPixel(legendx + x, HM_HEIGHT + y, biomeColors[i]);
			}
		}
	}
	for (int i = 6; i < NB_BIOMES; i++)
	{
		for (int x = 221; x < 251; x++)
		{
			for (int y = 4 + (i - 6) * 14; y < 14 + (i - 6) * 14; y++)
			{
				img.setPixel(legendx + x, HM_HEIGHT + y, biomeColors[i]);
			}
		}
	}
	//img.save(filename);
}

void WorldGenerator::saveTemperatureMap(const char* filename)
{
	static Doryen::Color tempGradient[256];

	static Doryen::Image* legend = NULL;
	static int legendHeight, legendWidth;
	if (legend == NULL)
	{
		legend = new Doryen::Image("Data/img/legend_temperature.png");

		const auto[width, height] = legend->getSize().unpack();

		legendWidth = width;
		legendHeight = height;

		generateSmoothColorMap(tempGradient, MAX_TEMP_KEY, tempKeyColor, tempIndexes);
	}

	if (filename == NULL)
	{ filename = "world_temperature.png"; }
	Doryen::Image img(std::max(HM_WIDTH, legendWidth), HM_HEIGHT + legendHeight);
	float minTemp, maxTemp;
	temperature->getMinMax(&minTemp, &maxTemp);
	// render temperature map
	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			float h = heightmap->getValue(x, y);
			if (h < sandHeight)
			{ img.setPixel(x, y, Doryen::Color(100, 100, 255)); }
			else
			{
				float temp = temperature->getValue(x, y);
				temp = (temp - minTemp) / (maxTemp - minTemp);
				int colorIdx = (int)(temp * 255);
				colorIdx = std::clamp(colorIdx, 0, 255);
				img.setPixel(x, y, tempGradient[colorIdx]);
			}
		}
	}
	drawCoasts(&img);

	// blit legend
	int legendx = std::max(HM_WIDTH, legendWidth) / 2 - legendWidth / 2;
	for (int x = 0; x < legendWidth; x++)
	{
		for (int y = 0; y < legendHeight; y++)
		{
			img.setPixel(legendx + x, HM_HEIGHT + y, legend->getPixel(x, y));
		}
	}
//	img.save(filename);
}

void WorldGenerator::savePrecipitationMap(const char* filename)
{
	static Doryen::Image* legend = NULL;
	static int legendHeight, legendWidth;
	if (legend == NULL)
	{
		legend = new Doryen::Image("Data/img/legend_precipitation.png");

		const auto[width, height] = legend->getSize().unpack();

		legendWidth = width;
		legendHeight = height;
	}

	if (filename == NULL)
	{ filename = "world_precipitation.png"; }
	Doryen::Image img(std::max(HM_WIDTH, legendWidth), HM_HEIGHT + legendHeight);
	// render precipitation map
	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			float h = heightmap->getValue(x, y);
			if (h < sandHeight)
			{ img.setPixel(x, y, Doryen::Color(100, 100, 255)); }
			else
			{
				float prec = precipitation->getValue(x, y);
				int iprec = (int)(prec * 180);
				int colorIdx = 0;
				while (colorIdx < MAX_PREC_KEY && iprec > precIndexes[colorIdx])
				{ colorIdx++; }
				colorIdx = std::clamp(colorIdx, 0, MAX_PREC_KEY);
				img.setPixel(x, y, precColors[colorIdx]);
			}
		}
	}
	drawCoasts(&img);

	// blit legend
	int legendx = std::max(HM_WIDTH, legendWidth) / 2 - legendWidth / 2;
	for (int x = 0; x < legendWidth; x++)
	{
		for (int y = 0; y < legendHeight; y++)
		{
			img.setPixel(legendx + x, HM_HEIGHT + y, legend->getPixel(x, y));
		}
	}
//	img.save(filename);
}

void WorldGenerator::saveAltitudeMap(const char* filename)
{
	static Doryen::Color altGradient[256];

	static Doryen::Image* legend = NULL;
	static int legendHeight, legendWidth;
	if (legend == NULL)
	{
		legend = new Doryen::Image("Data/img/legend_altitude.png");

		const auto[width, height] = legend->getSize().unpack();

		legendWidth = width;
		legendHeight = height;

		generateSmoothColorMap(altGradient, MAX_ALT_KEY, altColors, altIndexes);
	}

	if (filename == NULL)
	{ filename = "world_altitude.png"; }
	Doryen::Image img(HM_WIDTH + legendWidth, std::max(HM_HEIGHT, legendHeight));
	// render altitude map
	for (int x = 0; x < HM_WIDTH; x++)
	{
		for (int y = 0; y < HM_HEIGHT; y++)
		{
			float h = heightmap->getValue(x, y);
			int ialt = (int)(h * 256);
			ialt = std::clamp(ialt, 0, 255);
			img.setPixel(x, y, altGradient[ialt]);
		}
	}

	// blit legend
	int legendy = std::max(HM_HEIGHT, legendHeight) / 2 - legendHeight / 2;
	for (int x = 0; x < legendWidth; x++)
	{
		for (int y = 0; y < legendHeight; y++)
		{
			img.setPixel(HM_WIDTH + x, legendy + y, legend->getPixel(x, y));
		}
	}
//	img.save(filename);
}

WorldGenerator::~WorldGenerator()
{
	delete heightmap;
	delete heightmapWithoutErosion;
	delete temperature;
	delete precipitation;

	delete[] mapData;
	delete[] worldint;
}

WorldGenerator::WorldGenerator(Console& _console) : console(_console)
{

}

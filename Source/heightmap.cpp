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
#include <math.h>
#include <heightmap.hpp>

#include "libtcod.hpp"

#define CLAMP(a, b, x)        ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))

TCODHeightMap::TCODHeightMap(int w, int h)
{
	this->w = w;
	this->h = h;

	values.reserve(w * h);
	std::fill(values.cbegin(), values.cend(), 0.0f);
}

TCODHeightMap::~TCODHeightMap() = default;

void TCODHeightMap::clear()
{
	std::fill(values.cbegin(), values.cend(), 0.0f);
}

void TCODHeightMap::normalize(float newMin, float newMax)
{
	float curmin = getMin();
	float curmax = getMax();

	float invmax = 0;

	if (curmax - curmin == 0.0f)
	{
		invmax = 0.0f;
	}
	else
	{
		invmax = (newMax - newMin) / (curmax - curmin);
	}

	// NOTE: Comprobar con la función original. Posible fallo en el recorrido.
	for (int i = 0; i < w * h; ++i)
	{
		values[i] = newMin + (values[i] - curmin) * invmax;
	}
}

void TCODHeightMap::getMinMax(float* min, float* max) const
{
	// Without usage
}

void TCODHeightMap::addHill(float hx, float hy, float hradius, float height)
{
	float hradius2 = hradius * hradius;
	float coef = height / hradius2;

	int minx = (int)std::max(0.0f, hx - hradius);
	int maxx = (int)std::min((float)this->w, hx + hradius);

	int miny = (int)std::max(0.0f, hy - hradius);
	int maxy = (int)std::min((float)this->h, hy + hradius);

	for (int x = minx; x < maxx; ++x)
	{
		float xdist = (x - hx) * (x - hx);

		for (int y = miny; y < maxy; ++y)
		{
			float z = hradius2 - xdist - (y - hy) * (y - hy);

			if (z > 0.0)
			{
				values[x + this->w * y] += z * coef;
			}
		}
	}
}

void TCODHeightMap::digHill(float hx, float hy, float hradius, float height)
{
	float hradius2 = hradius * hradius;
	float coef = height / hradius2;

	int minx = (int)std::max(0.0f, hx - hradius);
	int maxx = (int)std::min((float)this->w, hx + hradius);

	int miny = (int)std::max(0.0f, hy - hradius);
	int maxy = (int)std::min((float)this->h, hy + hradius);

	for (int x = minx; x < maxx; ++x)
	{
		float xdist = (x - hx) * (x - hx);

		for (int y = miny; y < maxy; ++y)
		{
			float dist = xdist + (y - hy) * (y - hy);

			if (dist < hradius2)
			{
				float z = (hradius2 - dist) * coef;

				if (height > 0.0f)
				{
					if (values[x + this->w * y] < z)
					{
						values[x + this->w * y] = z;
					}
				}
				else
				{
					if (values[x + this->w * y] > z)
					{
						values[x + this->w * y] = z;
					}
				}
			}
		}
	}
}

void TCODHeightMap::copy(const TCODHeightMap* source)
{
	if (source->w != this->w || source->h != this->h)
	{
		// Throw Exception
		return;
	}

	for (int i = 0; i < source->w * source->h; ++i)
	{
		this->values[i] = source->values[i];
	}
}

void TCODHeightMap::addFbm(TCODNoise* noise, float mulx, float muly, float addx, float addy, float octaves, float delta,
		float scale)
{
	float xcoef = mulx / (float)this->w;
	float ycoef = muly / (float)this->h;

	float min = 1.0f;
	float max = 0.0f;

	for (int x = 0; x < this->w; x++)
	{
		float f[2];

		int offset = x;

		f[0] = (x + addx) * xcoef;

		for (int y = 0; y < this->h; y++)
		{
			float value;

			f[1] = (y + addy) * ycoef;

			value = delta + TCOD_noise_get_fbm(noise, f, octaves) * scale;

			this->values[offset] += value;

			if (value < min)
			{ min = value; }

			if (value > max)
			{ max = value; }

			offset += this->w;
		}
	}
}

void
TCODHeightMap::scaleFbm(TCODNoise* noise, float mulx, float muly, float addx, float addy, float octaves, float delta,
		float scale)
{
	float xcoef = mulx / this->w;
	float ycoef = muly / this->h;

	for (int x = 0; x < this->w; x++)
	{
		float f[2];
		int offset = x;

		f[0] = (x + addx) * xcoef;

		for (int y = 0; y < this->h; y++)
		{
			f[1] = (y + addy) * ycoef;

			this->values[offset] *= (delta + TCOD_noise_get_fbm(noise, f, octaves) * scale);

			offset += this->w;
		}
	}
}

float TCODHeightMap::getInterpolatedValue(float x, float y) const
{
	int ix = (int)x;
	int iy = (int)y;

	if (ix >= this->w - 1 || iy >= this->h - 1)
	{
		return this->values[ix + this->w * iy];
	}
	else
	{
		float dx = x - ix;
		float dy = y - iy;
		float c1 = this->values[ix + this->w * iy];
		float c2 = this->values[(ix + 1) + this->w * iy];
		float c3 = this->values[ix + this->w * (iy + 1)];
		float c4 = this->values[(ix + 1) + this->w * (iy + 1)];

		float top = (1.0f - dx) * c1 + dx * c2;
		float bottom = (1.0f - dx) * c3 + dx * c4;
		return (1.0f - dy) * top + dy * bottom;
	}
}

void TCODHeightMap::getNormal(float x, float y, float n[3], float waterLevel) const
{
	float h0, hx, hy, invlen; /* map heights at x,y x+1,y and x,y+1 */
	n[0] = 0.0f;
	n[1] = 0.0f;
	n[2] = 1.0f;

	if (x >= this->w - 1 || y >= this->h - 1)
	{ return; }

	h0 = getInterpolatedValue(x, y);

	if (h0 < waterLevel)
	{ h0 = waterLevel; }

	hx = getInterpolatedValue(x + 1, y);

	if (hx < waterLevel)
	{ hx = waterLevel; }

	hy = getInterpolatedValue(x, y + 1);

	if (hy < waterLevel)
	{ hy = waterLevel; }

	n[0] = 255 * (h0 - hx);
	n[1] = 255 * (h0 - hy);
	n[2] = 16.0f;

	// Normalize
	invlen = 1.0f / (float)sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);

	n[0] *= invlen;
	n[1] *= invlen;
	n[2] *= invlen;
}

void
TCODHeightMap::digBezier(int px[4], int py[4], float startRadius, float startDepth, float endRadius, float endDepth)
{
	int xFrom = px[0];
	int yFrom = py[0];

	for (float t = 0.0f; t <= 1.0f; t += 0.001f)
	{
		float it = 1.0f - t;

		int xTo = (int)(px[0] * it * it * it + 3 * px[1] * t * it * it + 3 * px[2] * t * t * it + px[3] * t * t * t);
		int yTo = (int)(py[0] * it * it * it + 3 * py[1] * t * it * it + 3 * py[2] * t * t * it + py[3] * t * t * t);

		if (xTo != xFrom || yTo != yFrom)
		{
			float radius = startRadius + (endRadius - startRadius) * t;
			float depth = startDepth + (endDepth - startDepth) * t;

			digHill((float)xTo, (float)yTo, radius, depth);

			xFrom = xTo;
			yFrom = yTo;
		}
	}
}

bool TCODHeightMap::hasLandOnBorder(float seaLevel) const
{
	for (int x = 0; x < this->w; x++)
	{
		if (values[x] > seaLevel
			|| values[x + this->w * (this->h - 1)] > seaLevel)
		{
			return true;
		}
	}

	for (int y = 0; y < this->h; y++)
	{
		if (values[this->w * y] > seaLevel
			|| values[(this->w - 1) + this->w * y] > seaLevel)
		{
			return true;
		}
	}

	return false;
}

void TCODHeightMap::islandify(float seaLevel, TCODRandom* rnd)
{
	// NOTE: Without implementation in Libtcod 1.5.1
}

void TCODHeightMap::add(float f)
{
	for (int i = 0; i < w * h; ++i)
	{
		values[i] += f;
	}
}

int TCODHeightMap::countCells(float min, float max) const
{
	int count = 0;

	for (int i = 0; i < w * h; ++i)
	{
		if (values[i] >= min && values[i] <= max)
		{
			count++;
		}
	}

	return count;
}

void TCODHeightMap::scale(float f)
{
	for (int i = 0; i < w * h; ++i)
	{
		values[i] *= f;
	}
}

void TCODHeightMap::clamp(float min, float max)
{
	for (int i = 0; i < w * h; ++i)
	{
		values[i] = CLAMP(min, max, values[i]);
	}
}

void TCODHeightMap::lerp(const TCODHeightMap* a, const TCODHeightMap* b, float coef)
{
	if (a->w != b->w || a->h != b->h)
	{
		return;
	}

	if (this->w != a->w || this->h != a->h)
	{
		return;
	}

	for (int i = 0; i < w * h; ++i)
	{
		this->values[i] = a->values[i] + (b->values[i] - a->values[i]) * coef;
	}
}

void TCODHeightMap::add(const TCODHeightMap* a, const TCODHeightMap* b)
{
	if (a->w != b->w || a->h != b->h)
	{
		return;
	}

	if (this->w != a->w || this->h != a->h)
	{
		return;
	}

	for (int i = 0; i < w * h; ++i)
	{
		this->values[i] = a->values[i] + b->values[i];
	}
}

void TCODHeightMap::multiply(const TCODHeightMap* a, const TCODHeightMap* b)
{
	if (a->w != b->w || a->h != b->h)
	{
		return;
	}

	if (this->w != a->w || this->h != a->h)
	{
		return;
	}

	for (int i = 0; i < w * h; ++i)
	{
		this->values[i] = a->values[i] * b->values[i];
	}
}

float TCODHeightMap::getSlope(int x, int y) const
{
	static int dix[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	static int diy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

	float mindy = 0.0f, maxdy = 0.0f;
	float v = values[x + this->w * y];

	for (int i = 0; i < 8; i++)
	{
		int nx = x + dix[i];
		int ny = y + diy[i];

		if (nx >= 0 && nx < this->w && ny >= 0 && ny < this->h)
		{
			float nslope = (values[nx + this->w * ny] - v);

			if (nslope > maxdy)
			{
				maxdy = nslope;
			}
			else if (nslope < mindy)
			{
				mindy = nslope;
			}
		}
	}

	return (float)std::atan2(maxdy + mindy, 1.0f);
}

void TCODHeightMap::rainErosion(int nbDrops, float erosionCoef, float agregationCoef, TCODRandom* rnd)
{
	while (nbDrops > 0)
	{
		int curx = TCOD_random_get_int(rnd, 0, this->w - 1);
		int cury = TCOD_random_get_int(rnd, 0, this->h - 1);

		static int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		static int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

		float slope = 0.0f;
		float sediment = 0.0f;

		do
		{
			int nextx = 0, nexty = 0, i;
			float v = values[curx + this->w * cury];

			// Calculate slope at x, y
			slope = 0.0f;

			for (i = 0; i < 8; i++)
			{
				int nx = curx + dx[i];
				int ny = cury + dy[i];
				if (nx >= 0 && nx < this->w && ny >= 0 && ny < this->h)
				{
					float nslope = v - values[nx + this->w * ny];
					if (nslope > slope)
					{
						slope = nslope;
						nextx = nx;
						nexty = ny;
					}
				}
			}
			if (slope > 0.0f)
			{
				values[curx + this->w * cury] -= erosionCoef * slope;
				curx = nextx;
				cury = nexty;
				sediment += slope;
			}
			else
			{
				values[curx + this->w * cury] += agregationCoef * sediment;
			}

		} while (slope > 0.0f);

		nbDrops--;
	}
}

void TCODHeightMap::kernelTransform(int kernelSize, const int* dx, const int* dy, const float* weight, float minLevel,
		float maxLevel)
{
	TCOD_heightmap_t hm = { w, h, values };
	TCOD_heightmap_kernel_transform(&hm, kernelSize, dx, dy, weight, minLevel, maxLevel);
}

void TCODHeightMap::addVoronoi(int nbPoints, int nbCoef, const float* coef, TCODRandom* rnd)
{
	TCOD_heightmap_t hm = { w, h, values };
	TCOD_heightmap_add_voronoi(&hm, nbPoints, nbCoef, coef, rnd->data);
}

float TCODHeightMap::getMin() const
{
	//std::max(values.cbegin(), values.cend());

	float min = values.at(0);

	for (float f : values)
	{
		if (f < min)
		{
			min = f;
		}
	}

	return min;
}

float TCODHeightMap::getMax() const
{
	//std::max(values.cbegin(), values.cend());

	float max = values.at(0);

	for (float f : values)
	{
		if (f > max)
		{
			max = f;
		}
	}

	return max;
}

#if 0
void TCODHeightMap::heatErosion(int nbPass,float minSlope,float erosionCoef,float agregationCoef,TCODRandom *rnd) {
	TCOD_heightmap_t hm={w,h,values};
	TCOD_heightmap_heat_erosion(&hm, nbPass, minSlope, erosionCoef, agregationCoef, rnd->data);
}

void TCODHeightMap::midPointDeplacement(TCODRandom *rnd) {
	TCOD_heightmap_t hm={w,h,values};
	TCOD_heightmap_mid_point_deplacement(&hm, rnd->data);
}
#endif


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

#include "MapData.hpp"
#include "Doryen/Algorithms/Generation/Heightmap.hpp"

// size of the heightmap
constexpr int HM_WIDTH = 400;

constexpr int HM_HEIGHT = 400;

// biome and climate list. based on Whittaker Biome Diagram
enum EClimate
{
	ARTIC_ALPINE,
	COLD,
	TEMPERATE,
	WARM,
	TROPICAL,
	NB_CLIMATES
};

// grassland : might be either grassland, shrubland or woodland depending on the vegetation level
// savanna : might be either savanna or thorn forest depending on the vegetation level
enum EBiome
{
	TUNDRA,
	COLD_DESERT, GRASSLAND, BOREAL_FOREST,
	TEMPERATE_FOREST, TROPICAL_MONTANE_FOREST,
	HOT_DESERT, SAVANNA, TROPICAL_DRY_FOREST, TROPICAL_EVERGREEN_FOREST,
	THORN_FOREST,
	NB_BIOMES
};

class WorldGenerator
{

public :

	// Fields

	/**
	 * Altitude->color map.
	 */
	Doryen::Color mapGradient[256];

	/**
	 * Complete world map (not shaded).
	 */
	Doryen::Image* imageWorldmap = new Doryen::Image(HM_WIDTH, HM_HEIGHT);

	/**
	 * World height map (0.0 - 1.0) .
	 */
	Doryen::Heightmap* heightmap = new Doryen::Heightmap(HM_WIDTH, HM_HEIGHT);

	/**
	 * Height map without erosion.
	 */
	Doryen::Heightmap* heightmapWithoutErosion = new Doryen::Heightmap(HM_WIDTH, HM_HEIGHT);

	/**
	 * Temperature map (in °C).
	 */
	Doryen::Heightmap* temperature = new Doryen::Heightmap(HM_WIDTH, HM_HEIGHT);

	/**
	 * Precipitation map (0.0 - 1.0).
	 */
	Doryen::Heightmap* precipitation = new Doryen::Heightmap(HM_WIDTH, HM_HEIGHT);

	/**
	 * Biome map.
	 */
	EBiome* biomeMap = new EBiome[HM_WIDTH * HM_HEIGHT];

	/**
	 * Map data.
	 */
	MapData* mapData = new MapData[HM_WIDTH * HM_HEIGHT];

	/**
	 * Random Generation.
	 */
	TCODRandom* wgRng = TCODRandom::getInstance();

	/**
	 * Noise.
	 */
	TCODNoise* noise;

	/**
	 * World light intensity map (shadow map).
	 */
	float* worldint = new float[HM_WIDTH * HM_HEIGHT];

	/**
	 * Horizontal offset for smooth scrolling.
	 */
	float cloudDx = 0.0f;

	/**
	 * Horizontal offset for smooth scrolling.
	 */
	float cloudTotalDx = 0.0f;

	/**
	 * Cloud thickness.
	 */
	float clouds[HM_WIDTH][HM_HEIGHT];

	// Destructor

	virtual ~WorldGenerator();

	// Methods

	void generate(TCODRandom* wRng);

	bool isOnSea(float x, float y) const;

	// Getters

	int getWidth() const;

	int getHeight() const;

	float getAltitude(int x, int y) const; // heightmap. between 0 and 1

	float getInterpolatedAltitude(float x, float y) const;

	float getSandHeight() const;

	float getCloudThickness(float x, float y) const;

	float getInterpolatedIntensity(float worldX, float worldY);

	void getInterpolatedNormal(float x, float y, float n[3]) const;

	Doryen::Color getInterpolatedColor(float worldX, float worldY);

	// Update Methods

	void updateClouds(float elapsedTime);

	void computeSunLight(float lightDir[3]);

	// Data

	float getRealAltitude(float x, float y) const; // altitude in meters

	float getPrecipitations(float x, float y) const; // in centimeter/m3/year

	float getTemperature(float x, float y) const; // in °C

	EBiome getBiome(float x, float y) const;

	// Map Generators

	void saveBiomeMap(const char* filename = nullptr);

	void saveAltitudeMap(const char* filename = nullptr);

	void saveTemperatureMap(const char* filename = nullptr);

	void savePrecipitationMap(const char* filename = nullptr);

private:

	void addHill(int nbHill, float baseRadius, float radiusVar, float height);

	/**
	 * Function building the heightmap.
	 */
	void buildBaseMap();

	void erodeMap();

	void smoothMap();

	// compute the ground color from the heightmap
	Doryen::Color getMapColor(float h);

	// get sun light intensity on a point of the map
	float getMapIntensity(float worldX, float worldY, float lightDir[3]);

	Doryen::Color getInterpolatedColor(Doryen::Image* img, float x, float y);

	float getInterpolatedFloat(float* arr, float x, float y, int width, int height);

	void generateRivers();

	void smoothPrecipitations();

	void setLandMass(float percent, float waterLevel);

	void computeTemperaturesAndBiomes();

	Doryen::Color getBiomeColor(EBiome biome, int x, int y);

	void computePrecipitations();

	void computeColors();

	void drawCoasts(Doryen::Image* img);

	EClimate getClimateFromTemp(float temp);
};

// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_PHOTONSHADER_HPP
#define DORYEN_PHOTONSHADER_HPP

#include "Interface/Shader.hpp"

class PhotonShader : public Shader
{

public :

	PhotonShader(Map& map) noexcept;

	void compute();

	int addLight(int x, int y, int radius, const Doryen::Color& col);

protected :
	// maximum radius of a light in the map
	int maxRadius = 0;
	float reflectivity = 1.5f;
	float selfIllumination = 0.4f;
	int nbPass = 3;
	// array of MAP_WIDTH*MAP_HEIGHT*maxDiameter*maxDiamter form factor coefficients (0-255)
	// maxDiameter = 2*maxRadius+1
	std::vector<float> ff;
	// array of MAP_WIDTH*MAP_HEIGHT containing the sum of all affected cells form factors for each cell in the map
	float* ffSum;

	// convenient way to store the list of cells with non null incoming light
	struct Coord
	{
		uint16 x;
		uint16 y;

		Coord()
		{
		}

		Coord(int x, int y) : x(x), y(y)
		{
		}
	};

	TCODList<Coord> lightsCoord;
	// color not limited to 0-255 range
	struct FColor
	{
		float r, g, b;
	};
	struct CellData
	{
		// amount of light emitted by this cell for this pass
		FColor emission;
		// amount of light incoming from other cells for this pass (including self through self illumination)
		FColor incoming;
		// total amount of light on the cell (used to shade the map)
		FColor outgoing;
	};

	// array of MAP_WIDTH*MAP_HEIGHT CellData
	std::vector<CellData> data;

	void propagate();

	void computeFormFactor(int x, int y);

	void computeLightContribution(int lx, int ly, int lradius, const FColor& lcol, float reflectivity);

};


#endif //DORYEN_PHOTONSHADER_HPP

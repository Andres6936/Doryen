#include "Doryen/FOV/Permissive.hpp"

void
Doryen::Permissive::operator()(Doryen::Map& map, int playerX, int playerY, int maxRadius, bool ligthWalls, int fovType)
{
	// Defines the parameters of the permissiveness.
	// Derived values defining the actual part of the square used as a range.
	offset = 8 - fovType;
	limit = 8 + fovType;

	map.resetFieldView();
	map.setVisibleFieldView(playerX, playerY);

	views.resize(map.getWidth() * map.getHeight());
	bumps.resize(map.getWidth() * map.getHeight());

	std::int32_t minX{ 0 };
	std::int32_t maxX{ 0 };

	std::int32_t minY {0};
	std::int32_t maxY {0};

	if (maxRadius > 0)
	{
		minX = std::min(playerX, maxRadius);
		maxX = std::min(map.getWidth() - playerX - 1, maxRadius);

		minY = std::min(playerY, maxRadius);
		maxY = std::min(map.getHeight() - playerY - 1, maxRadius);
	}
	else
	{
		minX = playerX;
		maxX = map.getWidth() - playerX - 1;

		minY = playerY;
		maxY = map.getHeight() - playerY - 1;
	}

	// Calculate fov. precise permissive field of view
	bumpIdx = 0;
}

void
Doryen::Permissive::checkQuadrant(Doryen::Map& _map,
		Doryen::Geometry::Point2D<> start, Doryen::Geometry::Point2D<> d,
		Doryen::Geometry::Point2D<> extent, bool lightWalls)
{
	Line shallowLine{ offset, limit, extent.x * STEP_SIZE, 0 };
	Line steepLine = { limit, offset, 0, extent.y * STEP_SIZE };

	std::int32_t maxI = extent.x + extent.y;

	auto view = views.begin() + (start.x + start.y * _map.getWidth());

	view->shallowLine = shallowLine;
	view->steepLine = steepLine;

	std::vector<View> activeViews;
	activeViews.push_back(*view);


}

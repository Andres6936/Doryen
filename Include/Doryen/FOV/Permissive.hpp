#ifndef LIBTCOD_PERMISSIVE_HPP
#define LIBTCOD_PERMISSIVE_HPP

#include <memory>
#include <vector>
#include <cstdint>

#include "Doryen/Geometry/Point2D.hpp"
#include <Doryen/FOV/Fov.hpp>

namespace Doryen
{

	class Permissive
	{

	private:

		class ViewBump final : public Geometry::Point2D<>
		{

		public:

			std::int32_t refCount{ 0 };

			std::shared_ptr<ViewBump> parent{ nullptr };

		};

		class Line final
		{

		public:

			std::int32_t xi{ 0 };
			std::int32_t yi{ 0 };

			std::int32_t xf{ 0 };
			std::int32_t yf{ 0 };

		};

		class View final
		{

		public:

			Line shallowLine;

			Line steepLine;

			std::vector<ViewBump> shallowBump;

			std::vector<ViewBump> steepBump;

		};

		std::vector<View> views;

		std::vector<ViewBump> bumps;

	public:

		void operator()(Doryen::Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls, int fovType);
	};

}

#endif //LIBTCOD_PERMISSIVE_HPP

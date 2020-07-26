#ifndef LIBTCOD_DIAMONDRAYCASTING_HPP
#define LIBTCOD_DIAMONDRAYCASTING_HPP

#include <memory>
#include <cstdint>
#include <optional>

#include <Doryen/FOV/Fov.hpp>
#include <Doryen/Geometry/Point2D.hpp>

namespace Doryen
{
	class DiamondRayCasting
	{

	private:

		class RayData
		{

		public:

			/**
			 * Position in coordinate x and y.
			 */
			Geometry::Point2D<> position{};

			/**
			 * Obscurity vector.
			 */
			Geometry::Point2D<> obscurity{};

			/**
			 * Bresenham error.
			 */
			Geometry::Point2D<> bresenham{};

			/**
			 * Offset of input rays.
			 */
			std::unique_ptr<RayData> xInput{ nullptr };

			/**
			 * Offset of input rays.
			 */
			std::unique_ptr<RayData> yInput{ nullptr };

			/**
			 * Already in the field of view.
			 */
			bool added = false;

			/**
			 * Not visible. Don't bother processing it.
			 */
			bool ignore = false;
		};

		using Iterator = std::vector<RayData>::iterator;

		/**
		 * Temporary rays
		 */
		std::vector<RayData> raymap2;

		/**
		 * Result rays
		 */
		std::vector<std::vector<RayData>> raymap;

		std::optional<Iterator> newRay(Map& _map, const Geometry::Point2D<>& _coordinate,
				const Geometry::Point2D<>& _origin);

		void processXInput(Iterator newRay, Iterator xInput);

		void processYInput(Iterator newRay, Iterator yInput);

		void mergeInput(Map& _map, const Geometry::Point2D<>& _origin, Iterator _ray);

		void expandPerimeterFrom(Map& map, std::vector<RayData>& perim, std::optional<Iterator> ray);

	public:

		void operator()(Map& map, int playerX, int playerY,
				int maxRadius, bool ligthWalls);
	};
}


#endif //LIBTCOD_DIAMONDRAYCASTING_HPP

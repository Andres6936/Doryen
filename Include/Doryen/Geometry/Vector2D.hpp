// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_VECTOR2D_HPP
#define DORYEN_VECTOR2D_HPP

#include "Doryen/Geometry/Point2D.hpp"

namespace Doryen
{

	namespace Geometry
	{
		// For avoid create a new class that inheritance of Point2D
		// Use a alias for simplify the code
		template<typename T = std::int32_t>
		using Vector2D = Point2D<T>;
	}

}

#endif //DORYEN_VECTOR2D_HPP

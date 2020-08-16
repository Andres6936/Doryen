// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_GRADIENT_HPP
#define DORYEN_GRADIENT_HPP

namespace Doryen::Algorithms
{

	class Gradient
	{

	public:

		double x;
		double y;
		double z;
		double w;

		Gradient() noexcept = default;

		Gradient(double _x, double _y, double _z) noexcept: x(_x), y(_y), z(_z)
		{
		};

		Gradient(double _x, double _y, double _z, double _w) noexcept: x(_x), y(_y), z(_z), w(_w)
		{
		};
	};
}

#endif //DORYEN_GRADIENT_HPP

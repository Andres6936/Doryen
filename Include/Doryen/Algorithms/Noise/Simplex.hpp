// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_SIMPLEX_HPP
#define DORYEN_SIMPLEX_HPP

#include <array>
#include <cstdint>
#include <cmath>

namespace Doryen::Algorithms
{

	template<int Dimension>
	class Simplex
	{

	private:

		// Class Private

		class Grad
		{

		public:

			double x;
			double y;
			double z;
			double w;

			Grad() noexcept = default;

			Grad(double _x, double _y, double _z) noexcept: x(_x), y(_y), z(_z)
			{
			};

			Grad(double _x, double _y, double _z, double _w) noexcept: x(_x), y(_y), z(_z), w(_w)
			{
			};
		};

		// Const Variables

		// Skewing and unskewing factors for 2, 3, and 4 dimensions
		const double F2 = 0.5 * (std::sqrt(3.0) - 1.0);
		const double G2 = (3.0 - std::sqrt(3.0)) / 6.0;
		const double F3 = 1.0 / 3.0;
		const double G3 = 1.0 / 6.0;
		const double F4 = (std::sqrt(5.0) - 1.0) / 4.0;
		const double G4 = (5.0 - std::sqrt(5.0)) / 20.0;

		// Static Variables

		inline static std::array<Grad, 12> grad3;

		inline static std::array<Grad, 32> grad4;

		// Methods

		static double dot(const Grad& g, double x, double y);

	public:

		Simplex() noexcept;

		float noise(const std::array<float, Dimension>& input);

	};

}

#endif //DORYEN_SIMPLEX_HPP

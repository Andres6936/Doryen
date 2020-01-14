#ifndef LIBTCOD_RENDERER_HPP
#define LIBTCOD_RENDERER_HPP

#include <vector>
#include <array>
#include <Color.h>

namespace Doryen
{
	class Renderer
	{

	private:

		std::vector <bool> ascii_updated;

		std::vector <bool> first_draw;

		std::vector <TCOD_color_t> charcols;

	public:

		// Constructs

		Renderer() = default;

		virtual ~Renderer() = default;

		// Static members

		static const std::array <int, 256> init_ascii_to_tcod;

		// Methods

		void clearAndResizeASCII(unsigned size);

		void clearAndResizeFirstDraw(unsigned size);

		void clearAndResizeCharcols(unsigned size);
	};
}

#endif //LIBTCOD_RENDERER_HPP

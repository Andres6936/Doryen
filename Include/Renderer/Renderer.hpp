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

	public:

		// Constructs

		Renderer() = default;

		virtual ~Renderer() = default;

		// Static members

		static const std::array <int, 256> init_ascii_to_tcod;

		// Methods
	};
}

#endif //LIBTCOD_RENDERER_HPP

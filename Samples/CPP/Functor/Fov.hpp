// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_FOV_HPP
#define DORYEN_FOV_HPP

#include <array>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class FOV : public ISample
	{

	private:

		const float TORCH_RADIUS = 10.0f;

		const float SQUARED_TORCH_RADIUS = (TORCH_RADIUS * TORCH_RADIUS);

		/**
		 * The width of dungeon is get: std::strlen(dungeon.at(0))
		 * The height of dungeon is get: dungeon.size()
		 *
		 * @note The width and height dungeon should be
		 *  correspond with the width and height of console.
		 */
		constexpr static const std::array dungeon = {
				"##############################################",
				"#######################      #################",
				"#####################    #     ###############",
				"######################  ###        ###########",
				"##################      #####             ####",
				"################       ########    ###### ####",
				"###############      #################### ####",
				"################    ######                  ##",
				"########   #######  ######   #     #     #  ##",
				"########   ######      ###                  ##",
				"########                                    ##",
				"####       ######      ###   #     #     #  ##",
				"#### ###   ########## ####                  ##",
				"#### ###   ##########   ###########=##########",
				"#### ##################   #####          #####",
				"#### ###             #### #####          #####",
				"####           #     ####                #####",
				"########       #     #### #####          #####",
				"########       #####      ####################",
				"##############################################",
		};

		bool torch = false;

		bool light_walls = true;

		/**
		 * The player moved. must recompute fov
		 */
		bool recomputeFov = true;

		TypeFOV algonum = TypeFOV::Basic;

		int playerX = 20;
		int playerY = 10;

		/**
		 * Torch light position in the perlin noise
		 */
		std::array<float, 1> torchx = { 0.0f };

		Map map;

		/**
		 * 1D noise used for the torch flickering.
		 */
		Noise<1> noise{ 1 };

		void drawPlayer();

		void drawDungeon();

		void drawTextHelp();

		void movePlayer(KeyCode key);

		void switchTypeFieldView();

		void prepareInstanceOfMap();

		std::string typeFOVString();

	public:

		FOV(Console& _console);

		void render(KeyCode key, const Mouse& mouse) override;

	};
}

#endif //DORYEN_FOV_HPP

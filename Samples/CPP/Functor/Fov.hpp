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

		Map map;

		/**
		 * 1D noise used for the torch flickering.
		 */
		TCODNoise noise{ 1 };

		void prepareInstanceOfMap();

	public:

		FOV(const std::string _name, Console& _console);

		void render(KeyCode key, const Mouse& mouse) override;

	};
}

#endif //DORYEN_FOV_HPP

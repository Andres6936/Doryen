// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_PATH_HPP
#define DORYEN_PATH_HPP

#include <array>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class Path : public ISample
	{

	private:

		// Constexpr Fields

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

		// Const Fields

		const Doryen::Color DARK_WALL = { 0, 0, 100 };
		const Doryen::Color DARK_GROUND = { 50, 50, 150 };
		const Doryen::Color LIGHT_GROUND = { 200, 180, 50 };

		// Fields

		int playerX = 20;
		int playerY = 10;

		int destinationX = 33;
		int destinationY = 4;

	public:

		Path(const std::string _name, std::reference_wrapper<Console> _console);

		void render(KeyCode key, const Doryen::Mouse& mouse) override;

	};

}

#endif //DORYEN_PATH_HPP

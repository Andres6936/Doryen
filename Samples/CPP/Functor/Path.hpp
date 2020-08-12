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

		// Fields

		bool usingAStar = true;

		int playerX = 20;
		int playerY = 10;

		int destinationX = 33;
		int destinationY = 4;

		Map map;

		Algorithms::Pathfinding::AStar AStar;

	private:

		// Private Methods

		/**
		 * @pre The dungeon has been drawn.
		 */
		void drawPlayer();

		void drawDungeon();

		/**
		 * @pre The dungeon has been drawn.
		 */
		void drawHelpMessage();

		void drawDestinationPoint();

		/**
		 * Get the event generate for the user and determine if
		 * the user wanna move the coordinate of destination, if
		 * it is the case return true, false otherwise.
		 *
		 * @param key The key to verify are: W A S D
		 * @return True if the user move coordinate destination.
		 */
		bool moveDestination(KeyCode key);

		/**
		 * Determine if the user wanna use A* or Dijkstra.
		 *
		 * @param key The key to verify: TAB
		 * @return True if the user change the algorithm to use.
		 */
		bool changeAlgorithm(KeyCode key);

		/**
		 * @post The variable {mapHaveSizeCorrect} has been initialized.
		 */
		void prepareInstanceOfMap();

	public:

		Path(Console& _console);

		void render(KeyCode key, const Doryen::Mouse& mouse) override;

	};

}

#endif //DORYEN_PATH_HPP

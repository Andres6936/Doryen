/*
 * libtcod 1.5.1
 * Copyright (c) 2008,2009,2010,2012 Jice & Mingos
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * The name of Jice or Mingos may not be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY JICE AND MINGOS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL JICE OR MINGOS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TCOD_FOV_HPP
#define _TCOD_FOV_HPP

#pragma once

#include <vector>

#include "Doryen/Algorithms/Util/Cell.hpp"

namespace Doryen
{

	enum class TypeFOV : unsigned char
	{
		Basic,
		Diamond,
		Shadow,
		Permissive,
	};

	// Definition for allow consistence with names
	using VectorCell = std::vector<Cell>;

	/**
	 * This toolkit allows to easily calculate the potential visible set of map
	 * cells from the player position.
	 *
	 * @note A cell is potentially visible if the line of sight from the player
	 * to the cell in unobstructed.
	 *
	 * 1 - <b>FOV_BASIC</b>: classic libtcod fov algorithm (ray casted from the player
	 * to all the cells on the submap perimeter).
	 *
	 * 2 - <b>FOV_DIAMOND</b>: based on
	 * <a href="http://www.adammil.net/blog/v125_Roguelike_Vision_Algorithms.html#diamondcode">
	 * Diamond Code</a>.
	 *
	 * 3 - <b>FOV_SHADOW</b>: based on
	 * http://roguebasin.roguelikedevelopment.org/index.php?
	 * title=FOV_using_recursive_shadowcasting this algorithm.
	 *
	 * 4 - <b>FOV_PERMISSIVE_x</b>: based on
	 * <a href="http://www.adammil.net/blog/v125_Roguelike_Vision_Algorithms.html#permissivecode">
	 * Permissive Code</a>
	 *
	 * 5- <b>FOV_RESTRICTIVE</b>: Mingos' Restrictive Precise Angle Shadowcasting (MRPAS).
	 * Original implementation http://umbrarumregnum.110mb.com/download/mrpas here.
	 *
	 * Comparison of the algorithms:
	 *
	 * Check http://roguecentral.org/libtcod/fov/fov.pdf this.
	 */
	class Map : public VectorCell
	{

	public :

		int width = 0;
		int height = 0;

		Map() = default;

		/**
		 * @brief Building the map.
		 *
		 * First, you have to allocate a map of the same size as your dungeon.
		 *
		 * @param width The size of the map (in map cells).
		 * @param height The size of the map (in map cells).
		 */
		Map(int width, int height);

		/**
		 * @brief Defining the cell properties.
		 *
		 * Then, build your dungeon by defining which cells let the light pass
		 * and which cells are walkable.
		 *
		 * @note (by default, all cells block the light)
		 * @note (by default, all cells are not-walkable).
		 *
		 * @param x Coordinate of the cell that we want to update.
		 * @param y Coordinate of the cell that we want to update.
		 *
		 * @param isTransparent If true, this cell will let the light pass else
		 * it will block the light.
		 *
		 * @param isWalkable If true, creatures can walk true this cell (it is
		 * not a wall).
		 */
		void setProperties(int x, int y, bool isTransparent, bool isWalkable);

		/**
		 * Marked the cell as visible in the field of view.
		 *
		 * @param x Coordinate of the cell that we want to update.
		 * @param y Coordinate of the cell that we want to update.
		 */
		void setVisibleFieldView(int x, int y);

		/**
		 * @brief Clearing the map.
		 *
		 * You can clear an existing map (setting all cells to the chosen
		 * walkable/transparent values).
		 *
		 * @param transparent Whether the cells should be transparent.
		 * @param walkable Whether the cells should be walkable.
		 */
		void clear(bool transparent = false, bool walkable = false);

		/**
		 * @brief Clearing the fov property of map.
		 *
		 * Set to false the field of view of all the cell in the map.
		 *
		 * This method is used for re-calculate the field of view
		 * when the entity has been moved.
		 */
		void resetFieldView();

		/**
		 * @brief Copying a map.
		 *
		 * You can copy an existing map into another. You have to allocate
		 * the destination map first.
		 *
		 * @param source The map containing the source data.
		 */
		void copy(const Map& source);

		/**
		 * @brief Computing the field of view.
		 *
		 * Once your map is allocated and empty cells have been defined, you can
		 * calculate the field of view.
		 *
		 * @param playerX Position of the player in the map. 0 <= player_x < map width.
		 * @param playerY Position of the player in the map. 0 <= player_y < map height.
		 *
		 * @param maxRadius If > 0, the fov is only computed up to maxRadius cells
		 * away from the player. Else, the range is unlimited.
		 *
		 * @param light_walls Wether the wall cells near ground cells in fov must be
		 * in fov too.
		 *
		 * @param algo FOV algorithm to use. This are:
		 *
		 * 1- FOV_BASIC
		 * 2- FOV_DIAMOND
		 * 3- FOV_SHADOW
		 * 4- FOV_PERMISSIVE_x
		 * 5- FOV_RESTRICTIVE
		 */
		void computeFov(int playerX, int playerY, int maxRadius = 0, bool light_walls = true,
				TypeFOV algo = TypeFOV::Basic);

        /**
         * @brief Checking if a cell is in fov.
         *
         * Once your computed the field of view, you can know if a cell is visible.
         *
         * @param x Coordinates of the cell we want to check. 0 <= x < map width.
         * @param y Coordinates of the cell we want to check. 0 <= y < map height.
         *
         * @return True if the cell is visible inside of fov, false otherwise.
         */
		bool isVisibleFieldView(int x, int y) const;

        /**
         * @brief Checking a cell transparency/walkability.
         *
         * You can also retrieve transparent/walkable informations.
         *
         * @param x Coordinates of the cell we want to check. 0 <= x < map width.
         * @param y Coordinates of the cell we want to check. 0 <= y < map height.
         *
         * @return True if the cell is transparent/walkable inside
         * of fov, false otherwise.
         */
        bool isTransparent( int x, int y ) const;

        /**
         * @brief Checking a cell transparency/walkability.
         *
         * You can also retrieve transparent/walkable informations.
         *
         * @param x Coordinates of the cell we want to check. 0 <= x < map width.
         * @param y Coordinates of the cell we want to check. 0 <= y < map height.
         *
         * @return True if the cell is transparent/walkable inside
         * of fov, false otherwise.
         */
        bool isWalkable( int x, int y ) const;

		/**
		 * @return The width of the map.
		 */
		int getWidth() const;

		/**
		 * @return The height of the map.
		 */
		int getHeight() const;
	};

}


#endif

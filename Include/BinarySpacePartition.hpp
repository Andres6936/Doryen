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

#ifndef _TCOD_BSP_HPP
#define _TCOD_BSP_HPP

#include <BinarySpacePartitionListener.hpp>

namespace Doryen
{

	/**
	 * @brief This toolkit allows to create and manipulate
	 * 2D Binary Space Partition trees.
	 *
	 * They can be used to split a rectangular region into
	 * non overlapping sub-regions.
	 */
	class BinarySpacePartition : public Tree
	{

	public :

		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;

		int position = 0; // position of splitting
		bool horizontal = false; // horizontal splitting ?
		uint8 level = 0; // level in the tree

		/**
		 * @brief Creating a BSP tree
		 *
		 * Creating the root node
		 *
		 * First, you have to create the root node of the tree.
		 * This node encompasses the whole rectangular region.
		 *
		 */
		BinarySpacePartition() : level(0)
		{
		}

		/**
		 * @brief Creating a BSP tree
		 *
		 * Creating the root node
		 *
		 * First, you have to create the root node of the tree.
		 *
		 * @example BinarySpacePartition bsp = BinarySpacePartition(0, 0, 50, 50);
		 *
		 * This node encompasses the whole rectangular region.
		 *
		 * @param x Top left corner in coordinate x.
		 * @param y Top left corner in coordinate y.
		 * @param w Width of the rectangular region covered by the BSP tree.
		 * @param h Height of the rectangular region covered by the BSP tree.
		 */
		BinarySpacePartition(int x, int y, int w, int h) : x(x), y(y), w(w), h(h), level(0)
		{
		}

		/**
		 * You can delete a part of the tree, releasing resources for all sub nodes.
		 *
		 * @code
		 *
		 * BinarySpacePartition bsp = BinarySpacePartition(0, 0, 50, 50);
		 *
		 * // Create a tree
		 * bsp.splitRecursive(NULL, 4, 5, 5, 1.5f, 1.5f);
		 *
		 * // Clear it (keep only the root)
		 * bsp.removeSons();
		 *
		 * // And rebuild another random tree
		 * bsp.splitRecursive(NULL, 4, 5, 5, 1.5f, 1.5f);
		 *
		 * @endcode
		 */
		void removeSons();

		/**
		 * You can also completely delete the tree, including
		 * the root node to release every resource used.
		 *
		 * @code
		 *
		 * BinarySpacePartition *bsp = new BinarySpacePartition(0, 0, 50, 50);
		 *
		 * // Create a tree
		 * bsp->splitRecursive(NULL, 4, 5, 5, 1.5f, 1.5f);
		 *
		 * // Use the tree ...
		 *
		 * // Delete the tree
		 * delete bsp;
		 *
		 * @endcode
		 */
		virtual ~BinarySpacePartition();

		/**
		 * Splitting the tree.
		 *
		 * Splitting a node once.
		 *
		 * Once you have the root node, you can split it
		 * into two smaller non-overlapping nodes.
		 *
		 * @param horizontal If true, the node will be splitted horizontally, else, vertically.
		 * @param position Coordinate of the splitting position.
		 *
		 * If horizontal is true, x <= position < x + w
		 * Else, y <= position < y + h
		 *
		 * @code
		 *
		 * BinarySpacePartition bsp = BinarySpacePartition(0, 0, 50, 50);
		 *
		 * bsp.splitOnce(true, 20) // Horizontal split into two nodes : (0,0,50,20) and (0,20,50,30)
		 *
		 * @endcode
		 *
		 * Other example:
		 *
		 * @code
		 *
		 * BinarySpacePartition bsp = BinarySpacePartition(0, 0, 50, 50);
		 *
		 * bsp.splitOnce(false, 20) // Vertical split into two nodes : (0,0,20,50) and (20,0,30,50)
		 *
		 * @endcode
		 */
		void splitOnce(bool horizontal, int position);

		/**
		 * Recursively splitting a node
		 *
		 * You can also recursively split the BinarySpacePartition. At each step,
		 * a random orientation (horizontal/vertical) and position.
		 *
		 * @note A node is splitted only if the resulting
		 * sub-nodes are bigger than minHSize x minVSize.
		 *
		 * @note If a node does not conform, the splitting
		 * orientation is forced to reduce either the w/h
		 * or the h/w ratio. Use values near 1.0 in maxHRatio
		 * and maxVRatio to promote square nodes.
		 *
		 * @param randomizer The random number generator to use. Use NULL for the default one.
		 * @param nb Number of recursion levels.
		 * @param minHSize minimum width value for a node.
		 * @param minVSize minimum height value for a node.
		 * @param maxHRatio maximum width value for a node.
		 * @param maxVRatio maximum height value for a node.
		 *
		 * @example
		 *
		 * @code
		 *
		 * // Do a 4 levels BSP tree (the region is splitted into a
		 * // maximum of 2*2 * 2*2 sub-regions).
		 * BinarySpacePartition bsp = BinarySpacePartition(0, 0, 50, 50);
		 *
		 * bsp.splitRecursive(NULL, 4, 5, 5, 1.5f, 1.5f);
		 *
		 * @endcode
		 */
		void
		splitRecursive(TCODRandom* randomizer, int nb, int minHSize, int minVSize, float maxHRatio, float maxVRatio);

		/**
		 * Resizing a tree.
		 *
		 * This operation resets the size of the tree nodes
		 * without changing the splitting data
		 * (orientation/position). It should be called with
		 * the initial region size or a bigger size, else
		 * some splitting position may be out of the region.
		 *
		 * You can use it if you changed the nodes size and
		 * position while using the BSP tree, which happens
		 * typically when you use the tree to build a dungeon.
		 *
		 * You create rooms inside the tree leafs, then
		 * shrink the leaf to fit the room size.
		 * Calling resize on the root node with the
		 * original region size allows you to reset all
		 * nodes to their original size.
		 *
		 * @note The original rectangular area covered by the
		 * node should be included in the new one to ensure
		 * that every splitting edge stay inside its node.
		 *
		 * @param x New coordinate in x of the node.
		 * @param y New coordinate in x of the node.
		 * @param w New width of the node.
		 * @param h New height of the node.
		 *
		 * @example
		 *
		 * @code
		 *
		 * // We create a BSP, do some processing that will modify
		 * // the x, y, w, h fields of the tree nodes, then
		 * // reset all the nodes to their original size.
		 * BinarySpacePartition bsp = BinarySpacePartition(0, 0, 50, 50);
		 *
		 * bsp.splitRecursive(NULL, 4, 5, 5, 1.5f, 1.5f);
		 *
		 * // ... do something with the tree here
		 *
		 * bsp.resize(0,0,50,50);
		 *
		 * @endcode
		 */
		void resize(int x, int y, int w, int h);

		/**
		@PageName bsp_read
		@PageFather bsp
		@PageTitle Reading information from the tree
		@FuncDesc Once you have built a BSP tree, you can retrieve information from any node. The node gives you free access to its fields :
		@Cpp
			class TCODBsp {
			public :
				int x,y,w,h; //
				int position; // position of splitting
				bool horizontal; // horizontal splitting ?
				uint8 level; // level in the tree
				...
			}
		@C
			typedef struct {
				int x,y,w,h;
				int position;
				bool horizontal;
				uint8 level;
				...
			} TCOD_bsp_t;
		@C#
			class TCODBsp
			{
			  public int x { get; set; }
			  public int y { get; set; }
			  public int h { get; set; }
			  public int w { get; set; }
			  public int position { get; set; }
			  public bool horizontal { get; set; }
			  public byte level { get; set; }
			}
		@Param x,y,w,h	Rectangular region covered by this node.
		@Param position	If this node is not a leaf, splitting position.
		@Param horizontal	If this node is not a leaf, splitting orientation.
		@Param level	Level in the BSP tree (0 for the root, 1 for the root's sons, ...).
		*/

		/**
		@PageName bsp_read
		@FuncTitle Navigate in the tree
		@FuncDesc You can navigate from a node to its sons or its parent using one of those functions. Each function returns NULL if the corresponding node does not exists (if the node is not splitted for getLeft and getRight, and if the node is the root node for getFather).
		@Cpp
			TCODBsp *TCODBsp::getLeft() const
			TCODBsp *TCODBsp::getRight() const
			TCODBsp *TCODBsp::getFather() const
		@C
			TCOD_bsp_t * TCOD_bsp_left(TCOD_bsp_t *node)
			TCOD_bsp_t * TCOD_bsp_right(TCOD_bsp_t *node)
			TCOD_bsp_t * TCOD_bsp_father(TCOD_bsp_t *node)
		@Py
			bsp_left(node)
			bsp_right(node)
			bsp_father(node)
		@C#
			TCODBsp TCODBsp::getLeft()
			TCODBsp TCODBsp::getRight()
			TCODBsp TCODBsp::getFather()
		@Param node	In the C version, the node reference.
		*/
		BinarySpacePartition* getLeft() const
		{
			return (BinarySpacePartition*)sons;
		}

		BinarySpacePartition* getRight() const
		{
			return sons ? (BinarySpacePartition*)(sons->next) : NULL;
		}

		BinarySpacePartition* getFather() const
		{
			return (BinarySpacePartition*)father;
		}

		/**
		@PageName bsp_read
		@FuncTitle Checking if a node is a leaf
		@FuncDesc You can know if a node is a leaf (not splitted, no sons) with this function :
		@Cpp bool TCODBsp::isLeaf() const
		@C bool TCOD_bsp_is_leaf(TCOD_bsp_t *node)
		@Py bsp_is_leaf(node)
		@C# bool TCODBsp::isLeaf()
		*/
		bool isLeaf() const
		{
			return sons == NULL;
		}

		/**
		@PageName bsp_read
		@FuncTitle Check if a cell is inside a node
		@FuncDesc You can check if a map cell is inside a node.
		@Cpp bool TCODBsp::contains(int cx, int cy) const
		@C bool TCOD_bsp_contains(TCOD_bsp_t *node, int cx, int cy)
		@Py bsp_contains(node, cx, cy)
		@C# bool TCODBsp::contains(int x, int y)
		@Param node	In the C version, the node reference.
		@Param cx,cy	Map cell coordinates.
		*/
		bool contains(int x, int y) const;

		/**
		@PageName bsp_read
		@FuncTitle Getting the node containing a cell
		@FuncDesc You can search the tree for the smallest node containing a map cell. If the cell is outside the tree, the function returns NULL :
		@Cpp TCODBsp *TCODBsp::findNode(int cx, int cy)
		@C TCOD_bsp_t * TCOD_bsp_find_node(TCOD_bsp_t *node, int cx, int cy)
		@Py bsp_find_node(node, cx, cy)
		@C# TCODBsp TCODBsp::findNode(int x, int y)
		@Param node	In the C version, the node reference.
		@Param cx,cy	Map cell coordinates.
		*/
		BinarySpacePartition* findNode(int x, int y);

		/**
		@PageName bsp_traverse
		@PageFather bsp
		@PageTitle Traversing the tree
		@FuncDesc You can scan all the nodes of the tree and have a custom function called back for each node.
			Each traversal function returns false if the traversal has been interrupted (a callback returned false).
			* Pre-order : the callback is called for the current node, then for the left son, then for the right son.
			* In-order : the callback is called for the left son, then for current node, then for the right son.
			* Post-order : the callback is called for the left son, then for the right son, then for the current node.
			* Level-order : the callback is called for the nodes level by level, from left to right.
			* Inverted level-order : the callback is called in the exact inverse order as Level-order.
			<table class="param">
			<tbody><tr><th>Pre order</th><th>In order</th><th>Post order</th><th>Level order</th><th>Inverted level<br>order</th></tr>
			<tr><td><img src="bsp_preorder.png"></td><td><img src="bsp_inorder.png"></td><td><img src="bsp_postorder.png"></td><td><img src="bsp_levelorder.png"></td><td><img src="bsp_invlevelorder.png"></td></tr>
			</tbody></table>
		@Cpp
			class ITCODBspCallback {
			public :
				virtual bool visitNode(TCODBsp *node, void *userData) = 0;
			};

			bool TCODBsp::traversePreOrder(ITCODBspCallback *callback, void *userData)
			bool TCODBsp::traverseInOrder(ITCODBspCallback *callback, void *userData)
			bool TCODBsp::traversePostOrder(ITCODBspCallback *callback, void *userData)
			bool TCODBsp::traverseLevelOrder(ITCODBspCallback *callback, void *userData)
			bool TCODBsp::traverseInvertedLevelOrder(ITCODBspCallback *callback, void *userData)
		@C
			typedef bool (*TCOD_bsp_callback_t)(TCOD_bsp_t *node, void *userData)

			bool TCOD_bsp_traverse_pre_order(TCOD_bsp_t *node, TCOD_bsp_callback_t callback, void *userData)
			bool TCOD_bsp_traverse_in_order(TCOD_bsp_t *node, TCOD_bsp_callback_t callback, void *userData)
			bool TCOD_bsp_traverse_post_order(TCOD_bsp_t *node, TCOD_bsp_callback_t callback, void *userData)
			bool TCOD_bsp_traverse_level_order(TCOD_bsp_t *node, TCOD_bsp_callback_t callback, void *userData)
			bool TCOD_bsp_traverse_inverted_level_order(TCOD_bsp_t *node, TCOD_bsp_callback_t callback, void *userData)
		@Py
			def bsp_callback(node, userData) : # ...

			bsp_traverse_pre_order(node, callback, userData=0)
			bsp_traverse_in_order(node, callback, userData=0)
			bsp_traverse_post_order(node, callback, userData=0)
			bsp_traverse_level_order(node, callback, userData=0)
			bsp_traverse_inverted_level_order(node, callback, userData=0)
		@C#
			bool TCODBsp::traversePreOrder(ITCODBspCallback callback)
			bool TCODBsp::traverseInOrder(ITCODBspCallback callback)
			bool TCODBsp::traversePostOrder(ITCODBspCallback callback)
			bool TCODBsp::traverseLevelOrder(ITCODBspCallback callback)
			bool TCODBsp::traverseInvertedLevelOrder(ITCODBspCallback callback)
		@Param node	In the C version, the node reference (generally, the root node).
		@Param callback	The function to call for each node.
			It receives the current node and the custom data as parameters
			If it returns false, the traversal is interrupted.
		@Param userData	Custom data to pass to the callback.
		@CppEx
			class MyCallback : public ITCODBspCallback {
			public :
				bool visitNode(TCODBsp *node, void *userData) {
					printf("node pos %dx%d size %dx%d level %d\n",node->x,node->y,node->w,node->h,node->level);
					return true;
				}
			};
			myBSP->traversePostOrder(new MyListener(),NULL);
		@CEx
			bool my_callback(TCOD_bsp_t *node, void *userData) {
				printf("node pos %dx%d size %dx%d level %d\n",node->x,node->y,node->w,node->h,node->level);
				return true;
			}
			TCOD_bsp_traverse_post_order(my_bsp,my_callback,NULL);
		@PyEx
			def my_callback(node, userData) :
				print "node pos %dx%d size %dx%d level %d"%(node.x,node.y,node.w,node.h,node.level))
				return True
			libtcod.bsp_traverse_post_order(my_bsp,my_callback)
		*/
		bool traversePreOrder(ITCODBspCallback* listener, void* userData);

		bool traverseInOrder(ITCODBspCallback* listener, void* userData);

		bool traversePostOrder(ITCODBspCallback* listener, void* userData);

		bool traverseLevelOrder(ITCODBspCallback* listener, void* userData);

		bool traverseInvertedLevelOrder(ITCODBspCallback* listener, void* userData);

	protected :
		BinarySpacePartition(BinarySpacePartition* father, bool left);

	};

}

#endif

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

#include <vector>

#include "Doryen/Random/Number.hpp"
#include "Doryen/Algorithms/Generation/Dungeon/BinarySpacePartition.hpp"
#include "Doryen/Algorithms/Generation/Dungeon/Util/BinarySpacePartitionListener.hpp"

Doryen::Algorithms::BinarySpacePartition::BinarySpacePartition(BinarySpacePartition* father, bool left)
{
	if (father->horizontal)
	{
		x = father->x;
		w = father->w;
		y = left ? father->y : father->position;
		h = left ? father->position - y : father->y + father->h - father->position;
	}
	else
	{
		y = father->y;
		h = father->h;
		x = left ? father->x : father->position;
		w = left ? father->position - x : father->x + father->w - father->position;
	}
	level=father->level+1;
}

Doryen::Algorithms::BinarySpacePartition::~BinarySpacePartition()
{
	removeSons();
}

bool Doryen::Algorithms::BinarySpacePartition::traversePreOrder(ITCODBspCallback* listener, void* userData)
{
	if (!listener->visitNode(this, userData))
	{ return false; }
	if (getLeft() && !getLeft()->traversePreOrder(listener, userData))
	{ return false; }
	if (getRight() && !getRight()->traversePreOrder(listener, userData))
	{ return false; }
	return true;
}

bool Doryen::Algorithms::BinarySpacePartition::traverseInOrder(ITCODBspCallback* listener, void* userData)
{
	if (getLeft() && !getLeft()->traverseInOrder(listener, userData))
	{ return false; }
	if (!listener->visitNode(this, userData))
	{ return false; }
	if (getRight() && !getRight()->traverseInOrder(listener, userData))
	{ return false; }
	return true;
}

bool Doryen::Algorithms::BinarySpacePartition::traversePostOrder(ITCODBspCallback* listener, void* userData)
{
	if (getLeft() && !getLeft()->traversePostOrder(listener, userData))
	{ return false; }
	if (getRight() && !getRight()->traversePostOrder(listener, userData))
	{ return false; }
	if (!listener->visitNode(this, userData))
	{ return false; }
	return true;
}

bool Doryen::Algorithms::BinarySpacePartition::traverseLevelOrder(ITCODBspCallback* listener, void* userData)
{
	std::vector<BinarySpacePartition*> stack;
	stack.push_back(this);
	while (!stack.empty())
	{
		BinarySpacePartition* node = stack.at(0);
		stack.erase(stack.begin());

		if (node->getLeft())
		{ stack.push_back(node->getLeft()); }
		if (node->getRight())
		{ stack.push_back(node->getRight()); }
		if (!listener->visitNode(node, userData))
		{ return false; }
	}
	return true;
}

bool Doryen::Algorithms::BinarySpacePartition::traverseInvertedLevelOrder(ITCODBspCallback* listener, void* userData)
{
	std::vector<BinarySpacePartition*> stack1;
	std::vector<BinarySpacePartition*> stack2;

	stack1.push_back(this);

	while (!stack1.empty())
	{
		BinarySpacePartition* node = stack1.at(0);
		stack2.push_back(node);

		stack1.erase(stack1.begin());

		if (node->getLeft())
		{ stack1.push_back(node->getLeft()); }
		if (node->getRight())
		{ stack1.push_back(node->getRight()); }
	}
	while (!stack2.empty())
	{
		BinarySpacePartition* node = stack2.at(stack2.size() - 1);
		stack2.pop_back();

		if (!listener->visitNode(node, userData))
		{ return false; }
	}
	return true;
}

void Doryen::Algorithms::BinarySpacePartition::removeSons()
{
	BinarySpacePartition* node = (BinarySpacePartition*)sons;
	while (node)
	{
		BinarySpacePartition* nextNode = (BinarySpacePartition*)node->next;
		node->removeSons();
		delete node;
		node = nextNode;
	}
	sons = NULL;
}

void Doryen::Algorithms::BinarySpacePartition::splitOnce(bool horizontal, int position)
{
	this->horizontal = horizontal;
	this->position = position;
	addSon(new BinarySpacePartition(this, true));
	addSon(new BinarySpacePartition(this, false));
}

void
Doryen::Algorithms::BinarySpacePartition::splitRecursive(TCODRandom* randomizer, int nb, int minHSize, int minVSize,
		float maxHRatio, float maxVRatio)
{
	if (nb == 0 || (w < 2 * minHSize && h < 2 * minVSize))
	{ return; }
	bool horiz;
	if (!randomizer)
	{ randomizer = TCODRandom::getInstance(); }
	// promote square rooms
	if (h < 2 * minVSize || w > h * maxHRatio)
	{ horiz = false; }
	else if (w < 2 * minHSize || h > w * maxVRatio)
	{ horiz = true; }
	else
	{ horiz = randomizer->getInt(0, 1) == 0; }
	int position;
	if (horiz)
	{
		position = randomizer->getInt(y + minVSize, y + h - minVSize);
	} else {
		position = randomizer->getInt(x+minHSize,x+w-minHSize);
	}
	splitOnce(horiz,position);
	getLeft()->splitRecursive(randomizer,nb-1,minHSize,minVSize,maxHRatio,maxVRatio);
	getRight()->splitRecursive(randomizer,nb-1,minHSize,minVSize,maxHRatio,maxVRatio);
}

void Doryen::Algorithms::BinarySpacePartition::resize(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	if (getLeft())
	{
		if (horizontal)
		{
			getLeft()->resize(x, y, w, position - y);
			getRight()->resize(x, position, w, y + h - position);
		}
		else
		{
			getLeft()->resize(x, y, position - x, h);
			getRight()->resize(position,y,x+w-position,h);
		}
	}
}

bool Doryen::Algorithms::BinarySpacePartition::contains(int px, int py) const
{
	return (px >= x && py >= y && px < x + w && py < y + h);
}

Doryen::Algorithms::BinarySpacePartition* Doryen::Algorithms::BinarySpacePartition::findNode(int px, int py)
{
	if (!contains(px, py))
	{ return NULL; }
	if (!isLeaf())
	{
		BinarySpacePartition* left, * right;
		left = getLeft();
		if (left->contains(px, py))
		{ return left->findNode(px, py); }
		right = getRight();
		if (right->contains(px, py))
		{ return right->findNode(px, py); }
	}
	return this;
}


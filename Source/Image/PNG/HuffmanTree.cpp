#include "Image/PNG/HuffmanTree.hpp"

void HuffmanTree::getTreeInflateFixed(
		HuffmanTree& tree_ll,
		HuffmanTree& tree_d)
{
	tree_ll.generateFixedLitLenTree();
}

void HuffmanTree::generateFixedLitLenTree()
{
	// given the code lengths (as stored in the PNG file), generate
	// the tree as defined by Deflate. maxbitlen is the maximum bits
	// that a code in the tree can have.

	this->lengths.reserve(NUM_DEFLATE_CODE_SYMBOLS);


	// 288 possible codes:
	// 		0-255   = literals,
	// 		256     = endcode,
	// 		257-285 = lengthcodes,
	// 		286-287 = unused

	for (unsigned i = 0; i <= 143; i++)
	{
		this->lengths.push_back(8);
	}

	for (unsigned i = 144; i <= 255; i++)
	{
		this->lengths.push_back(9);
	}

	for (unsigned i = 256; i <= 279; i++)
	{
		this->lengths.push_back(7);
	}

	for (unsigned i = 280; i <= 287; i++)
	{
		this->lengths.push_back(8);
	}

	// Second step for the generateFixedLitLenTree function.
	// numcodes, lengths and maxbitlen must already be filled
	// in correctly.

	this->tree1d.reserve(NUM_DEFLATE_CODE_SYMBOLS);

	std::vector <unsigned> blcount;
	std::vector <unsigned> nextcode;

	blcount.reserve(NUM_DEFLATE_CODE_SYMBOLS + 1);
	nextcode.reserve(NUM_DEFLATE_CODE_SYMBOLS + 1);

	// step 1: count number of instances of each code length
	for (unsigned bits = 0; bits < NUM_DEFLATE_CODE_SYMBOLS; bits++)
	{
		blcount[this->lengths[bits]]++;
	}

	// step 2: generate the nextcode values
	for (unsigned bits = 0; bits < NUM_DEFLATE_CODE_SYMBOLS; bits++)
	{
		nextcode[bits] = (nextcode[bits - 1] + blcount[bits - 1]) << 1;
	}

	// step 3: generate all the codes
	for (unsigned n = 0; n < NUM_DEFLATE_CODE_SYMBOLS; n++)
	{
		if (this->lengths[n] != 0)
		{
			this->tree1d[n] = nextcode[this->lengths[n]]++;
		}
	}

	// the tree representation used by the decoder.
	this->tree2d.reserve(NUM_DEFLATE_CODE_SYMBOLS * 2);

	// convert tree1d[] to tree2d[][]. In the 2D array, a value of
	// 32767 means uninited, a value >= numcodes is an address to
	// another bit, a value < numcodes is a code.
	//
	// The 2 rows are the 2 possible bit values (0 or 1), there are
	// as many columns as codes - 1.
	//
	// A good huffmann tree has N * 2 - 1 nodes, of which N - 1 are
	// internal nodes.
	//
	// Here, the internal nodes are stored (what their 0 and 1 option
	// point to).

	// There is only memory for such good tree currently, if there are
	// more nodes (due to too long length codes), error 55 will happen.

	for (unsigned n = 0; n < NUM_DEFLATE_CODE_SYMBOLS * 2; n++)
	{
		// 32'767 here means the tree2d isn't filled there yet
		this->tree2d.push_back(32'767);
	}

	// position in the tree (1 of the numcodes columns)
	unsigned treepos = 0;

	// up to which node it is filled
	unsigned nodefilled = 0;

	// the codes
	for (unsigned n = 0; n < NUM_DEFLATE_CODE_SYMBOLS; n++)
	{
		// the bits for this code
		for (unsigned i = 0; i < this->lengths[n]; i++)
		{
			auto bit = (unsigned char)((this->tree1d[n] >> (this->lengths[n] - i - 1)) & 1);

			if (treepos > NUM_DEFLATE_CODE_SYMBOLS - 2)
			{
				// TODO: Throw Exception
				//  oversubscribed, see comment in lodepng_error_text
				return;
			}

			// not yet filled in
			if (this->tree2d[2 * treepos + bit] == 32767)
			{
				// last bit
				if (i + 1 == this->lengths[n])
				{
					// put the current code in it
					this->tree2d[2 * treepos + bit] = n;
					treepos = 0;
				}
				else
				{
					// put address of the next step in here, first
					// that address has to be found of course
					// (it's just nodefilled + 1)...
					nodefilled++;

					// addresses encoded with numcodes added to it
					this->tree2d[2 * treepos + bit] = nodefilled + this->numcodes;
					treepos = nodefilled;
				}
			}
			else
			{
				treepos = this->tree2d[2 * treepos + bit] - this->numcodes;
			}
		}
	}

	for (unsigned n = 0; n < NUM_DEFLATE_CODE_SYMBOLS * 2; n++)
	{
		if (this->tree2d[n] == 32'767)
		{
			// remove possible remaining 32767's
			this->tree2d[n] = 0;
		}
	}
}

#include "Image/PNG/HuffmanTree.hpp"

void HuffmanTree::getTreeInflateFixed(
		HuffmanTree& tree_ll,
		HuffmanTree& tree_d)
{
	tree_ll.generateFixedLitLenTree();
	tree_d.generateFixedDistanceTree();
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

	makeTreeDimensional();

	makeTreeMultiDimensional();
}

void HuffmanTree::generateFixedDistanceTree()
{
	this->lengths.reserve(NUM_DEFLATE_CODE_SYMBOLS);

	for (int i = 0; i < NUM_DEFLATE_CODE_SYMBOLS; ++i)
	{
		this->lengths.push_back(5);
	}

	makeTreeDimensional();

	makeTreeMultiDimensional();
}

void HuffmanTree::makeTreeDimensional()
{
	// Second step for the generateFixedLitLenTree function.
	// numcodes, lengths and maxbitlen must already be filled
	// in correctly.

	this->tree1d.reserve(this->numcodes);

	std::vector <unsigned> blcount;
	std::vector <unsigned> nextcode;

	blcount.reserve(this->maxbitlen + 1);
	nextcode.reserve(this->maxbitlen + 1);

	// step 1: count number of instances of each code length
	for (unsigned bits = 0; bits < this->numcodes; bits++)
	{
		blcount[this->lengths[bits]]++;
	}

	// step 2: generate the nextcode values
	for (unsigned bits = 1; bits <= this->maxbitlen; bits++)
	{
		nextcode[bits] = (nextcode[bits - 1] + blcount[bits - 1]) << 1;
	}

	// step 3: generate all the codes
	for (unsigned n = 0; n < this->numcodes; n++)
	{
		if (this->lengths[n] != 0)
		{
			this->tree1d[n] = nextcode[this->lengths[n]]++;
		}
	}
}

void HuffmanTree::makeTreeMultiDimensional()
{
	// the tree representation used by the decoder.
	this->tree2d.reserve(this->numcodes * 2);

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

	for (unsigned n = 0; n < this->numcodes * 2; n++)
	{
		// 32'767 here means the tree2d isn't filled there yet
		this->tree2d.push_back(32'767);
	}

	// position in the tree (1 of the numcodes columns)
	unsigned treepos = 0;

	// up to which node it is filled
	unsigned nodefilled = 0;

	// the codes
	for (unsigned n = 0; n < this->numcodes; n++)
	{
		// the bits for this code
		for (unsigned i = 0; i < this->lengths[n]; i++)
		{
			auto bit = (unsigned char)((this->tree1d[n] >>
														(this->lengths[n] - i - 1)) & 1);

			if (treepos > this->numcodes - 2)
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
					this->tree2d[2 * treepos + bit] = nodefilled +
													  this->numcodes;

					treepos = nodefilled;
				}
			}
			else
			{
				treepos = this->tree2d[2 * treepos +
									   bit] - this->numcodes;
			}
		}
	}

	for (unsigned n = 0; n < this->numcodes * 2; n++)
	{
		if (this->tree2d[n] == 32'767)
		{
			// remove possible remaining 32767's
			this->tree2d[n] = 0;
		}
	}
}

void HuffmanTree::getTreeInflateDynamic(
		HuffmanTree* tree_ll, HuffmanTree* tree_d,
		const std::vector <unsigned>& in, size_t* bp)
{
	if ((*bp) >> 3 >= in.size() - 2)
	{
		// TODO: Throw Exception
		//  error: the bit pointer is or will go past the memory
	}

	// number of literal/length codes + 257.
	// Unlike the spec, the value 257 is added to it here already
	unsigned HLIT = readBitsFromStream(bp, in, 5) + 257;

	// number of distance codes.
	// Unlike the spec, the value 1 is added to it here already
	unsigned HDIST = readBitsFromStream(bp, in, 5) + 1;

	// number of code length codes.
	// Unlike the spec, the value 4 is added to it here already
	unsigned HCLEN = readBitsFromStream(bp, in, 4) + 4;

	bool error = false;

	while (!error)
	{
		// read the code length codes out of 3 * (amount of
		// code length codes) bits

		// see comments in deflateDynamic for explanation of
		// the context and these variables, it is analogous

		// lit,len code lengths
		std::vector <unsigned> bitlen_ll;

		// dist code lengths
		std::vector <unsigned> bitlen_d;

		// code length code lengths ("clcl"), the bit lengths of
		// the huffman tree used to compress bitlen_ll and bitlen_d
		std::vector <unsigned> bitlen_cl;

		// the code tree for code length codes (the huffman
		// tree for compressed huffman trees)
		HuffmanTree tree_cl;

		bitlen_cl.reserve(NUM_CODE_LENGTH_CODES);

		for (int i = 0; i < NUM_CODE_LENGTH_CODES; ++i)
		{
			bitlen_cl.push_back(0);
		}

		for (int i = 0; i < NUM_CODE_LENGTH_CODES; ++i)
		{
			if (i < HCLEN)
			{
				bitlen_cl.at(CLCL_ORDER[i]) = readBitsFromStream(
						bp, in, 3);
			}
			else
			{
				// if not, it must stay 0
				bitlen_cl.at(CLCL_ORDER[i]) = 0;
			}
		}

		// given the code lengths (as stored in the PNG file),
		// generate the tree as defined by Deflate.
		// maxbitlen is the maximum bits that a code in the
		// tree can have.

		tree_cl.lengths.reserve(NUM_CODE_LENGTH_CODES);

		for (int i = 0; i < NUM_CODE_LENGTH_CODES; ++i)
		{
			tree_cl.lengths.push_back(bitlen_cl[i]);
		}

		tree_cl.numcodes = NUM_CODE_LENGTH_CODES;
		tree_cl.maxbitlen = 7;

		tree_cl.makeTreeDimensional();
		tree_cl.makeTreeMultiDimensional();

		// now we can use this tree to read the lengths for
		// the tree that this function will return

		bitlen_ll.reserve(NUM_DEFLATE_CODE_SYMBOLS);
		bitlen_d.reserve(NUM_DISTANCE_SYMBOLS);

		for (int i = 0; i < NUM_DEFLATE_CODE_SYMBOLS; ++i)
		{
			bitlen_ll.push_back(0);
		}

		for (int i = 0; i < NUM_DISTANCE_SYMBOLS; ++i)
		{
			bitlen_d.push_back(0);
		}

		// i is the current symbol we're reading in the
		// part that contains the code lengths of lit/len
		// and dist codes

		unsigned i = 0;

		while (i < HLIT + HDIST)
		{
			// TODO: Implemente huffmanDecodeSymbol
		}
	}

}

unsigned HuffmanTree::readBitsFromStream(
		size_t* bitpointer,
		const std::vector <unsigned>& bitstream,
		size_t nbits)
{
	unsigned result = 0;

	for (int i = 0; i < nbits; ++i)
	{
		result += (unsigned)((bitstream[*bitpointer >> 3] >>
														  (*bitpointer & 0x7)) & (unsigned char)1) << i;

		(*bitpointer)++;
	}

	return result;
}

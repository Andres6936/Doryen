#include "Image/PNG/HuffmanTree.hpp"

// Static Members

unsigned HuffmanTree::CLCL_ORDER[NUM_CODE_LENGTH_CODES] =
		{ 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

// Methods

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

	this->tree1d.resize(this->numcodes, 0);

	std::vector <unsigned> blcount;
	std::vector <unsigned> nextcode;

	blcount.resize(this->maxbitlen + 1, 0);
	nextcode.resize(this->maxbitlen + 1, 0);

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

	// the tree representation used by the decoder.
	// 32'767 here means the tree2d isn't filled there yet
	this->tree2d.resize(this->numcodes * 2, 32'767);

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
		HuffmanTree& tree_ll, HuffmanTree& tree_d,
		const std::vector <unsigned char>& in, size_t* bp)
{
	// make sure that length values that aren't
	// filled in will be 0, or a wrong tree will
	// be generated

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

	unsigned error = 0;

	size_t inbitlength = in.size() * 8;

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

		bitlen_cl.resize(NUM_CODE_LENGTH_CODES, 0);

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
			unsigned code = tree_cl.huffmanDecodeSymbol(in, bp, inbitlength);

			// a length code
			if (code <= 15)
			{
				if (i < HLIT)
				{
					bitlen_ll[i] = code;
				}
				else
				{
					bitlen_d[i - HLIT] = code;
				}

				i++;
			}
				// repeat previous
			else if (code == 16)
			{
				// read in the 2 bits that indicate
				// repeat length (3-6)
				unsigned replength = 3;

				if (*bp >= inbitlength)
				{
					// TODO: error, bit pointer jumps past memory
					return;
				}

				if (i == 0)
				{
					// TODO: can't repeat previous if i is 0
					return;
				}

				replength += readBitsFromStream(bp, in, 2);

				unsigned value = 0;

				if (i < HLIT + 1)
				{
					value = bitlen_ll[i - 1];
				}
				else
				{
					value = bitlen_d[i - HLIT - 1];
				}

				for (int n = 0; n < replength; ++n)
				{
					if (i >= HLIT + HDIST)
					{
						// TODO: error: i is larger than
						//  the amount of codes
						return;
					}

					if (i < HLIT)
					{
						bitlen_ll[i] = value;
					}
					else
					{
						bitlen_d[i - HLIT] = value;
					}

					i++;
				}
			}
				// repeat "0" 3-10 times
			else if (code == 17)
			{
				unsigned replength = 3;

				if (*bp >= inbitlength)
				{
					// TODO: error, bit pointer jumps past memory
					return;
				}

				replength += readBitsFromStream(bp, in, 3);

				// repeat this value in the next lengths
				for (int n = 0; n < replength; n++)
				{
					if (i >= HLIT + HDIST)
					{
						// TODO: error: i is larger than the
						//  amount of codes
					}

					if (i < HLIT)
					{
						bitlen_ll[i] = 0;
					}
					else
					{
						bitlen_d[i - HLIT] = 0;
					}

					i++;
				}
			}
				// repeat "0" 11-138 times
			else if (code == 18)
			{
				// read in the bits that indicate repeat length
				unsigned replength = 11;

				if (*bp >= inbitlength)
				{
					// TODO: error, bit pointer jumps past memory
					return;
				}

				replength += readBitsFromStream(bp, in, 7);

				// repeat this value in the next lengths*/
				for (int n = 0; n < replength; n++)
				{
					if (i >= HLIT + HDIST)
					{
						// TODO: error: i is larger than the
						//  amount of codes
					}

					if (i < HLIT)
					{
						bitlen_ll[i] = 0;
					}
					else
					{
						bitlen_d[i - HLIT] = 0;
					}

					i++;
				}
			}
			else
			{
				// huffmanDecodeSymbol returns (unsigned)(-1) in
				// case of error
				if (code == (unsigned)(-1))
				{
					// return error code 10 or 11 depending on
					// the situation that happened in
					// huffmanDecodeSymbol (10=no endcode,
					// 11=wrong jump outside of tree)

					if ((*bp) > inbitlength)
					{
						error = 10;
					}
					else
					{
						error = 11;
					}
				}
				else
				{
					// unexisting code, this can never happen
					error = 16;
				}

				break;
			}
		}

		if (error)
		{
			break;
		}

		if (bitlen_ll[256] == 0)
		{
			// TODO: the length of the end code 256
			//  must be larger than 0
			return;
		}

		// now we've finally got HLIT and HDIST, so generate
		// the code trees, and the function is done

		tree_ll.numcodes = NUM_DEFLATE_CODE_SYMBOLS;
		tree_ll.maxbitlen = 15;
		tree_ll.lengths.clear();
		tree_ll.lengths.reserve(tree_ll.numcodes);

		for (int j = 0; j < tree_ll.numcodes; ++j)
		{
			tree_ll.lengths.push_back(bitlen_ll[j]);
		}

		tree_ll.makeTreeDimensional();
		tree_ll.makeTreeMultiDimensional();

		// Another tree {tree_d}

		tree_d.numcodes = NUM_DISTANCE_SYMBOLS;
		tree_d.maxbitlen = 15;
		tree_d.lengths.clear();
		tree_d.lengths.reserve(tree_d.numcodes);

		for (int j = 0; j < tree_d.numcodes; ++j)
		{
			tree_d.lengths.push_back(bitlen_d[j]);
		}

		tree_d.makeTreeDimensional();
		tree_d.makeTreeMultiDimensional();

		break;
	}
}

unsigned HuffmanTree::readBitsFromStream(
		size_t* bitpointer,
		const std::vector <unsigned char>& bitstream,
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

unsigned HuffmanTree::huffmanDecodeSymbol(
		const std::vector <unsigned char>& in,
		size_t* bp, size_t inbitlength)
{
	unsigned treepos = 0;

	while (true)
	{
		if (*bp >= inbitlength)
		{
			// TODO: error: end of input memory reached
			//  without endcode
			return (unsigned)(-1);
		}

		// decode the symbol from the tree.

		// The "readBitFromStream" code is inlined in the
		// expression below because this is the biggest
		// bottleneck while decoding

		unsigned ct = this->tree2d[(treepos << 1) + ((in[*bp >> 3] >> (*bp & 0x7)) & (unsigned char)1)];

		(*bp)++;

		if (ct < this->numcodes)
		{
			// the symbol is decoded, return it
			return ct;
		}
		else
		{
			// symbol not yet decoded, instead move tree position
			treepos = ct - this->numcodes;
		}

		if (treepos >= this->numcodes)
		{
			// TODO: error: it appeared outside the codetree
			return (unsigned)(-1);
		}
	}
}

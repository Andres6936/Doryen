#ifndef LIBTCOD_HUFFMANTREE_HPP
#define LIBTCOD_HUFFMANTREE_HPP

#include <vector>

class HuffmanTree
{

private:

	/**
	 * 256 literals, the end code, some length
	 * codes, and 2 unused codes
	 */
	static constexpr short NUM_DEFLATE_CODE_SYMBOLS = 288;

public:

	std::vector <unsigned> tree2d;
	std::vector <unsigned> tree1d;

	/**
	 * the lengths of the codes of the 1d-tree.
	 */
	std::vector <unsigned> lengths;

	/**
	 * maximum number of bits a single code can get.
	 */
	unsigned maxbitlen = 15;

	/**
	 * number of symbols in the alphabet = number of codes.
	 */
	unsigned numcodes = NUM_DEFLATE_CODE_SYMBOLS;

	// Methods

	/**
	 * get the tree of a deflated block with fixed
	 * tree, as specified in the deflate specification.
	 */
	static void getTreeInflateFixed(
			HuffmanTree& tree_ll, HuffmanTree& tree_d);

	/**
	 * get the tree of a deflated block with dynamic tree,
	 * the tree itself is also Huffman compressed with a known tree
	 */
	static void getTreeInflateDynamic(
			HuffmanTree* tree_ll, HuffmanTree* tree_d,
			const std::vector <unsigned>& in, size_t* bp);

private:

	/**
	 * get the literal and length code tree of a
	 * deflated block with fixed tree, as per the
	 * deflate specification.
	 */
	void generateFixedLitLenTree();

	/**
	 * get the distance code tree of a deflated block wit
	 * fixed tree, as specified in the deflate specification
	 */
	void generateFixedDistanceTree();

	void makeTreeDimensional();

	void makeTreeMultiDimensional();

};


#endif //LIBTCOD_HUFFMANTREE_HPP

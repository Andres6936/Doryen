#ifndef LIBTCOD_CRC32TABLE_HPP
#define LIBTCOD_CRC32TABLE_HPP

#include <vector>

class CRC32Table
{

private:

	std::vector <unsigned> table;

public:

	/**
	 * Make the table for a fast CRC.
	 */
	CRC32Table()
	{
		table.reserve(256);

		for (unsigned int n = 0; n < 256; ++n)
		{
			unsigned c = n;

			for (unsigned int k = 0; k < 8; ++k)
			{
				if (c & 1)
				{
					c = 0xedb88320L ^ (c >> 1);
				}
				else
				{
					c = c >> 1;
				}
			}

			table.push_back(c);
		}
	}

	unsigned getIndex(unsigned index)
	{
		return table[index];
	}

};

#endif //LIBTCOD_CRC32TABLE_HPP

#include "Doryen/Image/PNG/Adler32.hpp"

Adler32::Adler32(const std::vector <unsigned char>& data)
{
	unsigned s1 = 1L & 0xffff;
	unsigned s2 = (1L >> 16) & 0xffff;

	unsigned len = data.size();

	unsigned index = 0;

	while (len > 0)
	{
		// at least 5550 sums can be done before the sums
		// overflow, saving a lot of module divisions
		unsigned amount = 0;

		if (len > 5550)
		{
			amount = 5550;
		}
		else
		{
			amount = len;
		}

		len -= amount;

		while (amount > 0)
		{
			s1 = (s1 + data.at(index));
			s2 = (s2 + s1);

			index++;
			amount--;
		}

		s1 %= 65'521;
		s2 %= 65'521;
	}

	adler = (s2 << 16) | s1;
}

unsigned int Adler32::getChecksum() const
{
	return adler;
}

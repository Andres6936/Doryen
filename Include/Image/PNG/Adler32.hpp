#ifndef LIBTCOD_ADLER32_HPP
#define LIBTCOD_ADLER32_HPP

#include <vector>

class Adler32
{

private:

	unsigned adler = 0;

public:

	// Construct

	explicit Adler32(const std::vector <unsigned char>& data);

	// Methods

	unsigned int getChecksum() const;

};


#endif //LIBTCOD_ADLER32_HPP

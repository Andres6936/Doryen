#ifndef LIBTCOD_LINELISTENER_HPP
#define LIBTCOD_LINELISTENER_HPP

#include <Doryen/Doryen.hpp>

#include "SampleRenderer.hpp"

class RenderLine : public Doryen::Algorithms::LineListener
{

private:

	Doryen::Console& console;

public :

	RenderLine(Doryen::Console& _console) : console(_console)
	{
	};

	bool drawPoint(int x, int y) override;
};


#endif //LIBTCOD_LINELISTENER_HPP

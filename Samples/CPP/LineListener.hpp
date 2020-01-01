#ifndef LIBTCOD_LINELISTENER_HPP
#define LIBTCOD_LINELISTENER_HPP

#include <libtcod.hpp>

#include "SampleRenderer.hpp"

class RenderLine : public Doryen::Algorithms::LineListener
{

public :

	bool drawPoint(int x, int y) override;
};


#endif //LIBTCOD_LINELISTENER_HPP

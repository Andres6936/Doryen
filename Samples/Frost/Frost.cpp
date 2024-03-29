#include "Frost.hpp"

#include <Doryen/Doryen.hpp>

Frost::Frost(int x, int y, FrostManager* nFrostManager) : x(x), y(y), manager(nFrostManager)
{
	bestx = 0;
	besty = 0;

	rx = 0;
	ry = 0;

	ra = 0;
    rr = 0;

    border = 0;
    timer = 0.0f;
}

bool Frost::update( float elapsed )
{
    for ( int i = PIX_PER_FRAME; i > 0; i-- )
    {
        timer -= elapsed;

        if ( timer <= 0 )
		{
			// find a new random frost direction
			ra = Random::Number::nextFloat(0.0f, 2 * 3.1415926f);
			rr = Random::Number::nextFloat(0, 2 * RANGE);

			timer = ANGLE_DELAY;

			rx = (int)(RANGE + rr * cosf(ra));
			ry = (int)(RANGE + rr * sinf(ra));

			int minDist = 100000;

			// find closest frost pixel
			for (int cx = 1; cx < 2 * RANGE; cx++)
            {
                if (( unsigned ) ( x - RANGE + cx ) < ( unsigned ) manager->w )
                {
                    for ( int cy = 1; cy < 2 * RANGE; cy++ )
                    {
                        if (( unsigned ) ( y - RANGE + cy ) < ( unsigned ) manager->h )
                        {
                            float f = getValue( cx, cy );
                            if ( f > FROST_LEVEL )
                            {
                                int dist = ( cx - rx ) * ( cx - rx ) + ( cy - ry ) * ( cy - ry );
                                if ( dist < minDist )
                                {
                                    minDist = dist;
                                    bestx = cx;
                                    besty = cy;
                                }
                            }
                        }
                    }
                }
            }
        }
        // smoothing
        for ( int cx = 0; cx < 2 * RANGE + 1; cx++ )
        {
            if ( x - RANGE + cx < manager->w - 1 && x - RANGE + cx > 0 )
            {
                for ( int cy = 0; cy < 2 * RANGE + 1; cy++ )
                {
                    if ( y - RANGE + cy < manager->h - 1 && y - RANGE + cy > 0 )
                    {
                        if ( getValue( cx, cy ) < 1.0f )
                        {
							float f = getValue(cx, cy);
							float oldf = f;
							f = std::max(f, getValue(cx + 1, cy));
							f = std::max(f, getValue(cx - 1, cy));
							f = std::max(f, getValue(cx, cy + 1));
							f = std::max(f, getValue(cx, cy - 1));
							setValue(cx, cy, oldf + (f - oldf) * SMOOTH * elapsed);
						}
                    }
                }
            }
        }
        int curx = bestx;
        int cury = besty;

        // frosting
        Doryen::Algorithms::Line objLine = Doryen::Algorithms::Line();
        objLine.init( curx, cury, rx, ry );
        objLine.step( &curx, &cury );

        if (( unsigned ) ( x - RANGE + curx ) < ( unsigned ) manager->w &&
            ( unsigned ) ( y - RANGE + cury ) < ( unsigned ) manager->h )
        {
            float f = getValue( curx, cury );
			f += GROW * elapsed;
			f = std::min(1.0f, f);
			setValue(curx, cury, f);
            if ( f == 1.0f )
            {
                bestx = curx;
                besty = cury;
                if ( bestx == rx && besty == ry )
                { timer = 0.0f; }
                timer = 0.0f;
                if ( curx == 0 || curx == 2 * RANGE || cury == 0 || cury == 2 * RANGE )
                {
                    border++;
                    if ( border == 20 )
                    { return false; }
                }
            }
        }
        else
        { timer = 0.0f; }
    }
    return true;
}

void Frost::render(Doryen::Image* img)
{
	const auto[w, h] = img->getSize().unpack();

	for (int cx = x - RANGE; cx <= x + RANGE; cx++)
	{
		if ((unsigned)cx < (unsigned)w)
		{
			for (int cy = y - RANGE; cy <= y + RANGE; cy++)
			{
				if ((unsigned)cy < (unsigned)h)
				{
					float f = getValue(cx - (x - RANGE), cy - (y - RANGE));
					int idx = (int)(f * 255);
					idx = std::min(255, idx);
					img->setPixel(cx, cy, manager->getFrostColor()[idx]);
				}
            }
        }
    }
}

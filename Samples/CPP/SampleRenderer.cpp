#include "SampleRenderer.hpp"

void SampleRenderer::render( void *sdlSurface )
{
    SDL_Surface *screen = ( SDL_Surface * ) sdlSurface;
    // now we have almighty access to the screen's precious pixels !!
    // get the font character size
    int charw, charh;
    Doryen::Platform::getCharSize( &charw, &charh );
    // compute the sample console position in pixels
    int samplex = SAMPLE_SCREEN_X * charw;
    int sampley = SAMPLE_SCREEN_Y * charh;
    delay -= Doryen::Platform::getLastFrameLength( );
    if ( delay < 0.0f )
    {
        delay = 3.0f;
        effectNum = ( effectNum + 1 ) % 3;

        if ( effectNum == 2 )
        {
            sdl_callback_enabled = false; // no forced redraw for burn effect
        }
        else
        { sdl_callback_enabled = true; }
    }
    switch ( effectNum )
    {
        case 0 :
            blur( screen, samplex, sampley, SAMPLE_SCREEN_WIDTH * charw, SAMPLE_SCREEN_HEIGHT * charh );
            break;
        case 1 :
            explode( screen, samplex, sampley, SAMPLE_SCREEN_WIDTH * charw, SAMPLE_SCREEN_HEIGHT * charh );
            break;
        case 2 :
            burn( screen, samplex, sampley, SAMPLE_SCREEN_WIDTH * charw, SAMPLE_SCREEN_HEIGHT * charh );
            break;
    }
}

void SampleRenderer::burn( SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh )
{
    int ridx = screen->format->Rshift / 8;
    int gidx = screen->format->Gshift / 8;
    int bidx = screen->format->Bshift / 8;
    for ( int x = samplex; x < samplex + samplew; x++ )
    {
        Uint8 *p = ( Uint8 * ) screen->pixels + x * screen->format->BytesPerPixel + sampley * screen->pitch;
        for ( int y = sampley; y < sampley + sampleh; y++ )
        {
            int ir = 0, ig = 0, ib = 0;
            Uint8 *p2 = p + screen->format->BytesPerPixel; // get pixel at x+1,y
            ir += p2[ ridx ];
            ig += p2[ gidx ];
            ib += p2[ bidx ];
            p2 -= 2 * screen->format->BytesPerPixel; // get pixel at x-1,y
            ir += p2[ ridx ];
            ig += p2[ gidx ];
            ib += p2[ bidx ];
            p2 += screen->format->BytesPerPixel + screen->pitch; // get pixel at x,y+1
            ir += p2[ ridx ];
            ig += p2[ gidx ];
            ib += p2[ bidx ];
            p2 -= 2 * screen->pitch; // get pixel at x,y-1
            ir += p2[ ridx ];
            ig += p2[ gidx ];
            ib += p2[ bidx ];
            ir /= 4;
            ig /= 4;
            ib /= 4;
            p[ ridx ] = ir;
            p[ gidx ] = ig;
            p[ bidx ] = ib;
            p += screen->pitch;
        }
    }
}

void SampleRenderer::explode( SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh )
{
    int ridx = screen->format->Rshift / 8;
    int gidx = screen->format->Gshift / 8;
    int bidx = screen->format->Bshift / 8;
    TCODRandom *rng = TCODRandom::getInstance( );
    int dist = ( int ) ( 10 * ( 3.0f - delay ));
    for ( int x = samplex; x < samplex + samplew; x++ )
    {
        Uint8 *p = ( Uint8 * ) screen->pixels + x * screen->format->BytesPerPixel + sampley * screen->pitch;
        for ( int y = sampley; y < sampley + sampleh; y++ )
        {
            int ir = 0, ig = 0, ib = 0;
            for ( int i = 0; i < 3; i++ )
            {
                int dx = rng->getInt( -dist, dist );
                int dy = rng->getInt( -dist, dist );
                Uint8 *p2;
                p2 = p + dx * screen->format->BytesPerPixel;
                p2 += dy * screen->pitch;
                ir += p2[ ridx ];
                ig += p2[ gidx ];
                ib += p2[ bidx ];
            }
            ir /= 3;
            ig /= 3;
            ib /= 3;
            p[ ridx ] = ir;
            p[ gidx ] = ig;
            p[ bidx ] = ib;
            p += screen->pitch;
        }
    }
}

void SampleRenderer::blur( SDL_Surface *screen, int samplex, int sampley, int samplew, int sampleh )
{
    // let's blur that sample console
    float f[3], n = 0.0f;
    int ridx = screen->format->Rshift / 8;
    int gidx = screen->format->Gshift / 8;
    int bidx = screen->format->Bshift / 8;
    f[ 2 ] = Doryen::Platform::getElapsedSeconds( );
    for ( int x = samplex; x < samplex + samplew; x++ )
    {
        Uint8 *p = ( Uint8 * ) screen->pixels + x * screen->format->BytesPerPixel + sampley * screen->pitch;
        f[ 0 ] = ( float ) ( x ) / samplew;
        for ( int y = sampley; y < sampley + sampleh; y++ )
        {
            int ir = 0, ig = 0, ib = 0;
            if (( y - sampley ) % 8 == 0 )
            {
                f[ 1 ] = ( float ) ( y ) / sampleh;
                n = noise->getFbm( f, 3.0f );
            }
            int dec = ( int ) ( 3 * ( n + 1.0f ));
            int count = 0;
            switch ( dec )
            {
                case 4:
                    count += 4;
                    // get pixel at x,y
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p -= 2 * screen->format->BytesPerPixel; // get pixel at x+2,y
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p -= 2 * screen->pitch; // get pixel at x+2,y+2
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p += 2 * screen->format->BytesPerPixel; // get pixel at x,y+2
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p += 2 * screen->pitch;
                case 3:
                    count += 4;
                    // get pixel at x,y
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p += 2 * screen->format->BytesPerPixel; // get pixel at x+2,y
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p += 2 * screen->pitch; // get pixel at x+2,y+2
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p -= 2 * screen->format->BytesPerPixel; // get pixel at x,y+2
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p -= 2 * screen->pitch;
                case 2:
                    count += 4;
                    // get pixel at x,y
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p -= screen->format->BytesPerPixel; // get pixel at x-1,y
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p -= screen->pitch; // get pixel at x-1,y-1
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p += screen->format->BytesPerPixel; // get pixel at x,y-1
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p += screen->pitch;
                case 1:
                    count += 4;
                    // get pixel at x,y
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p += screen->format->BytesPerPixel; // get pixel at x+1,y
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p += screen->pitch; // get pixel at x+1,y+1
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p -= screen->format->BytesPerPixel; // get pixel at x,y+1
                    ir += p[ ridx ];
                    ig += p[ gidx ];
                    ib += p[ bidx ];
                    p -= screen->pitch;
                    ir /= count;
                    ig /= count;
                    ib /= count;
                    p[ ridx ] = ir;
                    p[ gidx ] = ig;
                    p[ bidx ] = ib;
                    break;
                default:
                    break;
            }
            p += screen->pitch;
        }
    }
}

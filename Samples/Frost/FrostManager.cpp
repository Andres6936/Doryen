#include "FrostManager.hpp"
#include "Frost.hpp"

FrostManager::FrostManager( int width, int height )
{
    w = width;
    h = height;

    grid = new float[width * height];

    img = new Doryen::TCODImage(width, height);
}

FrostManager::~FrostManager( )
{
    delete[] grid;
    delete img;
}

void FrostManager::addFrost( int x, int y )
{
    frost.emplace_back( x, y, this );
    setValue( x, y, 1.0f );
}

void FrostManager::clear( )
{
    img->clear( Doryen::Color::black );
    memset( grid, 0, sizeof( float ) * w * h );
}

void FrostManager::update( float elapsed )
{
    for ( unsigned long i = 0; i < frost.size( ); i++ )
    {
        if ( !frost[ i ].update( elapsed ))
        {
            frost.erase( frost.begin( ) + i );
        }
    }

}

void FrostManager::render( )
{
    for ( Frost &temp: frost )
    {
        temp.render( img );
    }

    img->blit2x( Doryen::Console::root, 0, 0 );
}

Doryen::Color *FrostManager::getFrostColor( )
{
    return frostColor;
}

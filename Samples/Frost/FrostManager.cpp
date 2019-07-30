#include "FrostManager.hpp"
#include "Frost.hpp"

FrostManager::FrostManager( int w, int h ) : w( w ), h( h )
{
    grid = new float[w * h];
    img = new TCODImage( w, h );
}

FrostManager::~FrostManager( )
{
    delete[] grid;
    delete img;
}

void FrostManager::addFrost( int x, int y )
{
    list.push( new Frost( x, y, this ));
    setValue( x, y, 1.0f );
}

void FrostManager::clear( )
{
    img->clear( Doryen::Color::black );
    memset( grid, 0, sizeof( float ) * w * h );
}

void FrostManager::update( float elapsed )
{
    TCODList <Frost *> toRemove;
    for ( Frost **it = list.begin( ); it != list.end( ); it++ )
    {
        if ( !( *it )->update( elapsed ))
        { toRemove.push( *it ); }
    }
    for ( Frost **it = toRemove.begin( ); it != toRemove.end( ); it++ )
    {
        list.removeFast( *it );
    }
    toRemove.clearAndDelete( );
}

void FrostManager::render( )
{
    for ( Frost **it = list.begin( ); it != list.end( ); it++ )
    {
        ( *it )->render( img );
    }
    img->blit2x( Doryen::Console::root, 0, 0 );
}
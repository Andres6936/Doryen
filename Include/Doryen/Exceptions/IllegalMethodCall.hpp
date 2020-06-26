#ifndef LIBTCOD_ILLEGALMETHODCALL_HPP
#define LIBTCOD_ILLEGALMETHODCALL_HPP

#include "Exception.hpp"

namespace Doryen
{
    namespace Exceptions
    {

        class IllegalMethodCall : public Exception
        {

        public:

            explicit IllegalMethodCall( const std::string &nMessage ) : Exception( nMessage )
            {

            }

            std::string showError( ) const override
            {
                return message;
            }

        };
    }
}

#endif //LIBTCOD_ILLEGALMETHODCALL_HPP

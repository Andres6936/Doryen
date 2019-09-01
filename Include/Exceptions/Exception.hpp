#ifndef LIBTCOD_EXCEPTION_HPP
#define LIBTCOD_EXCEPTION_HPP

#include <string>

namespace Doryen
{
    namespace Exceptions
    {
        class Exception
        {

        protected:

            std::string message;

        public:

            explicit Exception( const std::string &nMessage )
            {
                message = nMessage;
            }

            virtual ~Exception( ) = default;

            virtual std::string showError( ) const
            {
                return message;
            }
        };
    }
}

#endif //LIBTCOD_EXCEPTION_HPP

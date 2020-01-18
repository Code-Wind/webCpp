#ifndef _GSERVER_HPP_
#define _GSERVER_HPP_

extern "C"
{
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/types.h>
    #include <errno.h>
    #ifdef __BSD__
        #include <sys/event.h>
    #endif
}

#include "G/net/Socket.hpp"
#include "G/io/IOHandler.hpp"

namespace G
{
    class Server : public virtual Socket
    {
    protected:
        virtual int _service(G::IOHandler *, int) =0;
    };
}
#endif

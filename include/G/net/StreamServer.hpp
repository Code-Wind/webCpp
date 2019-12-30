#ifndef _STREAMSERVER_HPP_
#define _STREAMSERVER_HPP_

extern "C"
{
    #include "G/net/Gnet.h"
}

#include "G/net/Server.hpp"
#include "G/event/EventListener.hpp"

namespace G {

    class StreamServer: virtual public G::Server
    {
        G::EventListener * listener;
    // parent protected:
    //     virtual SOCKET initSocket() =0;
    public:
    // parent public:
    //     virtual int service(G::IOEvents *, int) =0;
        void init(G::EventListener *);
        StreamServer();
        virtual ~StreamServer() =0;
        virtual int service(int) override;
    };

}

#endif

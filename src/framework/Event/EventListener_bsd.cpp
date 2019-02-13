//
//  Event.cpp
//  GHTTPd
//
//  Created by 王兴卓 on 16/6/11.
//  Copyright © 2016年 watsserve. All rights reserved.
//

// struct kevent {
//     uintptr_t       ident;          // 文件描述符
//     int16_t         filter;         /* filter for event */
//     uint16_t        flags;          // general flags
//                                        EV_ADD, EV_ENABLE, EV_DISABLE, EV_DELETE,
//                                        EV_RECEIPT, EV_ONESHOT, EV_CLEAR, EV_EOF, EV_OOBAND, EV_ERRO,
//     uint32_t        fflags;         /* filter-specific flags */
//     intptr_t        data;           /* filter-specific data */
//     void            *udata;         /* opaque user data identifier */
// };

#include "G/EventListener.hpp"
#ifdef __BSD__

using namespace G;

int EventListener::init(EventListener &self, ThreadPool * tpool, int max)
{
    if (nullptr == tpool) {
        perror("Can't no thread pool");
        return -1;
    }
    self.tpool = tpool;

    // 准备注册内核事件
    self.epfd = kqueue();
    if (-1 == self.epfd) {
        perror("Can't create kqueue");
        return -1;
    }

    return 0;
}

void* G::EventListener::listener(void *that)
{
    G::EventListener *self;
    struct kevent *eventList;
    int i, nEvent, max;
    exeable_t *udata;
    ThreadPool *tpool;

    self = (G::EventListener *)that;
    tpool = self->tpool;
    max = self->max;

    // 可用事件列表
    eventList = (struct kevent *)malloc(sizeof(struct kevent) * max);
    if (nullptr == eventList) {
        perror("Can't create event list");
        exit(1);
    }

    while (1)
    {
        // 获取可用事件
        nEvent = kevent(self->epfd, nullptr, 0, eventList, max, nullptr);
        if (-1 == nEvent) {
            perror("wait event faild");
            exit(1);
        }
        for(i = 0; i < nEvent; i++)
        {
            if (eventList[i].flags & EV_ERROR)  // 出错
            {
                close((int)(eventList[i].ident));
                continue;
            }

            udata = (G::exeable_t *)eventList[i].udata;
            if (-1 == tpool->call(*udata)) {
                perror("request thread pool");
                exit(1);
            }
        }
    }

    return;
}

#endif
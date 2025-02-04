#include "Poller.h"
#include "EpollPoller.h"
#include <stdlib.h>

Poller *Poller::newDefaultPoller(EventLoop *loop)
{
    if (::getenv("MUDUO_USE_POLL"))
    {
        return nullptr;
        // return new PollPoller(loop); // 生成poll的实例
    }
    else
    {
        return new EPollPoller(loop); // 生成epoll的实例
    }
}

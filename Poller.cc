#include "Poller.h"
#include "Channel.h"

Poller::Poller(EventLoop *loop)
    : ownerloop_(loop)
{
}

bool Poller::hasChannel(Channel *channel) const
{
    return channels_.find(channel->fd()) != channels_.end() && channels_.find(channel->fd())->second == channel;
}

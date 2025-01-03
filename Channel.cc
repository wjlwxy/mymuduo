#include "Channel.h"
#include "Logger.h"
#include <sys/epoll.h>

const int KNoneEvent = 0;
const int kReadEvent = EPOLLIN | EPOLLPRI;
const int kWriteEvent = EPOLLOUT;

// EventLoop: ChannelList Poller
Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(0),
      revents_(0),
      index_(-1),
      tied_(false)
{
}

Channel::~Channel()
{
}

void Channel::tie(const std::shared_ptr<void> &obj)
{
    tie_ = obj;
    tied_ = true;
}

// 当改变channel所表fd的events事件后，需要调用update()来更新Poller中的事件 即 更新epoll_ctl()
void Channel::update()
{
    // 通过 channel 所属的EventLoop，调用poller的相应方法，注册fd的events事件
    /*add code ...*/
    // loop_->updateChannel(this);
}

// 在channel所属的EventLoop中，调用poller的removeChannel()方法，从Poller中移除当前channel
void Channel::remove()
{
    // add code ...
    // loop_->removeChannel(this);
}

// fd 得到poller通知以后，处理事件的
void Channel::handleEvent(Timestamp receiveTime)
{
    std::shared_ptr<void> guard;
    if (tied_)
    {
        guard = tie_.lock();
        if (guard)
        {
            handleEventWithGuard(receiveTime);
        }
    }
    else
    {
        handleEventWithGuard(receiveTime);
    }
}

// 根据poller通知的channel发生的具体事件， 由channel负责调用具体的回调操作
void Channel::handleEventWithGuard(Timestamp receiveTime)
{
    LOG_INFO("channel handle events:%d\n", revents_);
    if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN))
    {
        if (closeCallback_)
        {
            closeCallback_();
        }
    }
    if (revents_ & EPOLLIN)
    {
        if (readCallback_)
        {
            readCallback_(receiveTime);
        }
    }
    if (revents_ & EPOLLOUT)
    {
        if (writeCallback_)
        {
            writeCallback_();
        }
    }
    if (revents_ & EPOLLERR)
    {
        if (errorCallback_)
            errorCallback_();
    }
}

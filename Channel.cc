#include "Channel.h"
#include "Logger.h"
#include <sys/epoll.h>
#include "EventLoop.h"

const int Channel::KNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

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

// channel的tie方法什么时候调用过？一个TcpConnection新连接创建的时候  tie_为TcpConnection对象的弱智能指针
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
    loop_->updateChannel(this);
}

// 在channel所属的EventLoop中，调用poller的removeChannel()方法，从Poller中移除当前channel
void Channel::remove()
{
    // add code ...
    loop_->removeChannel(this);
}

// fd 得到poller通知以后，处理事件的
void Channel::handleEvent(Timestamp receiveTime)
{

    if (tied_)
    {
        std::shared_ptr<void> guard = tie_.lock();
        if (guard) // 一旦提升成功，必须等此智能指针被析构，也就是出作用域，才能析构TcpConnection对象资源
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

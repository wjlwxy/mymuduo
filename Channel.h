#pragma once

#include "noncopyable.h"
#include <functional>
#include "Timestamp.h"
#include <memory>

class EventLoop;

/*
 Channel 理解为通道，封装了sockfd和其感兴趣的event，如EPOLLIN、EPOLLOUT事件
*/
class Channel : noncopyable
{
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    // fd得到poller通知以后，处理事件的
    void handleEvent(Timestamp receiveTime);

    // 注册事件回调
    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    // 防止当channel被手动remove掉，channel还在执行回调操作，导致程序崩溃
    void tie(const std::shared_ptr<void> &);

    int fd() const { return fd_; }
    int events() const { return events_; }
    int set_revents(int revents)
    {
        events_ = revents;
        return events_; // 不确定是否是返回此值，根据调用情况再看
    }

    // 返回fd当前的事件状态
    bool isNoneEvent() const { return events_ == KNoneEvent; }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }

    // 注册fd想要的事件状态
    void enableReading()
    {
        events_ |= kReadEvent;
        update();
    }
    void disableReading()
    {
        events_ &= ~kReadEvent;
        update();
    }
    void enableWriting()
    {
        events_ |= kWriteEvent;
        update();
    }
    void disableWriting()
    {
        events_ &= ~kWriteEvent;
        update();
    }
    void disableAll()
    {
        events_ = KNoneEvent;
        update();
    }

    int index() { return index_; }
    void set_index(int index) { index_ = index; }

    // one loop per thread
    EventLoop *ownerLoop() { return loop_; }
    void remove(); // 移除channel

private:
    void update();
    void handleEventWithGuard(Timestamp receiveTime);

    static const int KNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop *loop_; // 事件循环
    const int fd_;    // fd, Poller监听的对象
    int events_;      // 注册fd感兴趣的事件
    int index_;
    int revents_; // 实际发生的事件

    std::weak_ptr<void> tie_;
    bool tied_;

    // 因为channel通道里面能够获知fd最终发生的具体的事件revents，所以它负责调用具体事件的回调操作
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};
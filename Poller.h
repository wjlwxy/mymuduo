#pragma once

#include <vector>
#include <unordered_map>
#include "Timestamp.h"
#include "noncopyable.h"

class Channel;
class EventLoop;

// muduo库中多路事件分发器的核心IO复用模块
class Poller : noncopyable
{
public:
    using ChannelList = std::vector<Channel *>;

    Poller(EventLoop *loop);
    virtual ~Poller() = default;

    // 给所有IO复用保留统一的接口
    virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
    virtual void updateChannel(Channel *channel) = 0;
    virtual void removeChannel(Channel *channel) = 0;

    // 判断参数channel是否已经注册到Poller中
    bool hasChannel(Channel *channel) const;

    // EventLoop可以通过该接口获取默认的IO复用的具体实现
    static Poller *newDefaultPoller(EventLoop *loop);

protected:
    // map的key：sockfd  value: sockfd所属的channel
    using ChannelMap = std::unordered_map<int, Channel *>;
    ChannelMap channels_; // 保存所有注册到Poller中的Channel

private:
    EventLoop *ownerloop_; // 定义Poller所属的事件循环EventLoop
};
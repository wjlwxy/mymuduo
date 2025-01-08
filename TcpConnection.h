#pragma once

#include "noncopyable.h"
#include "InetAddress.h"
#include "Callbacks.h"
#include "Timestamp.h"
#include "Buffer.h"

#include <memory>
#include <string>
#include <atomic>

class Channel;
class EventLoop;
class Socket;

/*
    TcpServer => Acceptor => 有一个新用户连接，通过accept函数拿到connfd

    =>  TcpConnection 设置回调 => Channel_ => Poller => Channel_的回调操作
*/
// enable_shared_from_this是一个模板类，所以这里就是继承了一个类
class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *loop,
                  const std::string &name,
                  int sockfd,
                  const InetAddress &localAddr,
                  const InetAddress &peerAddr);

    ~TcpConnection();

    EventLoop *getloop() const { return loop_; }
    /*
    函数前const：普通函数或成员函数（非静态成员函数）前均可加const修饰，表示函数的返回值为const，不可修改。

    函数后加const：只有类的非静态成员函数后可以加const修饰，表示该类的this指针为const类型，
    不能改变类的成员变量的值，即成员变量为read only（例外情况见2），
    任何改变成员变量的行为均为非法。此类型的函数可称为只读成员函数
    */
    const std::string &name() const { return name_; }
    const InetAddress &localAddress() const { return localAddr_; }
    const InetAddress &peerAddress() const { return peerAddr_; }

    bool connected() const { return state_ == kConnected; }
    bool disconnected() const { return state_ == kDisconnected; }

    // 发送数据
    // void send(const void *message, int len);
    void send(const std::string &buf);
    // 关闭连接
    void shutdown();

    void setConnectionCallback(const ConnectionCallback &cb)
    {
        connectionCallback_ = cb;
    }
    void setMessageCallback(const MessageCallback &cb)
    {
        messageCallback_ = cb;
    }
    void setWriteCompleteCallback(const WriteCompleteCallback &cb)
    {
        writeCompleteCallback_ = cb;
    }
    void setHighWaterMarkCallback(const HighWaterMarkCallback &cb)
    {
        highWaterMarkCallback_ = cb;
    }
    void setCloseCallback(const CloseCallback &cb)
    {
        closeCallback_ = cb;
    }
    // 连接建立
    void connectEstablished();
    // 连接销毁
    void connectDestroyed();

private:
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

    void sendInLoop(const void *data, size_t len);
    void shutdownInLoop();

    enum StateE
    {
        kDisconnected,
        kConnecting,
        kConnected,
        kDisconnecting
    };

    void setState(StateE state) { state_ = state; }

    EventLoop *loop_; // 这里绝对不是baseLoop， 因为tcpConnection都是在subLoop里面管理的
    const std::string name_;

    std::atomic_int state_;
    bool reading_;

    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;

    const InetAddress localAddr_;
    const InetAddress peerAddr_;

    ConnectionCallback connectionCallback_; // 会是&类名::方法格式的传进来吗？
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    CloseCallback closeCallback_;

    size_t highWaterMark_;

    Buffer inputBuffer_;  // 接收数据的缓冲区
    Buffer outputBuffer_; // 发送数据的缓冲区
};
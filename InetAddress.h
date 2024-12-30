#pragma once

#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>

// 封装socket地址类型
class InetAddress
{
public:
    explicit InetAddress(uint16_t port, std::string ip = "127.0.0.1");
    explicit InetAddress(const struct sockaddr_in &addr) : addr_(addr) {}

    std::string toIp() const;
    uint16_t toPort() const;
    std::string ToIpPort() const;
    const struct sockaddr_in *getSockAddr() const { return &addr_; }

private:
    struct sockaddr_in addr_;
};
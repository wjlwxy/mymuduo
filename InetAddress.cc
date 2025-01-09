#include "InetAddress.h"

#include <strings.h>

InetAddress::InetAddress(uint16_t port, std::string ip) // 默认值在声明和定义中只能出现一次
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip.c_str()); // 默认值
    // if (::inet_aton(ip.c_str(), &addr_.sin_addr) == 0) {
}

std::string InetAddress::toIp() const
{
    char buf[64] = {0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf); // 将网络字节序的IP地址转换为字符串
    return buf;
}

uint16_t InetAddress::toPort() const
{
    return ntohs(addr_.sin_port);
}
std::string InetAddress::ToIpPort() const
{
    return toIp() + ":" + std::to_string(toPort());
}

// #include <iostream>
// int main()
// {
//     InetAddress addr(8080, "127.0.0.1");
//     std::cout << addr.toIp() << std::endl;
//     std::cout << addr.toPort() << std::endl;
//     std::cout << addr.ToIpPort() << std::endl;
//     return 0;
// }
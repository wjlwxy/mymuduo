#pragma once

#include <vector>
#include <string>
#include <algorithm>

// 网络库底层的缓冲器类型定义
class Buffer
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
        : buffer_(kCheapPrepend + initialSize),
          readerIndex_(kCheapPrepend),
          writerIndex_(kCheapPrepend)
    {
    }

    size_t readableBytes() const
    {
        return writerIndex_ - readerIndex_;
    }

    size_t writableBytes() const
    {
        return buffer_.size() - writerIndex_;
    }

    size_t prependableBytes() const
    {
        return readerIndex_;
    }

    const char *peek() const
    {
        return begin() + readerIndex_;
    }

    // onMessage string <- Buffer
    void retrieve(size_t len)
    {
        if (len < readableBytes())
        {
            readerIndex_ += len; // 应用只读取了可读缓冲区数据的一部分，writerIndex_ - (readerIndex + len)部分的区域还未读取
        }
        else // len == readableBytes()
        {
            retrieveAll();
        }
    }

    void retrieveAll()
    {
        readerIndex_ = kCheapPrepend;
        writerIndex_ = kCheapPrepend;
    }

    // 把onMessage函数上报的Buffer数据，转成string类型的数据返回
    std::string retrieveAllAsString()
    {
        return retrieveAsString(readableBytes()); // 应用可读取数据的长度
    }

    std::string retrieveAsString(size_t len)
    {
        std::string result(peek(), len);
        retrieve(len); // 上面一句把可读缓冲区的数据读取出来，这里将复位可读缓冲区
        return result;
    }

    // buffer_.size - writerIndex_是否足够
    void ensureWriteableBytes(size_t len)
    {
        if (writableBytes() < len)
        {
            makespace(len); // 扩容
        }
    }

    // 把[data, data+len]内存上的数据，添加到writable缓冲区中
    void append(const char *data, size_t len)
    {
        ensureWriteableBytes(len);
        std::copy(data, data + len, beginWrite());
        writerIndex_ += len;
    }

    char *beginWrite()
    {
        return begin() + writerIndex_;
    }

    const char *beginWriter() const
    {
        return begin() + writerIndex_;
    }

    // 从fd上读取数据到Buffer中 接收数据
    ssize_t readFd(int fd, int *saveErrno);
    // 将Buffer中数据写入到fd发送出去
    ssize_t writeFd(int fd, int *saveErrno);

private:
    void makespace(size_t len)
    {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            buffer_.resize(writerIndex_ + len);
        }
        else
        {
            size_t readable = readableBytes();
            std::copy(begin() + readerIndex_,
                      begin() + writerIndex_,
                      begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = kCheapPrepend + readable;
        }
    }
    char *begin()
    {
        // it.operator*()
        return &*buffer_.begin();
    }
    const char *begin() const
    {
        return &*buffer_.begin();
    }

    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
};
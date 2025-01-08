#include "Thread.h"
#include "CurrentThread.h"
#include <semaphore.h>

std::atomic_int Thread::numCreated_(0);

Thread::Thread(ThreadFunc func, const std::string &name)
    : started_(false),
      joined_(false),
      tid_(0),
      func_(std::move(func)),
      name_(name)
{
    setDefaultName();
}
Thread::~Thread()
{
    if (started_ && !joined_) // joined和detach不能共存，joined后主线程会等待子线程执行完毕再结束，detach则是主线程结束，子线程也将结束
    {
        thread_->detach(); // thread类提供的设置分离线程的方法
    }
}

void Thread::start() // 一个Thread对象，记录的就是一个新线程的详细信息
{
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);
    // 开启线程
    thread_ = std::shared_ptr<std::thread>(new std::thread([&]()
                                                           {
        // 获取线程的tid值
        tid_ = CurrentThread::tid();
        sem_post(&sem);
        // 开启一个新线程，专门执行该线程函数
        func_(); }));
    // 这里必须等待获取上述新线程的tid值
    sem_wait(&sem);
    sem_destroy(&sem);
}
void Thread::join()
{
    joined_ = true;
    thread_->join();
}

void Thread::setDefaultName()
{
    int cnt = ++numCreated_;
    if (name_.empty())
    {
        char buf[32] = {0};
        snprintf(buf, sizeof buf, "thread%d", cnt);
        name_ = buf;
    }
}
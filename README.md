# mymuduo
简单介绍：
参考muduo网络库，使用C++11标准编写的拥有非阻塞IO多路复用的TCP网络编程库，采用主从Reactor模式+线程池，实现了对网络编程接口的封装。
去除了muduo库对boost库的依赖，提升了可移植性：采用c++11标准的绑定器、智能指针、线程函数以及原子类型
底层使用Epoll + non_block作为IO多路复用，并在事件触发后调用各事件的回调函数

概述：
在这个多核时代，服务端网络编程如何选择线程模型呢？ 赞同libev作者的观点：one loop per thread is usually a good model，
这样多线程服务端编程的问题就转换为如何设计一个高效且易于使用的event loop，
然后每个线程run一个event loop就行了（当然线程间的同步、互斥少不了，还有其它的耗时事件需要起另外的线程来做）。

event loop 是 non-blocking 网络编程的核心，在现实生活中，non-blocking 几乎总是和 IO-multiplexing
一起使用，原因有两点：
        1. 没有人真的会用轮询 (busy-pooling) 来检查某个 non-blocking IO 操作是否完成，这样太浪费CPU资源了。
        2. IO-multiplex 一般不能和 blocking IO 用在一起，因为 blocking IO 中read()/write()/accept()/connect() 都有可能阻塞当前线程，
        这样线程就没办法处理其他 socket上的 IO 事件了。
        
使用方法：
下载到文件夹后，sudo ./autobuild.sh，运行编译和安装脚本，相关头文件也会添加到系统路径。
测试代码示例在example文件夹中，同样也编写好了一键编译的makefile，在example目录下的终端make即可。

/*
*@brief 一些公共参数和工具
*@author yanning
*@date 2022-04-20
*/
#ifndef UTILS_H
#define UTILS_H
#include<stddef.h>
namespace serverco{


    // 去掉复制构造函数和赋值操作符
    #define DISALLOW_COPY_MOVE_AND_ASSIGN(TypeName) TypeName(const TypeName&) = delete; TypeName(const TypeName&&) = delete;  TypeName& operator=(const TypeName&) = delete

    // 协程栈的大小
    const static size_t coroutineStackSize = 32 * 1024;

    // 获取活跃的epoll_event的数组的初始长度
    static constexpr int epollerListFirstSize = 16;

    // epoll_wait的阻塞时长
    static constexpr int epollTimeOutMs = 10000;

    // 监听队列的长度
    constexpr static unsigned backlog = 4096;

    // 内存池没有空闲块时申请的内存块个数
    static constexpr size_t memPoolMallocObjCnt = 40;

    // 非闰年每月天数
    const char days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // 定时器Buffer大小
    constexpr int timerDummybufSize = 1024;

    // 协程状态
    enum coStatus
    {
        CO_READY = 0,
        CO_RUNNING,
        CO_WAITING,
        CO_DEAD
    };

    // 内存池分配链表
    struct MemBlockNode
    {
        union
        {
            MemBlockNode* next;
            char data;
        };
    };

    // 事件管理器选择器策略
    enum scheduleStrategy
    {
        MIN_EVENT_FIRST = 0, // 最少事件优先
        ROUND_ROBIN          // 轮流分发
    };

    // 事件管理器状态
    enum processerStatus
    {
        PRO_RUNNING = 0,
        PRO_STOPPING,
        PRO_STOPPED
    };

    // 添加新协程的状态
    // enum newCoAddingStatue
    // {
    //     NEWCO_ADDING = 0,
    //     NEWCO_ADDED
    // };
}
#endif
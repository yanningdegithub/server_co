/*
*@brief 协程类
*@author yanning
*@date 2022-04-20
*/
/*
* 每一个线程对应一个processor实例，协程运行成processor的主循环上，使用epoll和timer进行任务调度
* Scheduler是一个全局单例，当某个线程中调用co_go()运行一个新协程后实际就会调用
*/
#ifndef COROUTINE_H
#define COROUTINE_H

#include <functional>
#include "context.h"
#include "utils.h"

namespace serverco
{
    class Processor;
    class Coroutine
    {
    public:
        Coroutine(Processor*, size_t stackSize, std::function<void()>&&);
		Coroutine(Processor*, size_t stackSize, std::function<void()>&);
		~Coroutine();

        DISALLOW_COPY_MOVE_AND_ASSIGN(Coroutine);
        
        // 恢复运行当前协程
        void resume();

        // 暂停运行当前协程
        void yield();

        Processor* getMyProcessor() { return pMyProcessor_; }

        // 运行该协程的函数
        inline void startFunc() { coFunc_(); }
        
        // 获取该协程的上下文
        inline Context* getCtx() { return &ctx_; }

    private: 
        std::function<void()> coFunc_;
        Processor* pMyProcessor_;
        int status_;
        Context ctx_;
    };
}

#endif
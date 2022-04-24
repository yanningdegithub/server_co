/*
 *@brief 协程管理器,一个CPU核心对应一个线程，
 *@details 负责存放协程实例并管理其生命周期
 *@param [in] newCoroutines_ 新协程双缓冲队列，使用一个队列来存放新来的协程，另一个队列给Processor主循环用于执行新来的协程，消费完成以后就交换队列（每加入一次新协程就唤醒一次主循环，以立即执行新来的协程）
 *@param [in] actCoroutines_ 被epoll激活的协程队列.当epoll_wait被激活时，Processor主循环会尝试从Epoll中获取活跃的协程，存放在该actCoroutines队列中，然后依次恢复执行
 *@param [in] timerExpiredCo_ 超时的协程队列.当epoll_wait被激活时,Processor主循环会首先尝试从Timer中获取活跃的协程，存放在timerExpiredCo队列中，然后依次恢复执行
 *@param [in] removedCo_ 被移除的协程队列.执行完的协程会首先放到该队列中,在一次循环的最后被统一清理
 *@author yanning
 *@date 2022-04-20
 */
#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <queue>
#include <set>
#include <mutex>
#include <thread>
#include "objpool.h"
#include "spinlock.h"
#include "context.h"
#include "coroutine.h"
#include "epoller.h"
#include "timer.h"
extern __thread int threadIdx;

namespace serverco
{
    class Processor
    {
    public:  
        Processor(int);
        ~Processor();

        DISALLOW_COPY_MOVE_AND_ASSIGN(Processor);

        // 运行一个新的协程， 注册回调函数
        void goNewCo(std::function<void()>&& func, size_t stackSize);
        void goNewCo(std::function<void()>& func, size_t stackSize);

        void yield();

        // 当前协程等待time毫秒
        void wait(Time time);

        // 清除当前正在运行的协程
        void killCurCo();

        bool loop();

        void stop();

        void join();

        // 等待fd上的ev事件返回
        void waitEvent(int fd, int ev);

        // 获取当前正在运行的协程
        inline Coroutine* getCurRunningCo() { return pCurCoroutine_; }

        inline Context* getMainCtx() { return &mainCtx_; }

        inline size_t getCoCnt() { return coSet_.size(); }

        void goCo(Coroutine* co);

        void goCoBatch(std::vector<Coroutine*>& cos);

    private:  
        // 恢复运行指定协程
        void resume(Coroutine*);

        // 唤醒epoll
        inline void wakeUpEpoller();
        
        // 处理器的线程号
        int tid_;
        
        int status_;

        std::thread* pLoop_;

        // 新任务队列，使用双缓存队列
        std::queue<Coroutine*> newCoroutines_[2];

        // 信任我双缓冲队列中正在运行的队列号，另一条用于添加任务
        volatile int runningNewQue_;

        Spinlock newQueLock_;

        Spinlock coPoolLock_;

        //EventEpoller发现的活跃事件所放的列表
        std::vector<Coroutine*> actCoroutines_;

        std::set<Coroutine*> coSet_;

        std::vector<Coroutine*> timerExpiredCo_;

        std::vector<Coroutine*> removedCo_;

        Epoller epoller_;

        Timer timer_;

        ObjPool<Coroutine> coPool_;

        Coroutine* pCurCoroutine_;

        Context mainCtx_;
    };
}


#endif
/*
 *@brief 线程调度器，让用户指定协程运行在某个processor上，没有指定则挑选协程数量最少的processor接管新的协程
 *@author yanning
 *@date 2022-04-20
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <vector>
#include <functional>
#include "processor.h"
#include "processor_selector.h"
namespace serverco
{
    class Scheduler 
    {
    protected:  
        Scheduler();
        ~Scheduler();

    public:  
        DISALLOW_COPY_MOVE_AND_ASSIGN(Scheduler);

        static Scheduler* getScheduler();

        // 在idx号线程创建新协程
        void createNewCo(std::function<void()>&& func, size_t stackSize);
        void createNewCo(std::function<void()>& func, size_t stackSize);

        Processor* getProcessor(int);

        int getProCnt();

        void join();
    private:  
        // 初始化Scheduler, threadCnt是开启几个线程
        bool startScheduler(int threadCnt);

        static Scheduler* pScher_;

        // 用于保护的锁，为了服务器执行效率不能长时间占用
        static std::mutex scherMtx_;

        std::vector<Processor*> processors_;

        ProcessorSelector proSelector_;
    };
}
#endif
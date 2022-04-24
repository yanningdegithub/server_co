#include "../include/server_api.h"

void serverco::co_go(std::function<void()>&& func, size_t stackSize, int tid)
{
    if(tid < 0)
    {
        serverco::Scheduler::getScheduler()->createNewCo(std::move(func), stackSize);
    }else{
        tid %= serverco::Scheduler::getScheduler()->getProCnt();
        serverco::Scheduler::getScheduler()->getProcessor(tid)->goNewCo(std::move(func), stackSize);
    }
}

void serverco::co_go(std::function<void()>& func, size_t stackSize, int tid)
{
    if(tid < 0)
    {
        serverco::Scheduler::getScheduler()->createNewCo(func, stackSize);
    }else{
        tid %= serverco::Scheduler::getScheduler()->getProCnt();
        serverco::Scheduler::getScheduler()->getProcessor(tid)->goNewCo(func, stackSize);
    }
}

void serverco::co_sleep(Time time)
{
    serverco::Scheduler::getScheduler()->getProcessor(threadIdx)->wait(time);
}

void serverco::sche_join()
{
    serverco::Scheduler::getScheduler()->join();
}
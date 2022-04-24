/*
 *@brief 协程调用接口
 *@author yanning
 *@date 2022-04-20
 */
#ifndef SERVERCO_API_H
#define SERVERCO_API_H
#include "scheduler.h"
#include "mstime.h"
namespace serverco
{
    void co_go(std::function<void()>& func, size_t stackSize = coroutineStackSize, int tid = -1);
    void co_go(std::function<void()>&& func, size_t stackSize = coroutineStackSize, int tid = -1);

    // 当前协程等待t毫秒之后再继续执行
    void co_sleep(Time t);

    // 等待调度器结束
    void sche_join();
}
#endif
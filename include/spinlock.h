/*
 *@brief 二元信号量实现锁
 *@author yanning
 *@date 2022-04-20
 */
#ifndef SPINLOCK_H
#define SPINLOCK_H
#include <atomic>
#include "utils.h"

namespace serverco
{
    class Spinlock
    {
    public:
        Spinlock() : sem_(1) {}
        ~Spinlock() { unlock(); }

        DISALLOW_COPY_MOVE_AND_ASSIGN(Spinlock);

        void lock()
        {
            int exp = 1;
            // 如果sem_不等于exp才进入循环，等于exp时就结束,也就是sem为1时是可用资源
            while(!sem_.compare_exchange_strong(exp, 0))
            {
                exp = 1;
            }
        }

        void unlock()
        {
            // 将sem变量设置为1
            sem_.store(1);
        }
    private:  
        std::atomic_int sem_;
    };
}

#endif
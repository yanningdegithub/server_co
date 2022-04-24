/*
 *@brief 封装互斥锁
 *@author yanning
 *@date 2022-04-20
 */
#ifndef SPINLOCK_GUARD_H
#define SPINLOCK_GUARD_H
#include "spinlock.h"
#include "utils.h"
namespace serverco
{
    class spinlockGuard
    {
    public:  
        spinlockGuard(Spinlock& l) : lock_(l)
        {
            lock_.lock();
        }

        ~spinlockGuard()
        {
            lock_.unlock();
        }
        DISALLOW_COPY_MOVE_AND_ASSIGN(spinlockGuard);
    private:  
        Spinlock& lock_;
    };
}

#endif
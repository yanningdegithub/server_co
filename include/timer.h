/*
 *@brief 定时器
 *@author yanning
 *@date 2022-04-20
 */
#ifndef TIMER_H
#define TIMER_H
#include <map>
#include <queue>
#include <vector>
#include <mutex>
#include <functional>
#include "mstime.h"
#include "utils.h"

namespace serverco
{
    class Coroutine;
    class Epoller;
    class Timer
    {
    public:
        // 小根堆存放时间和协程对象
        using TimerHeap = typename std::priority_queue<std::pair<Time, Coroutine*>, std::vector<std::pair<Time, Coroutine*>>, std::greater<std::pair<Time, Coroutine*>>>;
        Timer();
        ~Timer();

        bool init(Epoller*);
        DISALLOW_COPY_MOVE_AND_ASSIGN(Timer);

        //获取所有已经超时的需要执行的函数
        void getExpiredCoroutines(std::vector<Coroutine*>& expiredCoroutinues);

        // 在time时刻需要恢复协程Co
        void runAt(Time time, Coroutine* pCo);

        // 经过time毫秒恢复协程Co
        void runAfter(Time time, Coroutine* pCo);

        void wakeUp();

    private:  
        // 给timefd重新设置时间，绝对时间
        bool resetTimeOfTimefd(Time time);
        
        inline bool isTimeFdUseful() { return timeFd_ < 0 ? false : true; }

        int timeFd_;

        // 读取timefd上数据
        char dummyBuf_[timerDummybufSize];

        TimerHeap timerCoHeap_;

    };
}
#endif
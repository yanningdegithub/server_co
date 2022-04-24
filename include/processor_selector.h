/*
 *@brief 事件管理器选择器，决定下一个协程应该放入哪个协程管理器中
 *@author yanning
 *@date 2022-04-20
 */
#ifndef PROCESSOR_SELECTOR_H
#define PROCESSOR_SELECTOR_H
#include <vector>
#include "utils.h"
namespace serverco
{
    class Processor;
    class ProcessorSelector
    {
    public:  
        ProcessorSelector(std::vector<Processor*>& processors, int strategy = MIN_EVENT_FIRST) : curPro_(-1), strategy_(strategy), processors_(processors) {}
        ~ProcessorSelector() {}

        //设置分发任务的策略
		//MIN_EVENT_FIRST则每次挑选EventService最少的EventManager接收新连接
		//ROUND_ROBIN则每次轮流挑选EventManager接收新连接
        inline void setStrategy(int strategy) { strategy_ = strategy; }

        Processor* next();

    private:  
        int curPro_;
        int strategy_;
        std::vector<Processor*>& processors_;
    };
}

#endif
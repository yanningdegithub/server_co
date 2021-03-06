#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <string.h>
#include <unistd.h>
#include "../include/timer.h"
#include "../include/coroutine.h"
#include "../include/epoller.h"

using namespace serverco;
Timer::Timer() : timeFd_(-1)
{}

Timer::~Timer()
{
    if(isTimeFdUseful())
    {
        ::close(timeFd_);
    }
}

bool Timer::init(Epoller* pEpoller)
{
    timeFd_ = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(isTimeFdUseful())
    {
        return pEpoller->addEv(nullptr, timeFd_, EPOLLIN | EPOLLPRI | EPOLLRDHUP);
    }
    return false;
}

void Timer::getExpiredCoroutines(std::vector<Coroutine*>& expiredCoroutines)
{
    Time nowTime = Time::now();
	while (!timerCoHeap_.empty() && timerCoHeap_.top().first <= nowTime)
	{
		expiredCoroutines.push_back(timerCoHeap_.top().second);
		timerCoHeap_.pop();
	}

	if (!expiredCoroutines.empty())
	{
		ssize_t cnt = timerDummybufSize;
		while (cnt >= timerDummybufSize)
		{
			cnt = ::read(timeFd_, dummyBuf_, timerDummybufSize);
		}
	}

	if (!timerCoHeap_.empty())
	{
		Time time = timerCoHeap_.top().first;
		resetTimeOfTimefd(time);
	}
}

void Timer::runAt(Time time, Coroutine* pCo)
{
    timerCoHeap_.push(std::move(std::pair<Time, Coroutine*>(time, pCo)));
    if(timerCoHeap_.top().first == time)
    {
        resetTimeOfTimefd(time);
    }
}

bool Timer::resetTimeOfTimefd(Time time)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    memset(&newValue, 0, sizeof(newValue));
    memset(&oldValue, 0, sizeof(oldValue));
    newValue.it_value = time.timeIntervalFromNom();
    int ret = ::timerfd_settime(timeFd_, 0, &newValue, &oldValue);
    return ret < 0 ? false : true;
}

void Timer::runAfter(Time time, Coroutine* pCo)
{
    Time runTime(Time::now().getTimeVal() + time.getTimeVal());
    runAt(runTime, pCo);
}

void Timer::wakeUp()
{
    resetTimeOfTimefd(Time::now());
}
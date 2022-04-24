#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>
#include "../include/epoller.h"
#include "../include/coroutine.h"
#include "../include/utils.h"

using namespace serverco;

Epoller::Epoller() : epollFd_(-1), activeEpollEvents_(epollerListFirstSize)
{}

Epoller::~Epoller()
{
    if(isEpollFdUseful())
    {
        ::close(epollFd_);
    }
}

bool Epoller::init()
{
    epollFd_ = ::epoll_create1(EPOLL_CLOEXEC);
    return isEpollFdUseful();
}

bool Epoller::modifyEv(Coroutine* pCo, int fd, int interesEv)
{
    if(!isEpollFdUseful())
    {
        return false;
    }
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = interesEv;
    event.data.ptr = pCo;
    if(::epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &event) < 0)
    {
        return false;
    }
    return true;
}

bool Epoller::addEv(Coroutine* pCo, int fd, int interesEv)
{
    if(!isEpollFdUseful())
    {
        return false;
    }
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = interesEv;
    event.data.ptr = pCo;
    if (::epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &event) < 0)
	{
		return false;
	}
	return true;
}

bool Epoller::removeEv(Coroutine* pCo, int fd, int interesEv)
{
	if (!isEpollFdUseful())
	{
		return false;
	}
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = interesEv;
	event.data.ptr = pCo;
	if (::epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &event) < 0)
	{
		return false;
	}
	return true;
}

int Epoller::getActEvServ(int timeOutMs, std::vector<Coroutine*>& activeEvServs)
{
    if (!isEpollFdUseful())
	{
		return -1;
	}
	int actEvNum = ::epoll_wait(epollFd_, &*activeEpollEvents_.begin(), static_cast<int>(activeEpollEvents_.size()), timeOutMs);
	int savedErrno = errno;
	if (actEvNum > 0)
	{
		if (actEvNum > static_cast<int>(activeEpollEvents_.size()))
		{
			return savedErrno;
		}
		for (int i = 0; i < actEvNum; ++i)
		{
			Coroutine* pCo = static_cast<Coroutine*>(activeEpollEvents_[i].data.ptr);
			activeEvServs.push_back(pCo);
		}
		if (actEvNum == static_cast<int>(activeEpollEvents_.size()))
		{
			activeEpollEvents_.resize(activeEpollEvents_.size() * 2);
		}
	}
	return savedErrno;
}
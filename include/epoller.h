/*
*@brief 监听epoll中的事件
*@author yanning
*@date 2022-04-20
*/

#ifndef EPOLLER_H
#define EPOLLER_H

#include <vector>
#include "utils.h"

struct epoll_event;
namespace serverco
{
    class Coroutine;

    class Epoller
    {
    public: 
        Epoller();
        ~Epoller();

        DISALLOW_COPY_MOVE_AND_ASSIGN(Epoller);

        // 初始化Epoller，新建一个epoll
        bool init();
        
        // 修改Epoller中的事件
        bool modifyEv(Coroutine* pCo, int fd, int interesEv);

        // 向Epoller中添加事件
        bool addEv(Coroutine* pCo, int fd, int interesEv);

        // 从Epoller中移除事件
        bool removeEv(Coroutine* pCo, int df, int ineresEv);

        // 获取被激活的事件服务
        int getActEvServ(int timeOutMs, std::vector<Coroutine*>& activeEvServs);
    private: 
        
        int epollFd_;
        inline bool isEpollFdUseful() { return epollFd_ < 0 ? false : true; }
        std::vector<struct epoll_event> activeEpollEvents_;
    };
}

#endif
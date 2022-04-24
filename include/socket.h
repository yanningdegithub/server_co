/*
 *@brief 封装Socket操作
 *@author yanning
 *@date 2022-04-22
 */
#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <sys/types.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include "utils.h"

struct tcp_info;
namespace serverco
{
    class Socket 
    {
    public:   
        explicit Socket(int sockfd, std::string ip = "", int port = -1) : _sockfd(sockfd), _pRef(new int(1)), _port(port), _ip(std::move(ip))
        {
            if(sockfd > 0)
            {
                setNonBlockSocket();
            }
        }

        Socket() : _sockfd(::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP)),
                _pRef(new int(1)), _port(-1), _ip("")
            {}
        Socket(Socket&& otherSock) : _sockfd(otherSock._sockfd)
        {
            *(otherSock._pRef) += 1;
            _pRef = otherSock._pRef;
            _ip = std::move(otherSock._ip);
            _port = otherSock._port;
        }

        Socket(Socket& otherSock) : _sockfd(otherSock._sockfd)
        {
            *(otherSock._pRef) += 1;
            _pRef = otherSock._pRef;
            _ip = otherSock._ip;
            _port = otherSock._port;
        }

        Socket& operator=(const Socket& otherSock) = delete;
        ~Socket();
        
        // 返回当前Socket的文件描述符
        int fd() const { return _sockfd; }

        // 返回当前Socket是否可用
        bool isUseful() { return _sockfd >= 0; }

        // 绑定IP和端口到当前Socket
        int bind(const char* ip, int port);

        // 监听当前Socket；
        int listen();

        // 接受一个连接，返回一个新连接的Socket
        Socket accept();

        // 从对应文件描述符读取数据
        ssize_t read(void* buf, size_t count);

        // 客户端连接
        void connect(const char* ip, int port);

        // 发送数据
        ssize_t send(const void* buf, size_t count);

        // 获取当前套接字的IP
        std::string ip() const { return _ip; }

        // 获取当前套接字的端口
        int port() const { return _port; }

        // 获取套接字选项
        bool getSocketOpt(struct tcp_info*) const;

        // 获取套接字选项的字符串
        bool getSocketOptString(char* buf, int len) const;

        // 获取套接字的选项的字符串
        std::string getSocketOptString() const;

        // 关闭套接字写操作
        int shutdownWrite();

        // 设置是否开机Nagle算法减少需要传输的数据包
        int setTcpNoDelay(bool on);

        //设置地址是否重用
        int setReuseAddr(bool on);

        // 设置端口是否重用
        int setReusePort(bool on);

        // 设置是否使用心跳检测
        int setKeepAlive(bool on);

        // 设置Socket为非阻塞的
        int setNonBlockSocket();

        // 设置Socket为阻塞的
        int setBlockSocket();
        
    private:  
        // 接受一个连接，返回一个新连接的Socket
        Socket accept_raw();

        // 端口或IO的文件描述符
        const int _sockfd;

        // 引用计数
        int* _pRef;

        // 端口号
        int _port;

        // ip
        std::string _ip;
    };
}

#endif
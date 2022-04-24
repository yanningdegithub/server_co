/*
 *@brief 封装服务器操作
 *@author yanning
 *@date 2022-04-22
 */
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <functional>
#include <memory>
#include <sys/sysinfo.h>
#include "socket.h"
#include "server_api.h"

namespace serverco
{
    class TcpServer
    {
    public:  
        typedef std::function<void(Socket*)> conn_callback;

        TcpServer() : _listen_fd(nullptr), _multi_listen_fd(nullptr), server_ip(nullptr), server_port(-1)
        {}

        virtual ~TcpServer()
        {
            delete _listen_fd;
            _listen_fd = nullptr;
            delete _multi_listen_fd;
            _multi_listen_fd =nullptr;
            delete server_ip;
            server_ip = nullptr;
        }

        // 单线程模式下开启服务器的运行
        void start(const char* ip, int port);

        // 多线程模式下开启服务器运行

        void start_multi(const char* ip, int port);

        // 注册回调函数的接口
        inline void register_connection(const conn_callback& func)
        {
            this->register_connection(conn_callback(func));
        }

        inline void register_connection(const conn_callback&& func)
        {
            _on_server_connection.reset(new conn_callback(std::move(func)));
        }

    private:  
        // 服务器的loop工作函数
        void server_loop();

        // 服务器多核loop函数
        void multi_server_loop(int thread_number);

        // 保存客户端连接时触发的回调函数
        std::shared_ptr<conn_callback> _on_server_connection;

        // 用于监听的socket
        Socket* _listen_fd;

        // 多线程下监听的socket队列
        Socket* _multi_listen_fd;

        // 服务器的初始参数
        const char* server_ip;
        int server_port;
    };
}

#endif
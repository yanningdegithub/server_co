#include "../include/tcpServer.h"
#include <iostream>
#include <cstring>
using namespace serverco;
std::function<void(Socket*)> default_connection(
    [](Socket* co_socket)
    {
        std::unique_ptr<Socket> connect_socket(co_socket);
        // std::vector<char> buf;
        // buf.resize(2048);
        char buf[2048];
        while(true)
        {
            memset(buf, '\0', 2048);
            auto readNum = connect_socket->read((void*)buf, 2048);
            if(readNum <= 0)
            {
                break;
            }
            
            // std::cout << buf << std::endl;
            printf("%s\n", buf);

            connect_socket->send((void*)&(buf[0]), readNum);

        }
    }
);

void TcpServer::start(const char* ip, int port)
{
    if(nullptr == _on_server_connection)
    {
        register_connection(default_connection);
    }
    _listen_fd = new Socket();
    if(_listen_fd->isUseful())
    {
        _listen_fd->setTcpNoDelay(true);
        _listen_fd->setReuseAddr(true);
        _listen_fd->setReusePort(true);
        if(_listen_fd->bind(ip, port) < 0)
        {
            return;
        }
        _listen_fd->listen();
        if(nullptr == ip)
        {
            server_ip = "any address";
        }else{
            server_ip = ip;
        }
        server_port = port;
    }
    auto loop = std::bind(&TcpServer::server_loop, this);
    co_go(loop);
    return;
}

void TcpServer::start_multi(const char* ip,int port)
{
    auto tCnt = ::get_nprocs_conf();
    if(_on_server_connection == nullptr)
    {
        register_connection(default_connection);
    }
    _multi_listen_fd = new Socket[tCnt];
    for(int i = 0; i < tCnt; ++i)
    {
        if(_multi_listen_fd[i].isUseful())
        {
            _multi_listen_fd[i].setTcpNoDelay(true);
            _multi_listen_fd[i].setReuseAddr(true);
            _multi_listen_fd[i].setReusePort(true);
            if(_multi_listen_fd[i].bind(ip,port) < 0)
            {
                return;
            }
            _multi_listen_fd[i].listen();
        }

        auto loop = std::bind(&TcpServer::multi_server_loop,this,i);

        co_go(loop, coroutineStackSize,i);
    }
    return;
}

void TcpServer::server_loop()
{

    while(true)
    {

        Socket* conn = new Socket(_listen_fd->accept());
        std::cout << "server accept" <<  std::endl;
        conn->setTcpNoDelay(true);
        auto user_connection = std::bind(*_on_server_connection,conn);
        co_go(user_connection);
    }

    return;
}

void TcpServer::multi_server_loop(int thread_number)
{
    while(true)
    {
        Socket* conn = new Socket(_multi_listen_fd[thread_number].accept());
        conn->setTcpNoDelay(true);
        auto user_connection = std::bind(*_on_server_connection, conn);
        co_go(user_connection);
    }
    return;
}
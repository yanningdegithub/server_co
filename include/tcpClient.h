/*
 *@brief 封装客户端操作
 *@author yanning
 *@date 2022-04-22
 */
#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <functional>
#include <memory>
#include <sys/sysinfo.h>
#include "socket.h"
#include "server_api.h"

namespace serverco
{
    class TcpClient
    {
    public:  
        TcpClient() : m_client_socket(new Socket())
        {}
        
        DISALLOW_COPY_MOVE_AND_ASSIGN(TcpClient);

        virtual ~TcpClient()
        {
            delete m_client_socket;
            m_client_socket = nullptr;
        }

        void connect(const char* ip, int port);

        int disconnect();

        size_t recv(void* buf, size_t count);

        size_t send(const void* buf, size_t count);

        inline int socket() const { return m_client_socket->fd(); }
    private:  
        Socket* m_client_socket;
    };
}

#endif
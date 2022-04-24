#include "../include/tcpClient.h"
using namespace serverco;
void TcpClient::connect(const char* ip, int port)
{
    return m_client_socket->connect(ip, port);
}

int TcpClient::disconnect()
{
    return m_client_socket->shutdownWrite();
}

size_t TcpClient::recv(void* buf, size_t count)
{
    return m_client_socket->read(buf, count);
}

size_t TcpClient::send(const void* buf, size_t count)
{
    return m_client_socket->send(buf, count);
}
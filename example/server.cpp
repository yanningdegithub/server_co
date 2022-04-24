#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/sysinfo.h>
#include "../include/tcpServer.h"

int main()
{
    serverco::TcpServer tcpServer;
    tcpServer.start(nullptr, 12345);
    serverco::sche_join();
    return 0;
}
#include <iostream>
#include <string>
#include <sys/sysinfo.h>
#include "../include/tcpClient.h"
void tcpClientWorker(serverco::TcpClient& tcpClient)
{
    tcpClient.connect("127.0.0.1", 12345);
    std::cout << "cliend connect" << std::endl;
    while(true)
    {

        
        char buf[1024];
        tcpClient.send("ping", 4);
        std::cout << "client send" << std::endl;
        tcpClient.recv(buf, 1024);
        std::cout << "client recv" << std::endl;
        sleep(5);
    }
}

int main()
{
    serverco::TcpClient tcpClinet;
    // int loop_time = 100;
    serverco::co_go([&tcpClinet](){
        tcpClientWorker(tcpClinet);
    });
    serverco::sche_join();
    return 0;
}
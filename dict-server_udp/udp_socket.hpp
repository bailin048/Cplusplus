#pragma once 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <string>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

class Udpsocket{
public:
    Udpsocket():_fd(-1){}
    bool Socket(){
        _fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(_fd < 0){
            perror("socket");
            return false;
        }
        return true;
    }
    bool Close(){
        close(_fd);
        return true;
    }
    bool Bind(const string& ip, uint16_t port){
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr  = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        int ret = bind(_fd, (sockaddr*)&addr, sizeof(addr));
        if(ret < 0){
            perror("bind");
            return false;
        }
        return true;
    }
    bool Recvfrom(string* buf, string* ip = NULL, uint16_t* port = NULL){
        char tmp[1024*10] = {0};
        sockaddr_in peer;
        socklen_t len = sizeof(peer);
        ssize_t read_size = recvfrom(_fd, tmp, sizeof(tmp)-1, 0, (sockaddr*)&peer, &len);
        if(read_size < 0){
            perror("recvfrom");
            return false;
        }
        buf->assign(tmp, read_size);
        if(NULL != ip)
            *ip = inet_ntoa(peer.sin_addr);
        if(NULL != ip)
            *port = ntohs(peer.sin_port);
        return true;
    }
    bool Sendto(string& buf, string& ip, uint16_t port){
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        ssize_t write_size = sendto(_fd, buf.data(), buf.size(), 0, (sockaddr*)&addr, sizeof(addr));
        if(write_size < 0){
            perror("sendto");
            return false;
        }
        return true;
    }
private:
    int _fd;
};

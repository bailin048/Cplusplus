#pragma once 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <cassert>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
using namespace std;
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
#define CHECK_RET(exp) if(!(exp)){ return false; }

class TcpSocket{
private:
    int _fd;
public:
    TcpSocket():_fd(-1){}
    TcpSocket(int fd):_fd(fd){}
    bool Socket(){
        _fd = socket(AF_INET, SOCK_STREAM, 0);
        if(_fd < 0){
            perror("socket error!");
            return false;
        }
        printf("open fd = %d\n", _fd);
        return true;
    }
    bool Close(){
        close(_fd);
        printf("close fd = %d\n", _fd);
        return true;
    }
    bool Bind(const string& ip, uint16_t port)const{
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        int ret = bind(_fd, (sockaddr*)&addr, sizeof(addr));
        if(ret < 0){
            perror("bind");
            return false;
        }
        return true;
    }
    bool Listen(int num)const{
        int ret = listen(_fd, num);
        if(ret < 0){
            perror("listen");
            return false;
        }
        return true;
    }
    bool Accept(TcpSocket* peer, string* ip = NULL, uint16_t* port = NULL)const{
        sockaddr_in peer_addr;
        socklen_t len = sizeof(peer_addr);
        int new_sock = accept(_fd, (sockaddr*)&peer_addr, &len);
        if(new_sock < 0){
            perror("accept");
            return false;
        }
        printf("accept fd = %d\n", new_sock);
        peer->_fd = new_sock;
        if(ip != NULL)
            *ip = inet_ntoa(peer_addr.sin_addr);
        if(port != NULL)
            *port = ntohs(peer_addr.sin_port);
        return true;
    }
    bool Recv(string* buf)const{
        buf->clear();
        char tmp[1024*10] = {0};
        ssize_t read_size = recv(_fd, tmp, sizeof(tmp), 0);
        if(read_size < 0){
            perror("recv");
            return false;
        }
        if(0 == read_size)
            return false;
        buf->assign(tmp, read_size);
        return true;
    }
    bool Send(const string& buf)const{
        ssize_t write_size = send(_fd, buf.data(), buf.size(), 0);
        if(write_size < 0){
            perror("send");
            return false;
        }
        return true;
    }
    bool Connect(const string& ip, uint16_t port)const{
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(port);
        int ret = connect(_fd, (sockaddr*)&addr, sizeof(addr));
        if(ret < 0){
            perror("connect");
            return false;
        }
        return true;
    }
    int GetFd()const{
        return _fd; 
    }
};

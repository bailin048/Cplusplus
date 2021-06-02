#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;
#define CHECK_RET(q) if((q)==false){return -1;}
#define MAX_LISTEN 5

class TcpSocket{
public:
    TcpSocket():_sockfd(-1){}
    bool Socket(){
        _sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(_sockfd < 0){
            perror("socket error");
            return false;
        }
        return true;
    }
    void Addr(struct sockaddr_in* addr,const string& ip,uint16_t port){
        addr->sin_family = AF_INET;
        addr->sin_port = htons(port);
        addr->sin_addr.s_addr = inet_addr(ip.c_str());
        return;
    }
    bool Bind(const string& ip,uint16_t port){
        struct sockaddr_in addr;
        Addr(&addr,ip,port);
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = bind(_sockfd,(sockaddr*)&addr,len);
        if(ret<0){
            perror("bind error");
            return false;
        }
        return true;
    }
    bool Listen(int backlog = MAX_LISTEN){
        int ret = listen(_sockfd,backlog);
        if(ret < 0){
            perror("listen error");
            return false;
        }
        return true;
    }
    bool Connect(const string& ip,uint16_t port){
        struct sockaddr_in addr;
        Addr(&addr,ip,port);
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = connect(_sockfd,(sockaddr*)&addr,len);
        if(ret < 0){
            perror("connect error");
            return false;
        }
        return true;
    }
    bool Accept(TcpSocket* sock,string* ip = NULL,uint16_t *port= NULL){
        struct sockaddr_in addr;
        socklen_t len = sizeof(struct sockaddr_in);
        int newfd = accept(_sockfd,(sockaddr*)&addr,&len);
        if(newfd < 0){
            perror("accept error");
            return false;
        }
        sock->_sockfd = newfd;
        if(ip)
            *ip = inet_ntoa(addr.sin_addr);
        if(port)
            *port = ntohs(addr.sin_port);
        return true;
    }
    bool Send(const string& data){
        int total = 0;
        while(total < data.size()){
            int ret = send(_sockfd,&data[0]+total,
                    data.size()-total,0);
            if(ret <0){
                perror("send error");
                return false;
            }
            total += ret;
        }
        return true;
    }
    bool Recv(string* buf){
        char tmp[4096] = {0};
        int ret = recv(_sockfd,tmp,4096,0);
        if(ret <0){
            perror("recv error");
            return false;
        }
        else if(!ret){
            cout<<"peer shutdown"<<endl;
            return false;
        }
        buf->assign(tmp,ret);
        return true;
    }
    bool Close(){
        if(_sockfd != -1)
            close(_sockfd);
        return true;
    }
private:
    int _sockfd;
};



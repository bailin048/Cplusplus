#pragma once 
#include "tcp_socket.hpp"
#include <vector>
#include <unordered_map>
#include <functional>
#include <sys/select.h>
using namespace std;

inline void PrintFdSet(fd_set* fds, int max_fd){
    printf("select fds: ");
    for(int i = 0; i < max_fd+1; ++i){
        if(!FD_ISSET(i, fds))
            continue;
        printf("%d ", i);
    }
    printf("\n");
}

typedef function<void(const string& req, string* resp)> Handler;

class Selector{
public:
    Selector(){
        max_fd = 0;
        FD_ZERO(&read_fds);
    }
    bool Add(const TcpSocket& sock){
        int fd = sock.GetFd();
        printf("[Selector::Add] %d\n", fd);
        if(fd_map.find(fd) != fd_map.end()){
            printf("Add failed! fd has in Selector!\n");
            return false;
        }
        fd_map[fd] = sock;
        FD_SET(fd, &read_fds);
        if(fd > max_fd)
            max_fd = fd;
        return true;
    }
    bool Del(const TcpSocket& sock){
        int fd = sock.GetFd();
        printf("[Selector::Del] %d\n", fd);
        if(fd_map.find(fd) == fd_map.end()){
            printf("Del failed! fd has not in Selector!\n");
            return false;
        }
        fd_map.erase(fd);
        FD_CLR(fd, &read_fds);
        for(int i = max_fd; i >=0 ; --i){
            if(!FD_ISSET(i, &read_fds))
                continue;
            max_fd = i;
            break;
        }
        return true;
    }
    bool Wait(vector<TcpSocket>* output){
        output->clear();
        fd_set tmp = read_fds;
        PrintFdSet(&tmp, max_fd);
        int nfds = select(max_fd+1, &tmp, NULL, NULL, NULL);
        if(nfds < 0){
            perror("select");
            return false;
        }
        for(int i = 0; i < max_fd+1; ++i){
            if(!FD_ISSET(i, &tmp))
                continue;
            output->push_back(fd_map[i]);
        }
        return true;
    }
private:
    fd_set read_fds;
    int max_fd;
    unordered_map<int, TcpSocket> fd_map;
};

class TcpSelectServer{
public:
    TcpSelectServer(const string& ip, uint16_t port):_ip(ip),_port(port){}
    bool Start(Handler handler)const{
        //1.创建socket
        TcpSocket listen_sock;
        bool ret = listen_sock.Socket();
        if(!ret)
            return false;
        //2.绑定端口号
        ret = listen_sock.Bind(_ip, _port);
        if(!ret)
            return false;
        //3.进行监听
        ret = listen_sock.Listen(5);
        if(!ret)
            return false;
        //4.创建Selector对象
        Selector selector;
        selector.Add(listen_sock);
        //5.进入事件循环
        for(;;){
            vector<TcpSocket> output;
            bool ret = selector.Wait(&output);
            if(!ret)
                continue;
            //6.根据就绪的文件描述符的差别，决定后续的处理逻辑
            for(size_t i = 0; i < output.size(); ++i){
                if(output[i].GetFd() == listen_sock.GetFd()){
                    //如果就绪的文件描述符是listen_sock，就执行accept，并加入select中
                    TcpSocket new_sock;
                    listen_sock.Accept(&new_sock, NULL, NULL);
                    selector.Add(new_sock);
                }else{
                    string req,resp;
                    bool ret = output[i].Recv(&req);
                    if(!ret){
                        //需要关闭socket
                        selector.Del(output[i]);
                        output[i].Close();
                        continue;
                    }
                    handler(req, &resp);
                    output[i].Send(resp);
                }
            }
        }
        return true;
    }
private:
    string _ip;
    uint16_t _port;
};

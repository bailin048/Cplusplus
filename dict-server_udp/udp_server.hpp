#pragma once
#include "udp_socket.hpp"
#include <functional>

typedef function<void(const string&, string*resp)> Handler;

class Udpserver{
public:
    Udpserver(){
        assert(_sock.Socket());
    }
    ~Udpserver(){
        _sock.Close();
    }
    bool Start(const string& ip, uint16_t port, Handler handler){
        bool ret = _sock.Bind(ip, port);
        if(!ret)
            return false;
        for(;;){
            string req;
            string remote_ip;
            uint16_t remote_port = 0;
            bool ret = _sock.Recvfrom(&req, &remote_ip, &remote_port);
            if(!ret)
                continue;
            string resp;
            handler(req, &resp);
            _sock.Sendto(resp, remote_ip, remote_port);
            printf("[%s:%d] req: %s, resp:%s\n",remote_ip.c_str(), remote_port, req.c_str(), resp.c_str());
        }
        _sock.Close();
        return true;
    }
private:
    Udpsocket _sock;
};

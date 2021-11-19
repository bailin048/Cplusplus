#pragma once 
#include "udp_socket.hpp"

class Udpclient{
public:
    Udpclient(const string& ip, uint16_t port):_ip(ip),_port(port){
        assert(_sock.Socket());
    }
    ~Udpclient(){
        _sock.Close();
    }
    bool RecvFrom(string* buf){
        return _sock.Recvfrom(buf);
    }
    bool SendTo(string& buf){
        return _sock.Sendto(buf, _ip, _port);
    }
private:
    Udpsocket _sock;
    string _ip;
    uint16_t _port;
};

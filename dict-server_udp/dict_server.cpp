#include "udp_server.hpp"
#include <unordered_map>
#include <iostream>
using namespace std;

unordered_map<string,string> g_dict;

void Translate(const string& req, string* resp){
    auto it = g_dict.find(req);
    if(it == g_dict.end()){
        *resp = "未查到!";
        return;
    }
    *resp = it->second;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        cout<<"Usage ./dict_server [ip] [port]"<<endl;
        return 1;
    }
    g_dict.insert(make_pair("hello","你好"));
    g_dict.insert(make_pair("world","世界"));
    g_dict.insert(make_pair("teacher","老师"));
    g_dict.insert(make_pair("student","学生"));
    g_dict.insert(make_pair("preofessor","教授"));

    Udpserver server;
    server.Start(argv[1],atoi(argv[2]),Translate);
    return 0;
}

#include "tcp_select_server.hpp"
#include <iostream>
using namespace std;

unordered_map<string,string> g_dict;

void Translate(const string& req, string* resp){
    auto it = g_dict.find(req);
    if(it == g_dict.end()){
        *resp = "未找到！";
        return;
    }
    *resp = it->second;
    return;
}

int main(int argc, char* argv[]){
    if(3 != argc){
        cout<<"Usage ./server [ip] [port]"<<endl;
        return 1;
    }
    g_dict.insert(make_pair("world","世界"));
    g_dict.insert(make_pair("happy","开心"));
    g_dict.insert(make_pair("sad","难过"));
    g_dict.insert(make_pair("cry","哭泣"));
    TcpSelectServer server(argv[1],atoi(argv[2]));
    server.Start(Translate);
    return 0;
}

#include "tcp_client.hpp"
#include <iostream>

int main(int argc, char* argv[]){
    if(3 != argc){
        cout<<"Usage: ./client [ip] [port]\n";
        return 1;
    }
    TcpClient client(argv[1], atoi(argv[2]));
    bool ret = client.Connect();
    if(!ret)
        return 1;
    for(;;){
        cout<<"请输入想要查询的单词："<<endl;
        string word;
        cin>>word;
        if(!cin)
            break;
        if("q" == word)
            return 0;
        client.Send(word);
        string result;
        client.Recv(&result);
        cout<<result<<endl;
    }
    return 0;
}

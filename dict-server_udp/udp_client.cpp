#include "udp_client.hpp"
#include <iostream>

int main(int argc, char* argv[]){
    if(argc!=3){
        cout<<"Usage ./udp_server [ip] [port]"<<endl;
        return 1;
    }
    Udpclient client(argv[1], atoi(argv[2]));
    for(;;){
        string word;
        cout<<"请输入您要查的单词：";
        cin>>word;
        if(word=="q"){
            cout<<"再见！"<<endl;
            break;
        }
        client.SendTo(word);
        string result;
        client.RecvFrom(&result);
        cout<<word<<"意思是"<<result<<endl;
    }
    return 0;
}

#include "tcpsocket.hpp"
#include <signal.h>
void worker(TcpSocket& new_sock){
    bool ret;
    while(1){
        //5.通过新建链接进行通信
        string buf;
        ret = new_sock.Recv(&buf);
        if(!ret){//通信结束
            new_sock.Close();
            return;
        }
        cout<<"client say:"<<buf<<endl;
        buf.clear();
        cout<<"server say:";
        cin>>buf;
        ret = new_sock.Send(buf);//发送数据
        if(!ret){
            new_sock.Close();
            return;
        }
    }
}

int main(int argc,char* argv[]){
    if(argc!=3){
        cout<<"usage: ./tcp_pro_cli 172.17.0.4 9000"<<endl;
        return -1;
    }
    signal(SIGCHLD,SIG_IGN);//避免产生僵尸进程//子进程忽略处理
    string ip = argv[1];
    uint16_t port = stoi(argv[2]);

    TcpSocket lst_sock;
    //1.创建套接字
    CHECK_RET(lst_sock.Socket());
    //2.绑定地址信息
    CHECK_RET(lst_sock.Bind(ip,port));
    //开始监听
    CHECK_RET(lst_sock.Listen());
    while(1){
        //4.获取新建连接
        TcpSocket new_sock;
        bool ret = lst_sock.Accept(&new_sock,&ip,&port);
        if(!ret){//新建socket失败，稍后重试
            usleep(1000);
            continue;
        }
        cout<<"new conn-"<<ip<<":"<<port<<endl;
        //创建子进程管理新建通信
        pid_t pid = fork();
        if(!pid){//子进程管理新通信
            worker(new_sock);
            new_sock.Close();//通信结束，释放socket
            exit(0);//子进程结束
        }
        new_sock.Close();//因为发生写时拷贝，父进程socket不释放浪费资源
    }
    lst_sock.Close();//关闭监听套接字
    return 0;
}


#include "udpsocket.hpp"
#include <string>
using namespace std;
#define CHECK_RET(q) if((q)==false){return -1;}
int main(){
    UdpSocket sock;
    //1.创建套接字
    CHECK_RET(sock.Socket());
    //2.绑定地址信息
    while(1){
        //3.发送数据
        string buf;
        cout<<"client say: ";
        cin>>buf;
        CHECK_RET(sock.Send(buf,"172.17.0.4",9000));
        //4.接收数据
        buf.clear();
        CHECK_RET(sock.Recv(&buf));
        cout<<"server say: "<<buf;
    }
    //5.关闭套接字
    CHECK_RET(sock.Close());
    return 0;
}

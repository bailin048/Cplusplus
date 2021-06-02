#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>//字节序转化接口头文件
#include <netinet/in.h>//地址结构协议类型头文件
#include <sys/socket.h>//socket头文件

int main(){
    //1.创建套接字
    int sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sockfd < 0){
        perror("socket error");
        return -1;
    }
    //2.绑定地址信息
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.s_addr = inet_addr("172.17.0.4");
    socklen_t len = sizeof(struct sockaddr_in);
    int ret = bind(sockfd,(struct sockaddr*)&addr,len);
    if(ret < 0){
        perror("bind error");
        return -1;
    }
    while(1){
        //3.接收数据
        char buf[1042] = {0};
        struct sockaddr_in peer_addr;
        ret = recvfrom(sockfd,buf,1023,0,(struct sockaddr*)&peer_addr,&len);
        if(ret < 0){
            perror("recvfrom error");
            close(sockfd);
            return -1;
        }
        printf("client-[%s:%d] say:%s\n",
                inet_ntoa(peer_addr.sin_addr),
                ntohs(peer_addr.sin_port),buf);
        //发送数据
        memset(buf,0x00,1024);
        printf("server say:");
        fflush(stdout);
        fgets(buf,1023,stdin);
        ret = sendto(sockfd,buf,strlen(buf),0,
                (struct sockaddr*)&peer_addr,len);
        if(ret < 0)
            perror("sendto error\n");
        if(ret < 0){
            perror("sendto error\n");
            close(sockfd);
            return -1;
        }
    }
    //5.关闭套接字
    close(sockfd);
    return 0;
}

#include<stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h> 
#include <stdlib.h>
#include <ctype.h>



#define MAXLINE 80  // 最大连接数
#define SERV_PORT 6666  // 服务端口号


int main() 
{
   struct sockaddr_in servaddr, cliaddr;
   socklen_t cliaddr_len;
   int listenfd, connfd;
   char buf[MAXLINE];
   char str[INET_ADDRSTRLEN];
   int i, n;

   // 创建套接字
   listenfd = socket(AF_INET, SOCK_STREAM, 0);

   // 初始化服务端端口地址
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
   servaddr.sin_port = htons(SERV_PORT);

   // 套接字文件与服务端口绑定
   bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

   // 开始侦听，并指定最大连接数
   listen(listenfd, MAXLINE);

   printf("Server is running...\n");

   // 接收客户端数据，并处理请求
   while (1)
   {
        cliaddr_len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        n = recv(connfd, buf, MAXLINE, 0);

        printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));

        for(i = 0; i < n; i++) {
            buf[i] = toupper(buf[i]);
        }
        send(connfd, buf, n, 0);
        close(connfd);
   }
   return 0;
   
}
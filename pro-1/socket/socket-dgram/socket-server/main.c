#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<ctype.h>


#define MAXLINE 80   // 最大连接数
#define  SERV_PORT 6666  // 服务端口号
int main(void) {
    struct sockaddr_in servaddr, cliaddr;  // 定义服务器与客户端地址结构
    socklen_t cliaddr_len;                 // 客户端地址长度
    int sockfd;                            // 服务器socket 文件描述符
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    int i, n;

    // 创建服务器端套接字文件
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 初始化服务器端口地址
    bzero(&servaddr, sizeof(servaddr)); //地址结构体
    servaddr.sin_family = AF_INET;  // 指定协议族
    // servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr);
    servaddr.sin_port = htons(SERV_PORT); // 指定端口

    // 绑定套接字到本地IP和端口
    int ret = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (0>ret) {
        printf("bind error\n");
        return -1;
    }

    printf("UDP server is ready at %s port %d\n", inet_ntop(AF_INET, &servaddr.sin_addr, str, sizeof(str)), ntohs(servaddr.sin_port));

   //数据传输
   while(1) {
    cliaddr_len = sizeof(cliaddr);
    
    // 接收数据
    n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &cliaddr_len);
    if (n == -1) {
        perror("recvfrom error");
        printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
        exit(1);
    }
    // 数据验证：
    if (n <=0 || n >= MAXLINE) {
        printf("Invalid data received ");
        continue;
    }
    // 转换大写字母并返回
    for (i = 0; i < n; i++) {
        buf[i] = toupper(buf[i]);;
    }
    // 回送数据
    n = sendto(sockfd, buf, n, 0, (struct sockaddr *)&cliaddr, cliaddr_len);
    if (n == -1) 
        perror("sendto error");
   }

   close(sockfd);

   return 0;
}   
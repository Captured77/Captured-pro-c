#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define MAXLINE 80
#define SERV_PORT 6666

int main(int argc, char *argv[]) 
{   
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    int socketfd, n;
    char *str;

    if(argc != 2) {
        fputs("Usage: ./client message \n", stderr);
        exit(1);
    }

    str = argv[1];

    // 创建客户端套接字文件
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    // 初始化服务器端口地址
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);

    // 请求连接到服务器
    connect(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    
    // 发送数据
    send(socketfd, str, strlen(str), 0);

    // 接收返回的数据
    n = recv(socketfd, buf, MAXLINE, 0);
    printf("Response from server:\n");

    buf[n] = '\0';  // 加上结束符

    write(STDERR_FILENO, buf, n);

    printf("\n");
     printf("Response from server:\n%s\n", buf);

    close(socketfd);

    return 0;
}
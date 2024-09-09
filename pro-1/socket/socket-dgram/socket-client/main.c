#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 1024
#define SERV_PORT  6666

int main(int argc, char *argv[]) {
    struct sockaddr_in servaddr;
    int sockfd, n;
    char buf[MAXLINE];
    
    // socket句柄初始化  
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 地址结构体初始化
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    // servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);

    printf("Server Address: %s\n", inet_ntoa(servaddr.sin_addr));
    printf("Server Port: %d\n", ntohs(servaddr.sin_port));
    printf("Server Family: %d\n", servaddr.sin_family);

    // 发送数据到服务端
    while(fgets(buf, MAXLINE, stdin) != NULL) {
        n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

        if (n == -1) {
            perror("sendto");
            exit(1);
        }

        // 接收到服务端返回的数据
        n = recvfrom(sockfd, buf, MAXLINE, 0, NULL, 0);
        if (n == -1) {
            perror("recvfrom");
            exit(1);
        }

        //数据长度验证
        if(n <=0 || n>MAXLINE) {
            printf("Invalid data received\n");
            continue;
        }

        // buf[n-1] = '';
        buf[n] = '\0'; 
        fputs(buf, stdout);
    }

    close(sockfd);

    return 0;

}
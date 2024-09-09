
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "wrap.h"
#include <stddef.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <ctype.h>

#define MAXLINE 80
#define SERV_PORT 6666

void do_sigchild(int num) {
    while (waitpid(0, NULL, WNOHANG) > 0);
}

int main() {
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];

    int i, n;
    pid_t pid;

    struct sigaction newact;
    memset(&newact, 0, sizeof(newact));
    newact.sa_handler = do_sigchild;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGCHLD, &newact, NULL);  // 信号处理获取与处理
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    // 设置服务器端口地址
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    Listen(listenfd, 20);

    printf("Echo Server is running...\n");

    while (1) {
        cliaddr_len = sizeof(cliaddr);
        connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

        pid = fork();

        if (pid == 0) {
            Close(listenfd); // 关闭监听套接字
            while(1) {
                n = Read(connfd, buf, MAXLINE);
                if (n == 0) {
                    printf("the other side has been closed.\n");
                    break;  // 子进程
                }

                printf("received from %s at PORT %d: %s\n",
                    inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
                    ntohs(cliaddr.sin_port), buf);
            
                for (i=0; i<n; i++) {
                    buf[i] = toupper(buf[i]);  // 转为大写
                }
                write(connfd, buf, n);

            }
        } 
        else if (pid > 0) {
            Close(connfd); // 关闭已连接套接字
        }
        else {
            perror("fork error");
        }  // 父进程

    }
    Close(listenfd);
    return 0;
}
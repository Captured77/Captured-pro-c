#include <ctype.h>
#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include<arpa/inet.h>
#define MAXLINE 80
#define SERV_PORT 6666
#define OPEN_MAX 1024

int main() {
    int listenfd, connfd, maxfd, sockfd, maxi, i, j;
    ssize_t n;
    int nready, efd, res;
    char buf[MAXLINE], str[INET_ADDRSTRLEN];
    socklen_t clilen;
    int client[OPEN_MAX];

    struct sockaddr_in servaddr, cliaddr;
    struct epoll_event tep, ep[OPEN_MAX];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 20);

    // 初始化client集合
    for(i=0;i<OPEN_MAX; i++) {
        client[i] = -1;
    }

    maxi = -1;
    efd = epoll_create(OPEN_MAX);

    if (efd == -1)
    {
        perror("epoll_create");
        exit(EXIT_FAILURE);
    }
    // 初始化tep
    tep.events = EPOLLIN;
    tep.data.fd = listenfd;
    // 微服务器进程注册事件（listenfd）
    res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep);
    if (res == -1)
    {
        perror("epoll_ctl: listenfd");
        exit(EXIT_FAILURE);
    }
    for(;;) {
        nready = epoll_wait(efd, ep, OPEN_MAX, -1);
        if(nready == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        // 处理就绪事件
        for(i = 0; i < nready; i++) {
            if(!(ep[i].events & EPOLLIN)) {
                continue;
            }

            // 若fd为listenfd, 表示连接请求到达
            if(ep[i].data.fd == listenfd) {
                clilen = sizeof(cliaddr);
                connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
              printf("received from %s at PORT %d\n", 
                    inet_ntop(AF_INET,&cliaddr.sin_addr, str, sizeof(str)),
                    ntohs(cliaddr.sin_port));
                // 将accept 获取到的文件描述符保存到client[]数组中
                for(j=0;j<OPEN_MAX; j++)
                if(client[j] < 0 ) {
                    client[j] = connfd;
                    break;
                }
                if(j == OPEN_MAX){
                    perror("open too many connections");
                    exit(EXIT_FAILURE);
                }
                if(j>maxi) {
                    maxi = j;  // 更新最大文件描述符
                }
                tep.events = EPOLLIN;
                tep.data.fd = connfd;
                //为新建立的连接的进程注册事件
                res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
                if(res == -1) {
                    perror("epoll_ctl: new connection");
                    exit(EXIT_FAILURE);
                }
            } 
            else {
                // 若fd不等于listenfd, 表示就绪是各路连接
                sockfd = ep[i].data.fd;
                n = read(sockfd, buf, MAXLINE);
                if(n == 0) {
                    for(j=0;j<=maxi;j++) {
                        if(client[j] == sockfd) {
                            client[j] = -1;
                            break;
                        }
                    }
                    // 取消监听
                    res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
                    if (res == -1) {
                        perror("epoll_ctl: delete");
                        exit(EXIT_FAILURE);
                    }
                    close(sockfd);
                    printf("client[%d] closed connnection\n", j);
                }
                else {
                    for(j=0; j<n; j++)
                        buf[j] = toupper(buf[j]);
                    write(sockfd, buf, n);
                }

            }
        }
    }
    close(listenfd);
    close(efd);
    return 0;
}
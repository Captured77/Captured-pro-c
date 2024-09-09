


#include <ctype.h>
#include <stdio.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAXLINE 80
#define SERV_PORT 6666
int main() {
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];

    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];

    socklen_t cliaddr_len;
    struct sockaddr_in servaddr, cliaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    printf("Connecting~~~\n");
    printf("servaddr: sin_family=%d, sin_addr=%s, sin_port=%d\n", servaddr.sin_family, inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 20);  // 默认最大128

    maxfd = listenfd;  // 初始时，maxfd为listenfd
    maxi = -1;
    // 初始化监控列表
    for(i=0;i<FD_SETSIZE;i++) 
        client[i] = -1;    // 使用-1 初始话client[]中的元素
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset); //将listenfd 添加到文件描述符中

    // 循环监测处于连接状态进程的文件描述符
    for(;;) {
        // 使用变量rset获取文件描述符集合
        rset = allset;

        // 记录就绪的进程数量
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
        if (nready < 0) {
            printf("select error: %d\n", nready);
            perror("select error");
        }

        // 如果有新的连接则处理新的请求
        if(FD_ISSET(listenfd, &rset)) {
            cliaddr_len = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);

            printf("Received a connection from %s, port %d, socket %d\n",
                inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
                ntohs(cliaddr.sin_port), connfd);

            for(i=0;i<FD_SETSIZE;i++) {
                if(client[i]<0) {
                    client[i] = connfd;   // 将文件描述符connfd保存到client[]中
                    break;
                }
            }
            if(i==FD_SETSIZE) {         // 判断连接数是否已经到达上限
                perror("Too many clients");
            }

            FD_SET(connfd, &allset);   // 添加新文件描述符到监控信号集中
            if(connfd > maxfd)         // 更新最大文件描述符
                maxfd = connfd;
            if(i > maxi)               // 更新最大文件描述符
                maxi = i; 
            if(--nready == 0)          // 若没有文件描述符就绪，便返回select,继续阻塞监测剩余的文件描述符
                continue;
        }
        // 遍历文件描述符集，处理已经就绪的文件描述符
        for(i=0; i<=maxi; i++) {
            if ((sockfd = client[i]) < 0)
                continue;
            
            if(FD_ISSET(sockfd, &rset)) {
                // n=0; client 就绪但未读到数据，标识client 将关闭连接
                if((n = read(sockfd, buf, MAXLINE)) == 0) {
                    // 关闭服务器端连接
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    int j;
                    for (j=0; j<n; j++) {
                        // buf[j] = '\0';
                        buf[j] = toupper(buf[j]);
                    }
                    write(sockfd, buf, n);
                }
                if(--nready == 0)
                break;
            }
        }

    }
    close(listenfd);
    return 0;  // 正常结束
}   
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include "wrap.h"


#define MAXLINE 80
#define SERV_PORT 6666

struct s_info {
    struct sockaddr_in cliaddr;
    int connfd;
};

void *do_work(void *arg) {
    int n, i;
    struct s_info *ts = (struct s_info *)arg;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];

    printf("I'm comming~~\n");
    // 使子线程处于分离态，保证子线程资源可被回收
    pthread_detach(pthread_self());

    while(1) {
        n = Read(ts->connfd, buf, MAXLINE);
        if (n ==0) {
            printf("Client disconnected.\n");
            break;
        }
        printf("Received from %s at PORT %d: %s\n",
               inet_ntop(AF_INET, &ts->cliaddr.sin_addr, str, sizeof(str)),
               ntohs(ts->cliaddr.sin_port), buf);
        
        for (i =0; i<n; i++) {
            buf[i] = toupper(buf[i]);
        }
        Write(ts->connfd, buf, n);
    }

    Close(ts->connfd);
    return NULL;

}

int main(void) {
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd, i=0;

    pthread_t tid;
    struct s_info ts[383];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);


    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    Listen(listenfd, 20);
    printf("Accepting connection ...\n");

    while (1)
    {
        cliaddr_len = sizeof(cliaddr);
        connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        ts[i].cliaddr = cliaddr;
        ts[i].connfd = connfd;

        pthread_create(&tid, NULL, do_work, (void *)&ts[i]);
        i++;
    }
    
    Close(listenfd);  // 关闭��听套接字

    return 0;
    
}
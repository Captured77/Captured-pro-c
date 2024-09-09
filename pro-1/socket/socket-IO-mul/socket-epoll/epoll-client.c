
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define MAXLINE 80
#define SERV_PORT 6666
int main() {
    struct sockaddr_in servaddr;
    char buf[MAXLINE];

    int sockfd, n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while(fgets(buf, MAXLINE, stdin) != NULL) {
        write(sockfd, buf, strlen(buf));
        n = read(sockfd, buf, MAXLINE);
        if (n == 0 ) {
            printf("the other side has been closed\n");
        } 
        printf("Received from server: %s\n", buf);
    }

    close(sockfd);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
// #include "../socket-multhreads-server/wrap.h"

#define MAXLINE 80
#define SERV_PORT 6666
// int main(int argc, char *argv[]) {
//     struct sockaddr_in servaddr;
//     char buf[MAXLINE];
//     int sockfd, n;

//     sockfd = Socket(AF_INET, SOCK_STREAM, 0);
//     bzero(&servaddr, sizeof(servaddr));
//     inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
//     servaddr.sin_port = htons(SERV_PORT);
//     Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

//     printf("Connected to the server. Type messages and hit ENTER.\n");

//     while (fgets(buf, sizeof(buf), stdin) != NULL) 
//     {   
//         size_t len = strlen(buf);
//         if (buf[len-1] == '\n') {
//             buf[len-1] = '\0';
//             len--;
//         }

//         Write(sockfd, buf, sizeof(buf));
//         n = Read(sockfd, buf, MAXLINE);
//         if (n == 0 ) {
//             printf("the other side has been closed.\n");
//         } else {
//             buf[n] = '\0';
//             fputs(buf, stdout);
//         }
//     }
//     Close(sockfd);
//     return 0;
// }

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buf[MAXLINE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while(fgets(buf, MAXLINE, stdin) != NULL) {
        
        write(sockfd, buf, strlen(buf));
        read(sockfd, buf, strlen(buf));

        printf("Received: %s\n", buf);

    }
    close(sockfd);
    return 0;
}
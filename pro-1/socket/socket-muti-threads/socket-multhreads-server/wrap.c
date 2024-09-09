#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include "wrap.h"

void perr_exit(const char *s) {
    perror(s);
    exit(1);
}

//socket
int Socket(int family, int type, int protocol) {
    int n;
    if((n = socket(family, type, protocol)) < 0) {
        perr_exit("socket error");
    }
    return n;
}

//bind
void Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
    if(bind(fd, sa, salen)<0 ) {
        perr_exit("bind error");
    }
}

//listen
void Listen(int fd, int backlog) {
    if(listen(fd, backlog)<0) {
        perr_exit("listen error");
    }
}

//connect
void Connect(int fd, const struct sockaddr *sa, socklen_t salen) {
    if(connect(fd, sa, salen)<0) {
        perr_exit("connect error");
    }
}

//accept
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr) {
    int n;
again:
    if((n = accept(fd, sa, salenptr)) < 0) {
        if ((errno == ECONNABORTED) || (errno == EINTR))
            goto again;
        else 
            perr_exit("accept error");
    }
    return n;
}

//close
void Close(int fd) {
    if(close(fd)<0) {
        perr_exit("close error");
    }
}

//read
ssize_t Read(int fd, void *pt, size_t nbytes) {
    ssize_t n;
again:
    if((n = read(fd, pt, nbytes)) == -1) {
        if (errno == EINTR)
            goto again;
       else
            return -1;
    }
    return n;
}

//write
ssize_t Write(int fd, const void *ptr, size_t nbytes) {
    ssize_t n;
again:
    if((n = write(fd, ptr, nbytes)) == -1) {
        if (errno == EINTR)
            goto again;
        else
            return -1;
    }
    return n;
}
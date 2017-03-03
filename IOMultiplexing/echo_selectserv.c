#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define BUF_SIZE 30

void error_handling(char* errmsg) {
    fputs(errmsg, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    int serv_sock, clnt_sock;
    int str_len, result;
    int fd_num, fd_max, i;
    struct timeval timeout;
    fd_set reads, cpy_reads;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) error_handling("socket() error!!");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((bind(serv_sock, (struct sockaddr*)(&serv_addr), sizeof(serv_addr))) == -1) error_handling("bind() error!!");
    if((listen(serv_sock, 5)) == -1) error_handling("listen() error!!");
    
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while(1) {
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        cpy_reads = reads;
        fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout);
        if(fd_num == -1) error_handling("select() error!!");
        else if(fd_num == 0) {
            puts("time out!!!");
            continue;
        } else {
            for(i = 0 ; i < fd_max+1; ++i) {
                if(FD_ISSET(i, &cpy_reads)) {
                    if(i == serv_sock) {
                        clnt_addr_sz = sizeof(clnt_addr);
                        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
                        if(fd_max < clnt_sock) fd_max = clnt_sock;
                        FD_SET(clnt_sock, &reads);
                        printf("connect client : %d\n", clnt_sock);
                    } else {
                        str_len = read(i, buf, BUF_SIZE);
                        if(str_len == 0) {
                            FD_CLR(i, &reads);
                            close(i);
                            printf("closed client: %d \n",i);
                        } else {
                            write(i, buf, str_len);
                            buf[str_len] = 0;
                            printf("Message from %d client : %s\n", i, buf);
                        }
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}

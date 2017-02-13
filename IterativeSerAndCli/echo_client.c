#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
void error_handling(char* message);

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in sock_addr;
    socklen_t addrlen = sizeof(sock_addr);
    char message[BUF_SIZE];
    int str_len = 0;
    int recv_len, recv_cnt;

    if(argc != 3) {
        printf("Usage %s <IP> <PORT>", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == -1) error_handling("socket() error!!");

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (struct sockaddr*) &sock_addr, addrlen) == -1) {
        error_handling("connect() error!!");
    }
    else {
        puts("connected...");
    }

    while(1) {
        fputs("Input Message: ",stdout);
        fgets(message, BUF_SIZE, stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        str_len = write(sock, message, strlen(message));
        while(recv_len < str_len) {
            recv_cnt = read(sock, message, BUF_SIZE - 1);
            if(recv_cnt == -1) error_handling("read() error!!");
            recv_len += recv_cnt;
        }
        message[recv_len] = 0;
        printf("Message from server: %s", message);
    }
    close(sock);
    return 0;
}

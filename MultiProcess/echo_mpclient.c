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

void read_routine(int sock, char* buf) {
    while(1){
      int str_len = 0;
      str_len = read(sock, buf, BUF_SIZE);
      if(str_len == 0) return;
      buf[str_len] = 0;
      printf("Message from server : %s\n",buf);
    }
}

void write_routine(int sock, char* buf) {
    while(1) {
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n")) {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));
    }
}

int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in sock_addr;
    socklen_t addrlen = sizeof(sock_addr);
    char message[BUF_SIZE];
    pid_t pid = 0;

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
    pid = fork();
    if(pid == 0) write_routine(sock, message);
    else read_routine(sock, message); 
    close(sock);
    return 0;
}

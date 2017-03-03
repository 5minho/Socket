#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 10000

void error_handling(char* msg) {
    fputs(msg, stderr);
    exit(1);
}

void read_childproc(int sig) {
    int status = 0;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)) {
        printf("remove %d child process \n", id);
        printf("Child send : %d \n",WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]) {
    char message[BUF_SIZE];
    int lisn_sock = 0,
        clnt_sock = 0;
    socklen_t clnt_addr_size = 0;
    int str_len = 0;
    struct sockaddr_in serv_addr,
                       clnt_addr;
    struct sigaction act;
    pid_t pid = 0;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    if((lisn_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        error_handling("create listening socket error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((bind(lisn_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1))
        error_handling("bind() error");

    if((listen(lisn_sock, 5)) == -1)
        error_handling("listen() error");
    while(1) {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(lisn_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1) continue;
        else puts("new client connected...");
        pid = fork();
        switch(pid) {
            case 0:
                close(lisn_sock);
                while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
                    write(clnt_sock, message, str_len);
                close(clnt_sock);
                puts("client disconnected...");
                return 0;
            case -1:
                close(clnt_sock);
                continue;
            default:
                close(clnt_sock);
        }
    }
    close(lisn_sock);
    return 0;
}


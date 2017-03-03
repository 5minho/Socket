#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100

void readChildProc(int sig) {
    int status = 0;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status)) {
        printf("remove %d child process \n", id);
        printf("Child send : %d \n",WEXITSTATUS(status));
    }
}

void error_handling(char* msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    int fds[2];
    pid_t pid;
    int serv_sock,
        clnt_sock;
    int str_len = 0;
    struct sockaddr_in serv_addr,
                       clnt_addr;
    socklen_t clnt_addr_sz = sizeof(clnt_addr);
    char buf[BUF_SIZE];
    struct sigaction act;
    act.sa_handler = readChildProc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    
    if(argc != 2) {
        printf("Usage %s <PORT>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if((bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) == -1) error_handling("bind() error");
    if((listen(serv_sock, 5)) == -1) error_handling("listen() error");
    
    pipe(fds);
    pid = fork();
    if(pid == 0) {
        FILE* fp = fopen("echomsg.txt","wt");
        char msgbuf[BUF_SIZE];
        int i, len;
        for(i = 0; i < 10; ++i) {
            len = read(fds[0], msgbuf, BUF_SIZE);
            fwrite((void*)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    while(1) {
        clnt_addr_sz = sizeof(clnt_sock);
       clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_sock, &clnt_addr_sz);
       if(clnt_sock == -1) continue;
       else puts("new client connect!!!!");
       pid = fork();
       if(pid == 0) {
           close(serv_sock);
           while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {
               write(clnt_sock, buf, str_len);
               write(fds[1], buf, str_len);
           }
           close(clnt_sock);
           puts("client disconnected....");
           return 4;
       } else
           close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

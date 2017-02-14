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
    char expression[BUF_SIZE];
    int oper_cnt = 0;
    int result = 0;
    int i = 0;

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
    
    fputs("Operand count : ", stdout);
    scanf("%d",&oper_cnt);
    getchar();
    expression[0] = (char)oper_cnt; 
    for(i = 0 ; i < oper_cnt ; ++i) {
        printf("Operand %d : ", i + 1);
        scanf("%d",(int*)&expression[i * 4 + 1]);
    }
    getchar();
    printf("Operator : ");
    scanf("%c",&expression[i * 4 + 1]);
    write(sock, expression, (i * 4 + 2));
    printf("complete write\n");
    
    if(read(sock, &result, 4) != -1) {
        printf("Operation result : %d\n", result);
    } else {
        error_handling("read() error!!");
    }
    close(sock);
    return 0;
}

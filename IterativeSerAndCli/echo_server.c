#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char* message);

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    int ser_sock;
    int cli_sock;
    
    int i = 0;
    int str_len = 0;

    struct sockaddr_in ser_addr;
    struct sockaddr_in cli_addr;
    char message[BUF_SIZE];

    socklen_t cli_adr_size;
    
    if(argc != 2) {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    if((ser_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1) {
        fputs("create server socket!!", stdout);
    }
    else {
        error_handling("socket() error!!");
    }
    
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(atoi(argv[1]));
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if((bind(ser_sock, (struct sockaddr*) &ser_addr, sizeof(ser_addr))) == -1) {
        error_handling("bind() error!!");
    }

    if(listen(ser_sock, 5) == -1) {
        error_handling("listen() error!!");
    }
    
    for(i = 0 ; i < 5 ;++i) {
        cli_adr_size = sizeof(cli_addr);
        cli_sock = accept(ser_sock, (struct sockaddr*) &cli_addr, &cli_adr_size);
        if(cli_sock == -1) error_handling("accept() error!!");
        else printf("Connect client %d\n",i+1);

        while((str_len = read(cli_sock, message, BUF_SIZE)) != 0) {
            printf("Receive Message : %s",message);
            write(cli_sock, message, str_len);
        }
        close(cli_sock);
    }

    close(ser_sock);
    return 0;
}

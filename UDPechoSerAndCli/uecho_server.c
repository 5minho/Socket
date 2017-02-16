#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[]) {
    int ser_sock;
    char message[BUF_SIZE];
    int str_len;

    struct sockaddr_in ser_addr, 
                       cli_addr;
    
    socklen_t cli_adr_sz;

    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    ser_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(ser_sock == -1) error_handling("socket() error!!");

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(atoi(argv[1]));
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(ser_sock, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) == -1) error_handling("bind() error!!");
    
    while(1) {
        cli_adr_sz = sizeof(cli_addr);
        str_len = recvfrom(ser_sock, message, BUF_SIZE, 0, (struct sockaddr*)&cli_addr, &cli_adr_sz);
        sendto(ser_sock, message, str_len, 0, (struct sockaddr*)&cli_addr, cli_adr_sz);
    }
    close(ser_sock);
    return 0;
}
void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}

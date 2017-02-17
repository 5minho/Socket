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
    int sock = 0;
    int recv_cnt = 0;
    FILE* fp = NULL;
    
    char buf[BUF_SIZE];

    struct sockaddr_in ser_addr;

    socklen_t cli_adr_size;
    
    if(argc != 3) {
        printf("Usage : %s <ip> <port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == -1) error_handling("socket() error!!");
    
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(atoi(argv[1]));
    ser_addr.sin_addr.s_addr = inet_addr(argv[2]); 
   
    fp = fopen("./recv_data.dat", "rw");
    connect(sock, (struct sockaddr*)&ser_addr, sizeof(ser_addr));

    while((recv_cnt = read(sock, buf, BUF_SIZE)) != EOF)
        fwrite(buf, 1, recv_cnt, fp);
    
    write(sock, "Thank you", 10);

    fclose(fp);
    close(sock);
    return 0;
}

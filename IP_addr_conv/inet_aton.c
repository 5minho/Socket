#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    char* addr = "127.232.124.79";
    struct sockaddr_in addr_inet;

    if(inet_aton(addr,&addr_inet.sin_addr))
        printf("Network ordered Integer addr : %#x \n",addr_inet.sin_addr.s_addr);
    else
        printf("Error occured");

    return 0;
}

#include "tftp.h"

int createUDPsocket(int port){
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0){
        return -1;
 
    }   
    struct sockaddr_in server;
}
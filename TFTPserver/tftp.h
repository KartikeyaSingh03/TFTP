#ifndef MY_HEADER_H__
#define MY_HEADER_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include<sys/time.h>
#include<errno.h>

#define OPCODE_RRQ   "01"
#define OPCODE_WRQ   "02"
#define OPCODE_DATA  "03"
#define OPCODE_ACK   "04"
#define OPCODE_ERR   "05"

#define MODE_NETASCII "netascii"
#define MODE_OCTET    "octet"
#define MODE_MAIL     "mail"

#define BUF_SIZE 1024 

#define MAX_TIMEOUT 5

enum connection_type {GET,PUT};

struct read_req_packet{
    char opcode[2];    
    char* file_name;
    char* mode;
};

struct write_req_packet{
    char opcode[2];    
    char* file_name;
    char* mode;
};

struct data_packet{
    char opcode[3];
    char block_number[3];
    char data[512];
};

struct ack_packet{
    char opcode[3];
    char block_number[3];
};

struct error_packet{
    char opcode[3];
    char error_code[3];
    char* error_msg;
};

struct connection{
    enum connection_type type;
    int socket;
    FILE* fp;    
};

void sendFile(int sock,struct sockaddr_in* sender,struct sockaddr_in* reciever,const char* file_name);
void recieveFile(int sock,struct sockaddr_in* sender,const char* file_name);
int construct_read_packet(char** packet,char* file_name,char* mode);
int construct_write_packet(char** packet,char* file_name,char* mode);
int construct_data_packet(char** packet,char* block_number,char* data);
int construct_ack_packet(char** packet,char* block_number);
int construct_err_packet(char** packet,char* error_code,char* error_msg);
struct read_req_packet decode_read_packet(char* packet);
struct write_req_packet decode_write_packet(char* packet);
struct data_packet decode_data_packet(char* packet);
struct ack_packet decode_ack_packet(char* packet);
struct error_packet decode_error_packet(char* packet);

#endif

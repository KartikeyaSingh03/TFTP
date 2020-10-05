#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

#define OPCODE_RRQ   "01"
#define OPCODE_WRQ   "02"
#define OPCODE_DATA  "03"
#define OPCODE_ACK   "04"
#define OPCODE_ERR   "05"

#define MODE_NETASCII "netascii"
#define MODE_OCTET    "octet"
#define MODE_MAIL     "mail"

char* error_codes[8] = {
	"Not Defined",
	"File not found",
	"Access violation",
	"Disk full or allocation exceeded",
	"Illegal TFTP operation",
	"Unknown transfer ID",
	"File already exists",
	"No such user"
};

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
    char opcode[2];
    char block_number[2];
    char data[512];
};

struct ack_packet{
    char opcode[2];
    char block_number[2];
};

struct error_packet{
    char opcode[2];
    char error_code[1];
    char* error_msg;
};

char* construct_read_packet(char* file_name,char* mode){
    int packet_size = 2 + sizeof(file_name) + 1 + sizeof(mode) + 1;
    char* packet = (char*)malloc(packet_size);
    strcpy(packet,OPCODE_RRQ);
    strcat(packet,file_name);
    strcpy(packet+sizeof(packet)+2,mode);
    return packet;
} 

char* construct_write_packet(char* file_name,char* mode){
    int packet_size = 2 + sizeof(file_name) + 1 + sizeof(mode) + 1;
    char* packet = (char*)malloc(packet_size);
    strcpy(packet,OPCODE_WRQ);
    strcat(packet,file_name);
    strcpy(packet+sizeof(packet)+2,mode);
    return packet;
}  

char* construct_data_packet(char* block_number,char* data){
    int packet_size = 516;
    char* packet = (char*)malloc(packet_size);
    strcpy(packet,OPCODE_DATA);
    strcat(packet,block_number);
    strcat(packet,data);
    return packet;
}

char* construct_ack_packet(char* block_number){
    int packet_size = 4;
    char* packet = (char*)malloc(packet_size);
    strcpy(packet,OPCODE_ACK);
    strcat(packet,block_number);
    return packet;
}

char* construct_err_packet(char* error_code,char* error_msg){
    int packet_size = 2 + 2 + sizeof(error_msg) + 1;
    char* packet = (char*)malloc(packet_size);
    strcpy(packet,OPCODE_ERR);
    strcat(packet,error_code);
    strcat(packet,error_msg);
    return packet;
}

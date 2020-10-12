#include "tftp.h"

int construct_read_packet(char** packet,char* file_name,char* mode){
    int packet_size = 2 + strlen(file_name) + 1 + strlen(mode) + 1;
    *packet = (char*)malloc(packet_size);
    strcpy(*packet,OPCODE_RRQ);
    strcat(*packet,file_name);
    strcpy(*packet+strlen(*packet)+1,mode);
    return packet_size;
} 

int construct_write_packet(char** packet,char* file_name,char* mode){
    int packet_size = 2 + strlen(file_name) + 1 + strlen(mode) + 1;
    *packet = (char*)malloc(packet_size);
    strcpy(*packet,OPCODE_WRQ);
    strcat(*packet,file_name);
    strcpy(*packet+strlen(*packet)+1,mode);
    return packet_size;
}  

int construct_data_packet(char** packet,char* block_number,char* data){
    int packet_size = 516;
    *packet = (char*)malloc(packet_size);
    strcpy(*packet,OPCODE_DATA);
    strcat(*packet,block_number);
    strcat(*packet,data);
    return packet_size;
}

int construct_ack_packet(char** packet,char* block_number){
    int packet_size = 4;
    *packet = (char*)malloc(packet_size);
    strcpy(*packet,OPCODE_ACK);
    strcat(*packet,block_number);
    return packet_size;
}

int construct_err_packet(char** packet,char* error_code,char* error_msg){
    int packet_size = 2 + 2 + strlen(error_msg) + 1;
    *packet = (char*)malloc(packet_size);
    strcpy(*packet,OPCODE_ERR);
    strcat(*packet,error_code);
    strcat(*packet,error_msg);
    return packet_size;
}

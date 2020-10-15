#include "tftp.h"

struct read_req_packet decode_read_packet(char* packet){
    int i = 2;
    int n = strlen(packet);
    int filename_len=0,mode_len=0;
    for(;packet[i] != '\0';i++){
        filename_len++;
    }
    i++;
    for(;packet[i]!='\0';i++){
        mode_len++;
    }
    struct read_req_packet data;
    data.file_name = (char*)malloc(1 + filename_len);
    data.mode = (char*)malloc(1 + mode_len);

    strcpy(data.opcode,"01");
    strcpy(data.file_name,packet+2);
    strcpy(data.mode,packet+2+filename_len+1);  

    return data; 
}

struct write_req_packet decode_write_packet(char* packet){
    int i = 2;
    int n = strlen(packet);
    int filename_len=0,mode_len=0;
    for(;packet[i] != '\0';i++){
        filename_len++;
    }
    i++;
    for(;packet[i]!='\0';i++){
        mode_len++;
    }
    struct write_req_packet data;
    data.file_name = (char*)malloc(1 + filename_len);
    data.mode = (char*)malloc(1 + mode_len);

    strcpy(data.opcode,"02");
    strcpy(data.file_name,packet+2);
    strcpy(data.mode,packet+2+filename_len+1);  

    return data; 
}

struct data_packet decode_data_packet(char* packet){
    struct data_packet data;
    strcpy(data.opcode,"03");
    data.block_number[0] = packet[2];
    data.block_number[1] = packet[3];
    data.block_number[2] = '\0';
    strcpy(data.data,packet+4);  
    return data; 
}

struct ack_packet decode_ack_packet(char* packet){
    struct ack_packet data;
    strcpy(data.opcode,"04");
    data.block_number[0] = packet[2];
    data.block_number[1] = packet[3];
    data.block_number[2] = '\0';
    return data; 
}

struct error_packet decode_error_packet(char* packet){
    struct error_packet data;
    strcpy(data.opcode,"05");
    data.error_code[0] = packet[2];
    data.error_code[1] = packet[3];
    data.error_code[2] = '\0';
    
    int i=4;
    int error_msg_len = 0;
    for(;packet[i] != '\0';++i){
        error_msg_len++;
    }
    data.error_msg = (char*)malloc(error_msg_len+1);   
    strcpy(data.error_msg,packet+4);  
    return data; 
}
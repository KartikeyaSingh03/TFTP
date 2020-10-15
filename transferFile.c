#include "tftp.h"

void get_block_num(int block_num,char block[]){
	block[0] = (block_num/10) + '0';
	block[1] = (block_num%10) + '0';
	block[2] = '\0';
}

void sendFile(int sock,struct sockaddr_in* sender,struct sockaddr_in* reciever,const char* file_name){
    FILE* fp_send = fopen(file_name,"r");
    char buf[BUF_SIZE];
    int length = sizeof(struct sockaddr_in);
    fseek(fp_send,0L,SEEK_END);
    int file_len = ftell(fp_send);
    fseek(fp_send,0L,SEEK_SET);

    int num_blocks = (file_len/512) + 1;

    
    char block_num[3];
    char* data_pack = NULL;
    char* data = (char*)malloc(512);
    for(int i=0;i<num_blocks;i++){
        data = realloc(data,512);
        bzero(data,512);
        fread(data,1,512,fp_send);
        get_block_num(i+1,block_num);
        int data_len = construct_data_packet(&data_pack,block_num,data);
        int n = sendto(sock,data_pack,data_len,0,(struct sockaddr*)reciever,length);
        free(data_pack);
        bzero(buf,BUF_SIZE);
        n = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr *)sender,&length);
        if (n < 0)
        {
            printf("recvfrom Error\n");
            exit(1);
        }
        struct ack_packet ack = decode_ack_packet(buf);
        printf("Recieved ACK for Block number: %s\n",ack.block_number);
    }
    free(data);
    fclose(fp_send);
}

void recieveFile(int sock,struct sockaddr_in* sender,const char* file_name){
    char buf[BUF_SIZE];
    FILE* fp_rec = fopen(file_name,"a");
    if(fp_rec == NULL){
        printf("Unable to open file\n.");
    }		
    int length = sizeof(struct sockaddr_in);
    while(1){
        bzero(buf,BUF_SIZE);
        int n = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr *)sender,&length);
        if (n < 0)
        {
            printf("Error in Receiving.\n");
        }
        if(buf[1] == '5'){
            struct error_packet rcvd_error = decode_error_packet(buf);
            printf("Error Packet Received: %s\n",rcvd_error.error_msg);
            free(rcvd_error.error_msg);
            break;
        }
        else{
            struct data_packet rcvd_data = decode_data_packet(buf);
            fputs(rcvd_data.data,fp_rec);
            char* ack_pack;
            int ack_len = construct_ack_packet(&ack_pack,rcvd_data.block_number);
            n=sendto(sock,ack_pack,ack_len,0,(struct sockaddr*)sender,length);
            free(ack_pack);
            if(strlen(rcvd_data.data) < 512){
                break;
            }
        }
    }	
    fclose(fp_rec);		
}
#include "tftp.h"

void get_block_num(int block_num,char block[]){
	block[0] = (block_num/10) + '0';
	block[1] = (block_num%10) + '0';
	block[2] = '\0';
}

void sendFile(int sock,struct sockaddr_in* sender,struct sockaddr_in* reciever,const char* file_name){
    FILE* fp = fopen(file_name,"r");
    char buf[BUF_SIZE];
    int length = sizeof(struct sockaddr_in);
    fseek(fp,0L,SEEK_END);
    int file_len = ftell(fp);
    fseek(fp,0L,SEEK_SET);

    int num_blocks = (file_len/512) + 1;

    for(int i=0;i<num_blocks;i++){
        char* data_pack;
        char data[512];
        bzero(data,512);
        fread(data,512,1,fp);
        fseek(fp,0L,SEEK_CUR);
        char block_num[3];
        get_block_num(i+1,block_num);
        int data_len = construct_data_packet(&data_pack,block_num,data);
        int n = sendto(sock,data_pack,data_len,0,reciever,length);
        free(data_pack);
        bzero(buf,BUF_SIZE);
        n = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr *)sender,&length);
        if (n < 0)
        {
            error("recvfrom Error");
        }
        struct ack_packet ack = decode_ack_packet(buf);
        printf("Recieved ACK for Block number: %s\n",ack.block_number);
    }
    fclose(fp);
}

void recieveFile(int sock,struct sockaddr_in* sender,const char* file_name){
    char buf[BUF_SIZE];
    FILE* fp1 = fopen(file_name,"a");
    if(fp1 == NULL){
        printf("Unable to open file\n.");
    }		
    int length = sizeof(struct sockaddr_in);
    while(1){
        bzero(buf,BUF_SIZE);
        int n = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr *)sender,&length);
        if (n < 0)
        {
            printf("Error in Recieving.\n");
        }
        struct data_packet rcvd_data = decode_data_packet(buf);
        fputs(rcvd_data.data,fp1);
        char* ack_pack;
        int ack_len = construct_ack_packet(&ack_pack,rcvd_data.block_number);
        n=sendto(sock,ack_pack,ack_len,0,sender,length);
        if(strlen(rcvd_data.data) < 512)
            break;
    }	
    fclose(fp1);		
}
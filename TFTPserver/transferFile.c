/*

This file contains functions sendFile() and receiveFile() which are used by client and server for file tranfer purposes.

==> SendFile(): For each block of data perform following steps--
    1. For each block of data, read data into buffer from file.
    2. Then Send the data packet over the network
    3. Receive ACK for the sent data packet. 

==> receiveFile(): Perform below steps till data is being received
    1. Receive the incoming packet
    2. Check whether it is error packet or not.
       If yes then notify client about the error and exit
    3. If received packet is data packet, then:
          -- Decode the packet to get data in it
          -- Write the data into file 
          -- Send ACK for received block 
    4. Terminate the connection if length of data is less than 512 i.e. it is last data
       packet of the file.         
 
*/

#include "tftp.h"

void get_block_num(int block_num,char block[]){
	block[0] = (block_num/10) + '0';
	block[1] = (block_num%10) + '0';
	block[2] = '\0';
}

void sendFile(int sock,struct sockaddr_in* sender,struct sockaddr_in* reciever,const char* file_name)
{
    
    //Opening the file pointer to read data from
    FILE* fp_send = fopen(file_name,"r");
    if(fp_send == NULL)
    {
        printf("Unable to open file\n.");
        exit(1);
    }

    //Declaring required arrays and variables
    char buf[BUF_SIZE];
    char block_num[3];
    char* data_pack = NULL;                   
    char* data = (char*)malloc(512);          //Declaring data array dynamically to read data from file
    int length = sizeof(struct sockaddr_in);


    //Calculating the size of File in bytes
    fseek(fp_send,0L,SEEK_END);
    int file_len = ftell(fp_send);
    fseek(fp_send,0L,SEEK_SET);

    //Calculating Number of blocks of data in File
    int num_blocks = (file_len/512) + 1;

    //Performing below steps for each block of data in file...
    for(int i=0;i<num_blocks;i++)
    {

        //Reallocating memory to data array in every loop to avoid memory leak.
        data = realloc(data,512);                         

        //Reading 512 bytes of data from file into data array
        bzero(data,512);
        fread(data,1,512,fp_send);

        //Constructing data packet to be sent over the network
        get_block_num(i+1,block_num);
        int data_len = construct_data_packet(&data_pack,block_num,data);

        //Sending data packet
        int n = sendto(sock,data_pack,data_len,0,(struct sockaddr*)reciever,length);
        free(data_pack);

        //Receiving Acknowledgement for the data packet just sent.
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



void recieveFile(int sock,struct sockaddr_in* sender,const char* file_name)
{
    char buf[BUF_SIZE];

    //Opening the file pointer to write received data into file
    FILE* fp_rec = fopen(file_name,"a");
    if(fp_rec == NULL){
        printf("Unable to open file\n.");
    }		

    int length = sizeof(struct sockaddr_in);


    //This loop runs until termination i.e. either file transfer completes or an error packet is received
    while(1)
    {

        //Receiving incoming data...
        bzero(buf,BUF_SIZE);
        int n = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr *)sender,&length);
        if (n < 0)
        {
            printf("Error in Receiving.\n");
        }

        //If the incoming packet was error packet ==> notify client about it and exit.
        if(buf[1] == '5')
        {
            struct error_packet rcvd_error = decode_error_packet(buf);
            printf("Error Packet Received: %s\n",rcvd_error.error_msg);
            free(rcvd_error.error_msg);
            break;
        }

        //Perform below steps if received packt is data packet
        else
        {
            //Decoding the data packet to get the data inside it.
            struct data_packet rcvd_data = decode_data_packet(buf);

            //Writing the received data to file
            fputs(rcvd_data.data,fp_rec);

            //Sending ACK packet for received block of data.
            char* ack_pack;
            int ack_len = construct_ack_packet(&ack_pack,rcvd_data.block_number);
            printf("Sending ACK for Block Number: %s\n", rcvd_data.block_number);
            n=sendto(sock,ack_pack,ack_len,0,(struct sockaddr*)sender,length);
            free(ack_pack);

            //If the length of data is less than 512 bytes ==> it is last packet of the file 
            //So terminate the connection.....
            if(strlen(rcvd_data.data) < 512){
                break;
            }
        }
    }	
    fclose(fp_rec);		
}
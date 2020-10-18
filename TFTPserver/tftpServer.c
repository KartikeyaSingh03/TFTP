/*

This file contains implementation of TFTP Server according to RFC 1350.

	==> Establishing UDP Socket Connection

	==> Receive the first incoming packet and demux according to opcode of received packet

	==> Serving READ Request from Client
	    1. Decode Read Request Packet to get filename
	    2. Check for existence of file on server
	       -- If file does not exist on server, send corresponding error packet to client
	       -- Else continue
	    3. Check for Reading permission of file
	       -- If the specified file does not have read permission, send corresponding error packet to client
	       -- Else continue
	    4. Send File to client by calling sendFile() function       


	==> Serving WRITE Request from Client
		1. Decode Write Request Packet to get filename
		2. Check for existence of file on server
	       -- If file of that name already exist on server, send corresponding error packet to client
	       -- Else continue 
	    3. Send ACK packet to acknowledge write request 
	    4. Receive the incoming file from client   
	
*/



#include "tftp.h"

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

int main(int argc,char* argv[])
{
	//Check for correct number of command line arguments.
    if (argc < 2)
	{
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	char buf[BUF_SIZE];


	//Establishing UDP socket for communication on the server with specified port number

	int sock;
	struct sockaddr_in server,client;
    
    //Opening the socket
	sock=socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		printf("Error while opening socket\n");
		exit(1);
	}

	int length = sizeof(server);
	bzero(&server,length);

	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(atoi(argv[1]));

	//Binding the socket with IP Address of server
	if (bind(sock,(struct sockaddr *)&server,length)<0)
	{
		printf("Binding Error.\n");
		exit(1);
	}

	//Connection Establishment done....


	//Server is Ready for receiving requests......


	//Server is always running and ready to receive incoming requests
    while(1){

    	//Receive first incoming packet from client
        bzero(buf,BUF_SIZE);
		int n = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr *)&client,&length);
		if (n < 0)
		{
			printf("recvfrom Error");
			exit(1);
		}

		//Read Opcode of received packet
        char opcode[3];
        opcode[0] = buf[0];
        opcode[1] = buf[1];
        opcode[2] ='\0';


        //Demultiplex according to opcode of received packet

        //Perform below steps if it is Read Request
        if(strcmp(opcode,"01") == 0)
        {
        	//Decode read request packet
			struct read_req_packet read_pack = decode_read_packet(buf);
			printf("Recieved RRQ for file: %s\n",read_pack.file_name);

			//Check whether file to be read is present on server or not
			if(access(read_pack.file_name,F_OK) == -1)
			{
				//Construct and Send required error packet
				char* err_pack;
				int err_len = construct_err_packet(&err_pack,"01",error_codes[1]);
				printf("Error Encountered : Sending Error Packet \n\n");
				n=sendto(sock,err_pack,err_len,0,(struct sockaddr*)&client,length);
				free(err_pack);
			}

			//Check whether file to be read has reading permission or not
			else if(access(read_pack.file_name,R_OK) == -1)
			{
				//Construct and Send required error packet
				char* err_pack;
				int err_len = construct_err_packet(&err_pack,"02",error_codes[2]);
				printf("Error Encountered : Sending Error Packet \n\n");
				n=sendto(sock,err_pack,err_len,0,(struct sockaddr*)&client,length);
				free(err_pack);					
			}

			//If there are no error scenarios then send the File to client
			else
			{
				sendFile(sock,&server,&client,read_pack.file_name);
				printf("Read Request Served for file : %s \n\n", read_pack.file_name);
			}
			free(read_pack.file_name);
			free(read_pack.mode);
			
        }


        //Perform below steps if it is Write Request
        else if(strcmp(opcode,"02") == 0)
        {
        	//Decode write request packet
            struct write_req_packet write_pack = decode_write_packet(buf);
	        printf("Recieved WRQ for file: %s\n",write_pack.file_name);


	        //Check whether file to be read is present on server or not
			if(access(write_pack.file_name,F_OK) != -1)
			{
				//Construct and Send error packet to client if file by that name is already present on server
				char* err_pack;
				int err_len = construct_err_packet(&err_pack,"06",error_codes[6]);
				printf("Error Encountered : Sending Error Packet.... \n\n");
				n=sendto(sock,err_pack,err_len,0,(struct sockaddr*)&client,length);
				free(err_pack);
			}

			//In case of no error follow below steps
			else
			{
				bzero(buf,BUF_SIZE);

				//Construct and send ACK packet to acknowledge write request
				char* ack_pack;
				int ack_len = construct_ack_packet(&ack_pack,"00");
				n=sendto(sock,ack_pack,ack_len,0,(struct sockaddr*)&client,length);
				free(ack_pack);

				//Appending the filename of receiving file by "PUT_" to indicate that this file is put by client on server...
				char file_name[100];
				strcpy(file_name,"PUT_");
				strcat(file_name,write_pack.file_name);

				//Receive File from server
				recieveFile(sock,&server,file_name);

				printf("Write Request Served for file : %s \n\n", write_pack.file_name);
			}
			free(write_pack.file_name);
			free(write_pack.mode);

			
        }

        //Illegal TFTP Operation because initially RRQ or WRQ must be sent to initiate connection
        else
        {
            //printf("%s\n","RRQ/WRQ must be sent before data transfer.\n");

            //Construct and Send required error packet
			char* err_pack;
			int err_len = construct_err_packet(&err_pack,"04",error_codes[4]);
			printf("Error Encountered : Sending Error Packet..... \n\n");
			n=sendto(sock,err_pack,err_len,0,(struct sockaddr*)&client,length);
			free(err_pack);
        }
    }

	return 0;  
}
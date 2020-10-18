/*

This file contains implementation of TFTP Cient according to RFC 1350.

	==> Establishing UDP Socket Connection

	==> PUT File on Server
	    1. Construct and Send Write Request to server
	    2. Receive incoming packet from server
	    3. If received packet is error packet, then notify client of the error and exit
	    4. If received packet is an ACK packet, then Send the file to server

	==> GET File from server
		1. Construct and Send Read Request to server
		2. Receive File from Server
		3. NOTE: Error Handling part is implemented in receiveFile() function   
	

NOTE: 

	1. In case of WRQ request sent to server ==> Server acknowledges with an ACK packet
	2. In case of RRQ request sent to server ==> Server directly sends DATA packet without acknowledgement

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
	if(argc != 5){
		printf("Incorrect Format\nCorrect Format: ./client <ip_addr> <port_no> [-g/-p] <file_name> \n");
		exit(1);
	}
	char buf[BUF_SIZE];
	int length=sizeof(struct sockaddr_in);

	//Establishing UDP socket for communication

	int sock;
	struct sockaddr_in server, client;
	struct hostent *hp;
	struct in_addr addr;

	//Opening the UDP socket 
	sock= socket(AF_INET, SOCK_DGRAM, 0);
	if(sock<0)
	{
		printf("Error while opening socket\n");
		exit(1);
	}


	server.sin_family = AF_INET;

	//Converting server IP address from string to IP address format and getting the host
	inet_aton(argv[1],&addr);           
	hp=gethostbyaddr(&addr,sizeof(addr),AF_INET);
	if(hp==0)
	{
		printf("Unknown Host\n");
		exit(1);
	}
	bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
	server.sin_port = htons(atoi(argv[2]));
	
	//Connection Establishment Complete......



	//Ready for file transfer....

	//Perform below steps to GET file from Server....
	if(strcmp("-g",argv[3]) == 0)
	{	
		//Construct Read Packet
		char* read_pack;
		int read_len = construct_read_packet(&read_pack,argv[4],MODE_NETASCII);

		//Send Read Request to Server
		int n = sendto(sock,read_pack,read_len,0,(struct sockaddr*)&server,length);
		free(read_pack);
		
		//Appending the filename of receiving file by "GET_" to indicate that this file is received...
		char file_name[100];
		strcpy(file_name,"GET_");
		strcat(file_name,argv[4]);

		//Receive File from Server
		recieveFile(sock,&client,file_name);
	}


	//Perform below steps to PUT file on server....
	else if(strcmp("-p",argv[3]) == 0)
	{	
		//Check for existence of specified file on client machine
		if(access(argv[4],F_OK) == -1)
		{
			printf("Error : File does not exist on Client Machine\n");
			exit(1);
		}
		//Construct Write Packet..
		char* write_pack;
		int write_len = construct_write_packet(&write_pack,argv[4],MODE_NETASCII);

		//Send Write Request to server
		int n = sendto(sock,write_pack,write_len,0,(struct sockaddr*)&server,length);
		free(write_pack);

		//Receive incoming packet from server and demultiplex according to opcode
		bzero(buf,BUF_SIZE);
		n = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr *)&client,&length);
		if (n < 0)
		{
			printf("recvfrom Error\n");
			exit(1);
		}

		//If received packet is an ACK packet then perform below steps
		if(buf[1] == '4')
		{
			//Decode ACK Packet
			struct ack_packet ack = decode_ack_packet(buf);
			printf("Recieved ACK for Block number: %s\n",ack.block_number);

			//Send File to server
			sendFile(sock,&client,&server,argv[4]);
		}
		else
		{
			//Decode Error Packet and notify the client
			struct error_packet rcvd_error = decode_error_packet(buf);
            printf("Error Packet Received: %s\n",rcvd_error.error_msg);
            free(rcvd_error.error_msg);
		}
	}


	//Incorrect format for command line argumnents..
	else
	{
		printf("Incorrect Options. Use -g for GET and -p for PUT\n");
		exit(1);
	}

	// char* err_pack;
	// int err_len = construct_err_packet(&err_pack,"01",error_codes[1]);
	// n=sendto(sock,err_pack,err_len,0,(struct sockaddr*)&server,length);

	return 0;
}
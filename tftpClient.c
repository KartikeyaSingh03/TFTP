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

int main(int argc,char* argv[]){
	if(argc != 5){
		printf("Incorrect Format\nCorrect Format: ./client <ip_addr> <port_no> [-g/-p] <file_name> \n");
		exit(1);
	}
	char buf[BUF_SIZE];

	int sock;
	struct sockaddr_in server, client;
	struct hostent *hp;
	struct in_addr addr;


	sock= socket(AF_INET, SOCK_DGRAM, 0);
	if(sock<0)
	{
		printf("Error while opening socket\n");
		exit(1);
	}

	server.sin_family = AF_INET;
	inet_aton(argv[1],&addr);
	hp=gethostbyaddr(&addr,sizeof(addr),AF_INET);

	if(hp==0)
	{
		printf("Unknown Host\n");
		exit(1);
	}

	bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
	server.sin_port = htons(atoi(argv[2]));
	
	int length=sizeof(struct sockaddr_in);

	if(strcmp("-g",argv[3]) == 0){
		char* read_pack;
		int read_len = construct_read_packet(&read_pack,argv[4],MODE_NETASCII);
		int n = sendto(sock,read_pack,read_len,0,(struct sockaddr*)&server,length);
		free(read_pack);
		
		char file_name[100];
		strcpy(file_name,"GET_");
		strcat(file_name,argv[4]);
		recieveFile(sock,&client,file_name);
	}
	else if(strcmp("-p",argv[3]) == 0){
		char* write_pack;
		int write_len = construct_write_packet(&write_pack,argv[4],MODE_NETASCII);
		int n = sendto(sock,write_pack,write_len,0,(struct sockaddr*)&server,length);
		free(write_pack);
		bzero(buf,BUF_SIZE);
		n = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr *)&client,&length);
		if (n < 0)
		{
			printf("recvfrom Error\n");
			exit(1);
		}
		if(buf[1] == '4'){
			struct ack_packet ack = decode_ack_packet(buf);
			printf("Recieved ACK for Block number: %s\n",ack.block_number);
			sendFile(sock,&client,&server,argv[4]);
		}
		else{
			struct error_packet rcvd_error = decode_error_packet(buf);
            printf("Error Packet Received: %s\n",rcvd_error.error_msg);
            free(rcvd_error.error_msg);
		}
	}
	else{
		printf("Incorrect Options. Use -g for GET and -p for PUT\n");
		exit(1);
	}

	// char* err_pack;
	// int err_len = construct_err_packet(&err_pack,"01",error_codes[1]);
	// n=sendto(sock,err_pack,err_len,0,(struct sockaddr*)&server,length);

	return 0;
}
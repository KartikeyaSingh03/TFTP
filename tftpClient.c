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

void error(char *msg)
{
	perror(msg);
	exit(0);
}


int main(int argc,char* argv[]){
	if(argc != 5){
		printf("Incorrect Format\nCorrect Format: ./client <ip_addr> <port_no> [-g/-p] <file_name> \n");
		exit(1);
	}
	char buf[BUF_SIZE];

	int sock, length, n;
	struct sockaddr_in server, client;
	struct hostent *hp;
	struct in_addr addr;


	sock= socket(AF_INET, SOCK_DGRAM, 0);
	if(sock<0)
	{
		error("socket");
	}

	server.sin_family =AF_INET;
	inet_aton(argv[1],&addr);
	hp=gethostbyaddr(&addr,sizeof(addr),AF_INET);

	if(hp==0)
	{
		error("Unknown Host");
	}

	bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
	server.sin_port = htons(atoi(argv[2]));
	length=sizeof(struct sockaddr_in);

	if(strcmp("-g",argv[3]) == 0){
		char* read_pack;
		int read_len = construct_read_packet(&read_pack,argv[4],MODE_NETASCII);
		n = sendto(sock,read_pack,read_len,0,&server,length);
		recieveFile(sock,&client,"client_rec.txt");
	}
	else if(strcmp("-p",argv[3]) == 0){
		char* write_pack;
		int write_len = construct_write_packet(&write_pack,argv[4],MODE_NETASCII);
		n = sendto(sock,write_pack,write_len,0,&server,length);
		bzero(buf,BUF_SIZE);
		n = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr *)&client,&length);
		if (n < 0)
		{
			error("recvfrom Error");
		}
		struct ack_packet ack = decode_ack_packet(buf);
	    printf("Recieved ACK for Block number: %s\n",ack.block_number);

		sendFile(sock,&client,&server,argv[4]);
	}
	else{
		printf("Incorrect Options. Use -g for GET and -p for PUT\n");
		exit(1);
	}

	// char* read_pack;
	// int read_len = construct_read_packet(&read_pack,"12345678.rnd",MODE_NETASCII);
	// n=sendto(sock,read_pack,read_len,0,&server,length);

	// char* write_pack;
	// int write_len = construct_write_packet(&write_pack,"hello.txt",MODE_OCTET);
	// n=sendto(sock,write_pack,write_len,0,&server,length);

	// char* data_pack;
	// int data_len = construct_data_packet(&data_pack,"35","Hi I am Kartikeya Singh.");
	// n=sendto(sock,data_pack,data_len,0,&server,length);

	// char* ack_pack;
	// int ack_len = construct_ack_packet(&ack_pack,"15");
	// n=sendto(sock,ack_pack,ack_len,0,&server,length);

	// char* err_pack;
	// int err_len = construct_err_packet(&err_pack,"01",error_codes[1]);
	// n=sendto(sock,err_pack,err_len,0,&server,length);

	return 0;
}
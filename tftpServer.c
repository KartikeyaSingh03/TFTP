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
    int sock, length, n;
	struct sockaddr_in server;
	struct sockaddr_in client;
    char buf[BUF_SIZE];

	if (argc < 2)
	{
		fprintf(stderr, "ERROR, no port provided\n");
		exit(0);
	}

	sock=socket(AF_INET, SOCK_DGRAM, 0);

	if (sock < 0)
	{
		error("Opening socket");
	}

	length = sizeof(server);
	bzero(&server,length);


	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(atoi(argv[1]));


	if (bind(sock,(struct sockaddr *)&server,length)<0)
	{
		error("binding");
	}

    while(1){
        bzero(&buf,length);
		n = recvfrom(sock,buf,BUF_SIZE,0,(struct sockaddr *)&client,&length);
		if (n < 0)
		{
			error("recvfrom Error");
		}

        char opcode[3];
        opcode[0] = buf[0];
        opcode[1] = buf[1];
        opcode[2] ='\0';
        if(strcmp(opcode,"01") == 0){
			struct read_req_packet read_pack = decode_read_packet(buf);
			printf("Recieved RRQ for file: %s\n",read_pack.file_name);
			sendFile(sock,&server,&client,read_pack.file_name);
        }
        else if(strcmp(opcode,"02") == 0){
            struct write_req_packet write_pack = decode_write_packet(buf);
	        printf("Recieved WRQ for file: %s\n",write_pack.file_name);
			bzero(buf,BUF_SIZE);
			char* ack_pack;
			int ack_len = construct_ack_packet(&ack_pack,"00");
			n=sendto(sock,ack_pack,ack_len,0,&client,length);
			recieveFile(sock,&server,"received.txt");
        }
        else{
            printf("%s\n","RRQ/WRQ must be sent before data transfer.\n");
        }
    } 
}
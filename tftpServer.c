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
    int sock, length, fromlen, n;
	struct sockaddr_in server;
	struct sockaddr_in from;
    char buf[1024];

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


	fromlen = sizeof(struct sockaddr_in);   

    while(1){
        bzero(&buf,length);
		n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
		if (n < 0)
		{
			error("recvfrom Error");
		}

    	//write(1,buf,n);
        char opcode[3];
        opcode[0] = buf[0];
        opcode[1] = buf[1];
        opcode[2] ='\0';
        if(strcmp(opcode,"01") == 0){
            struct read_req_packet data = decode_read_packet(buf);
	        printf("%s\n%s\n%s\n\n",data.opcode,data.file_name,data.mode);
        }
        else if(strcmp(opcode,"02") == 0){
            struct write_req_packet write_pack = decode_write_packet(buf);
	        printf("%s\n%s\n%s\n\n",write_pack.opcode,write_pack.file_name,write_pack.mode);
        }
        else if(strcmp(opcode,"03") == 0){
            struct data_packet data_data = decode_data_packet(buf);
	        printf("%s\n%s\n%s\n\n",data_data.opcode,data_data.block_number,data_data.data);
        }
        else if(strcmp(opcode,"04") == 0){
            struct ack_packet ack = decode_ack_packet(buf);
	        printf("%s\n%s\n\n",ack.opcode,ack.block_number);
        }
        else if(strcmp(opcode,"05") == 0){
            struct error_packet err = decode_error_packet(buf);
	        printf("%s\n%s\n%s\n\n",err.opcode,err.error_code,err.error_msg);
        }
        else{
            printf("%s\n","Error!!!");
        }

        

    } 
}
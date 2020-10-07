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
  
    char* pack = construct_read_packet("12345678.rnd",MODE_NETASCII);
    struct read_req_packet data = decode_read_packet(pack);
    printf("%s\n%s\n%s\n\n",data.opcode,data.file_name,data.mode);

    char* writePack = construct_write_packet("hello.txt",MODE_OCTET);
	struct write_req_packet write_pack = decode_write_packet(writePack);
	printf("%s\n%s\n%s\n\n",write_pack.opcode,write_pack.file_name,write_pack.mode);  

	char* packData = construct_data_packet("35","Hi I am Kartikeya Singh.");
    struct data_packet data_data = decode_data_packet(packData);
    printf("%s\n%s\n%s\n\n",data_data.opcode,data_data.block_number,data_data.data);

	char* ackPack = construct_ack_packet("15");
	struct ack_packet ack = decode_ack_packet(ackPack);
	printf("%s\n%s\n\n",ack.opcode,ack.block_number);

	char* errPack = construct_err_packet("01",error_codes[1]);
	struct error_packet err = decode_error_packet(errPack);
	printf("%s\n%s\n%s\n\n",err.opcode,err.error_code,err.error_msg);

    return 0;
}
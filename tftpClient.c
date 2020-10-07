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
    // char* pack = construct_data_packet("35","Hello World!");
    // printf("%d\n",(int)sizeof(pack));
    char* pack = construct_read_packet("jik.txt",MODE_MAIL);
    // 01jik.txt\0mail\0
    for(int i=0;i<16;++i){
        if(pack[i] == '\0')
            printf("%d\n",i);
    }
}
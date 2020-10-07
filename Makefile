CXX = gcc

common = construct_packets.c
flags =

default: all

all: client

##server:
##	${CXX} ${flags} -o tftpserver ${common} tftpserver.c 

client: 
	${CXX} ${flags} -o client ${common} tftpClient.c

clean:
	rm *.o client
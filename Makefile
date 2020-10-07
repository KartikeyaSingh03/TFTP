CXX = gcc

common = constructPackets.c decodePackets.c
flags = -g

default: all

all: client

##server:
##	${CXX} ${flags} -o tftpserver ${common} tftpserver.c 

client: 
	${CXX} ${flags} -o client ${common} tftpClient.c

clean:
	rm *.o client
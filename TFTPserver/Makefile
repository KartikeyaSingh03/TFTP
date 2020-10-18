CXX = gcc

common = constructPackets.c decodePackets.c transferFile.c
flags = -g

default: all

all: server 

server:
	${CXX} ${flags} -o server ${common} tftpServer.c 

##client: 
##	${CXX} ${flags} -o client ${common} tftpClient.c

clean:
	rm *.o server

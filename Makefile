CXX = gcc

common = constructPackets.c decodePackets.c
flags = -g

default: all

all: server client 

server:
	${CXX} ${flags} -o server ${common} tftpServer.c 

client: 
	${CXX} ${flags} -o client ${common} tftpClient.c

clean:
	rm *.o client server
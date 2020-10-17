# TFTP CLIENT AND SERVER

To Run the project follow below given steps:

1. Run this command : make clean
2. Run this command : make
3. To start the server run this command in terminal : ./server <Port-number>
4. To start the client open a separate terminal and run : ./client 127.0.0.1 <Port-Number> [-g/-p] <File-name>

NOTE : Use port number greater than 1024 because port number smaller than 1024 are fixed for special application services.



This Project consists of 6 different files, the description of which are as follows:

1. tftp.h ==> This is the header file which includes all the library files included alongwith the prototypes of various 
   functions used for different purposes. It also includes declaration of struct for storing information in different 
   packets after decoding them.	

2. tftpClient.c ==> This c file contains implementation of TFTP Client according to RFC 1350. The details of working of 
   Client are explained inside the file.

3. tftpServer.c ==> This c file contains implementation of TFTP Srver according to RFC 1350. The details of working of 
   Server are explained inside the file.

4. transferFile.c ==> This c file contains two functions viz. sendFile() and receiveFile() which are used by client and server 
   to transfer files among themselves. The details of working of respective functions can be found inside the file itelf.

5. constructPackets.c ==> This c file contains 5 different functions which are used for constructing 5 differnt types of TFTP
   packets. The details of implementation can be found inside the file.   

6. decodePackets.c ==> This c file contains 5 different functions which are used for decoding 5 differnt types of TFTP
   packets to get the information contained in them. The details of implementation can be found inside the file.    

# TFTP CLIENT AND SERVER

INDIAN INSTITUTE OF TECHNOLOGY - GUWAHATI

Group Number : 80
Member 1 : Mohammad Humam Khan (180123057)
Member 2 : Kartikeya Singh (180123021)



To Run the TFTP Server follow below given steps:

1. Navigate to TFTPserver folder
2. Run this command : make clean
3. Run this command : make
4. To start the server run this command in terminal : ./server [Port-number]


To Run the TFTP Client follow below given steps:

1. Navigate to TFTPclient folder
2. Run this command : make clean
3. Run this command : make
4. To start the client open a separate terminal and run : ./client [IP-Address] [Port-Number] [-g/-p] [File-name]

NOTE : Use port number greater than 1024 because port number smaller than 1024 are fixed for special application services.
NOTE : Since Server is run on local machine use 127.0.0.1 as IP-Address


NOTE : For testing purpose we have provided a testfile.txt on client side and rfc.txt on server side.

NOTE : To test the implementation, following commands can be used ==>

1. START SERVER : ./server 2000
2. GET FILE FROM SERVER :    ./client 127.0.0.1 2000 -g rfc.txt
3. PUT FILE ON SERVER :      ./client 127.0.0.1 2000 -p testfile.txt
4. ERROR (FILE NOT FOUND) :  ./client 127.0.0.1 2000 -g random.txt
5. ERROR (FILE TO BE PUT NOT EXIST ON CLIENT) : ./client 127.0.0.1 2000 -p random.txt


This Project consists of 6 different files organized in two different folders viz. TFTPserver and TFTPclient,
the description of which are as follows:

1. tftp.h ==> This is the header file which includes all the library files included alongwith the prototypes of various 
   functions used for different purposes. It also includes declaration of struct for storing information in different 
   packets after decoding them.	

2. tftpClient.c ==> This c file contains implementation of TFTP Client according to RFC 1350. The details of working of 
   Client are explained inside the file.

3. tftpServer.c ==> This c file contains implementation of TFTP Server according to RFC 1350. The details of working of 
   Server are explained inside the file.

4. transferFile.c ==> This c file contains two functions viz. sendFile() and receiveFile() which are used by client and server 
   to transfer files among themselves. The details of working of respective functions can be found inside the file itelf.

5. constructPackets.c ==> This c file contains 5 different functions which are used for constructing 5 differnt types of TFTP
   packets. The details of implementation can be found inside the file.

6. decodePackets.c ==> This c file contains 5 different functions which are used for decoding 5 differnt types of TFTP
   packets to get the information contained in them. The details of implementation can be found inside the file.

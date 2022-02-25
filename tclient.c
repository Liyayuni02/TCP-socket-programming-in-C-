#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORT 3490 // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once
int main(int argc, char *argv[])
{
	int sockfd;
	struct hostent *he;
	struct sockaddr_in their_addr; // connector's address information
if (argc != 3) {
	fprintf(stderr,"Penggunaan: %s server pesan\n", argv[0]);
	exit(1);
}
if ((he=gethostbyname(argv[1])) == NULL) { // get the host info
	perror("gethostbyname");
	exit(1);
}
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { // socket create and varification
	perror("socket");
	exit(1);
}
their_addr.sin_family = AF_INET; // host byte order
their_addr.sin_port = htons(PORT); // short, network byte order
their_addr.sin_addr = *((struct in_addr *)he->h_addr);
memset(&(their_addr.sin_zero), '\0', 8);// zero the rest of the struct
if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) { // connect the client socket to server socket
	perror("connect");
	exit(1);
}
if ((send(sockfd, argv[2], strlen(argv[2]), 0)) == -1) {
	perror("send");
	exit(0);
}
printf("mengirimkan %s ke %s\n", argv[2], argv[1]);
close(sockfd); //close the socket
return 0;
}

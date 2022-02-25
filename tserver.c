#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#define MYPORT 3490 // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define MAXDATA 100000

void sigchld_handler(int s)
{
	while(wait(NULL) > 0);
}
int main(void)
{
	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr; // my address information
	struct sockaddr_in their_addr; // connector's address information
	int sin_size, numbytes;
	char buf[MAXDATA];
	struct sigaction sa;
	int yes=1;
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 1) // socket create and verification
{
	perror("socket");
	exit(1);
}
if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))== 1)
{
perror("setsockopt");
exit(1);
}

my_addr.sin_family = AF_INET; // host byte order
my_addr.sin_port = htons(MYPORT); // short, network byte order
my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with myIP
memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct

if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == 1) // Binding newly created socket to given IP and verification
{
	perror("bind");
	exit(1);
}
if (listen(sockfd, BACKLOG) == 1) // Now server is ready to listen and verification
{
	perror("listen");
	exit(1);
}
sa.sa_handler = sigchld_handler; // reap all dead processes
sigemptyset(&sa.sa_mask);
sa.sa_flags = SA_RESTART;
if (sigaction(SIGCHLD, &sa, NULL) == 1)
{
	perror("sigaction");
	exit(1);
}
printf("SERVER: siap menerima koneksi\n");
while(1) { // main accept() loop
sin_size = sizeof(struct sockaddr_in);
if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr,&sin_size)) == 1)
{
	perror("accept");
	continue;
}
printf("SERVER: menerima koneksi dari%s\n",inet_ntoa(their_addr.sin_addr));
if (!fork()) { // this is the child process
close(sockfd); // child doesn't need the listener
numbytes=recv(new_fd, buf, MAXDATA,0);
if (numbytes < 0) {
	perror("recv");
	exit(1);
}
printf("Data: %s\n", buf);
buf[numbytes]= '\0';
close(new_fd);
exit(0);
}
close(new_fd);
}
return 0;
}


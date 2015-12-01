#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/un.h>
int main(){
	struct sockaddr_un address;
	int sockfd;
	int len;
	int result;
	char ch='A';
	//create a socket
	sockfd=socket(AF_UNIX,SOCK_STREAM,0);
	//Name the socket as agreed with the server
	address.sun_family=AF_UNIX;
	strcpy(address.sun_path,"server_socket");
	len=sizeof(address);
	// connecting socket to a server
	result=connect(sockfd, (struct sockadd *)&address,len);
	if(result== -1){
		perror("oops client");
	}
	// read and write via socket
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("char from server = %c\n", ch);
	close(sockfd);
	exit(0);
}

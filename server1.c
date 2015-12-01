#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/un.h>
int main(){
	int server_sockfd,client_sockfd;
	int server_len,client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	//remove old socket and create a new unnamed socket for the server
	unlink("server_socket");
	server_sockfd=socket(AF_UNIX,SOCK_STREAM,0);
	//Name the socket
	server_address.sun_family =AF_UNIX;
	strcpy(server_address.sun_path,"server_socket");
	server_len=sizeof(server_address);
	bind(server_sockfd,(struct sockadd *)&server_address,server_len);
	//create a connection queue and wait for the client
	listen(server_sockfd,5);
	while(1){
		char ch;
		printf(" server waiting \n");
		//Accept a connection
		client_len=sizeof(client_address);
		client_sockfd=accept(server_sockfd,(struct sockadd *)&client_address,&client_len);
		//Read and write to client
		read(client_sockfd,&ch,1);
		ch++;
		write(client_sockfd,&ch,1);
		close(client_sockfd);
	}
}

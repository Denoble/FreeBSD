#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/capability.h>
#include<err.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>

int main(){
	int server_sockfd,client_sockfd;
	int server_len,client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	int a,b,c,d;
	char readWords[128];
	char* status="server waiting for a connection ";
	char ty[20],by[20],cy[20];
	char* newline="\n";
	unlink("server_socket");
	server_sockfd=socket(AF_UNIX,SOCK_STREAM,0);
	server_address.sun_family=AF_UNIX;
	strcpy(server_address.sun_path,"server_socket");
	server_len=sizeof(server_address);
	bind(server_sockfd,(struct sockaddr*) &server_address,server_len);
	listen(server_sockfd,5);
	while(1){
		write(STDOUT_FILENO,status,strlen(status));
		write(STDOUT_FILENO,newline,strlen(newline));	
		client_len=sizeof(client_address);
		client_sockfd=accept(server_sockfd,NULL,NULL);
		a=open("Reading.txt",O_RDONLY);
		b=open("Written.txt",O_RDONLY|O_APPEND|O_CREAT);
		if(a<0)
			err(-1,"fail to open file Reading.txt");
		b=open("Written.txt",O_WRONLY|O_CREAT|O_APPEND);
		cap_enter();
		sprintf(ty,"%d",b);
		write(STDOUT_FILENO,ty,strlen(ty));
		if(a>0 && b>0){
			write(client_sockfd,ty,strlen(ty));
			
		}
		while(read(client_sockfd,by,sizeof(by)<0)){
			sleep(30);
		}
		while((read(a,readWords,sizeof(readWords)))>0){
			write(client_sockfd,readWords,strlen(readWords));
				
			
		}
			
		while(read(client_sockfd,cy,sizeof(cy))<0){sleep(10);}


	}	
	
	
	close(client_sockfd);
	close(a);
	close(b);
	return 0;
}
	

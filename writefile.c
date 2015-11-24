#include<sys/types.h>
#include<sys/socket.h>

#include<stdio.h>
#include<unistd.h>
#include<ctype.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<err.h>
#include<sys/un.h>
#include<sys/capability.h>

int main(){
	int sockfd;
	int len;
	int fd,j,i;
	char* done="Am done";
	char ty[20];
	struct sockaddr_un address;
	int result;
	char uppercase[128];
	char readWords[128];
	char* newline="\n";
	sockfd= socket(AF_UNIX,SOCK_STREAM,0);
	address.sun_family=AF_UNIX;
	strcpy(address.sun_path,"server_socket");
	len=sizeof(address);
	result=connect(sockfd,(struct sockaddr*) &address,len);
	if(result== -1){
		perror("OOPS: Client cannot connect \n");
		exit(1);
	}
	while((j=read(sockfd,ty,sizeof(ty)))<0){
		sleep(100);
	}
	fd=atoi(ty);
	if(fd>0){
		while(read(sockfd,readWords,sizeof(readWords))>0){
			for(i=0;i<strlen(readWords)+1;i++){
				uppercase[i]=toupper(readWords[i]);
			}
			write(fd,uppercase,strlen(uppercase));
			write(fd,newline,strlen(newline));
		}

		write(sockfd,done,strlen(done));
		
	}
	close(sockfd);
	return 0;
   }



	



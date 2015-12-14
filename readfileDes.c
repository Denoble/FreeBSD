#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/capability.h>
#include<err.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include"fileDes.h"


static int
send_file_descriptor(
  int socket, /* Socket through which the file descriptor is passed */
  int fd_to_send) /* File descriptor to be passed, could be another socket */
{
 struct msghdr message;
 struct iovec iov[1];
 struct cmsghdr *control_message = NULL;
 char ctrl_buf[CMSG_SPACE(sizeof(int))];
 char data[1];

 memset(&message, 0, sizeof(struct msghdr));
 memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

 /* We are passing at least one byte of data so that recvmsg() will not return 0 */
 data[0] = ' ';
 iov[0].iov_base = data;
 iov[0].iov_len = sizeof(data);

 message.msg_name = NULL;
 message.msg_namelen = 0;
 message.msg_iov = iov;
 message.msg_iovlen = 1;
 message.msg_controllen =  CMSG_SPACE(sizeof(int));
 message.msg_control = ctrl_buf;

 control_message = CMSG_FIRSTHDR(&message);
 control_message->cmsg_level = SOL_SOCKET;
 control_message->cmsg_type = SCM_RIGHTS;
 control_message->cmsg_len = CMSG_LEN(sizeof(int));

 *((int *) CMSG_DATA(control_message)) = fd_to_send;

 return sendmsg(socket, &message, 0);
}




int main(){
	int server_sockfd,client_sockfd;
	int server_len,client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	int a,b,c,d,i,n;
	char data_recieved[30];

	char* status="server waiting for a connection ";
	cap_rights_t rights,wrights;
	char* newline="\n";
	SocketMessage *cMsg= (SocketMessage*)malloc(sizeof(SocketMessage));
	unlink("server_socket");
	server_sockfd=socket(AF_UNIX,SOCK_STREAM,0);
	server_address.sun_family=AF_UNIX;
	strcpy(server_address.sun_path,"server_socket");
	
	while(1){
		server_len=sizeof(server_address);
		bind(server_sockfd,(struct sockaddr*) &server_address,server_len);
		listen(server_sockfd,5);
		write(STDOUT_FILENO,status,strlen(status));
		write(STDOUT_FILENO,newline,strlen(newline));	
		client_len=sizeof(client_address);
		client_sockfd=accept(server_sockfd,NULL,NULL);
		if(client_sockfd<0)
			printf("cannot connect to server \n");
		i=read(client_sockfd,cMsg->rdFileName,sizeof(cMsg->rdFileName));
	
		if(i>0){
			
		  printf(" File name %s recieved from client\n",cMsg->rdFileName);	
		  cMsg->fileDesRd=open(cMsg->rdFileName,O_RDONLY);			

			if(cMsg->fileDesRd<0){
				err(-1,"fail to open file ");
				printf("%s \n",cMsg->rdFileName);
				exit(0);
			}	
		}
		
				
		else
			printf("cannot read read-file from socket \n");
		i=read(client_sockfd,cMsg->rdRights,sizeof(cMsg->rdRights));
		if(i>0){
			
			cap_rights_init(&rights,CAP_FSTAT,cMsg->rdRights);
		
		       if(cap_rights_limit(cMsg->fileDesRd,&rights)<0)
			err(1,"cap_rights_limit() for reading failed");
		
	
			
			
		}
		
				
		else
			printf("cannot read read-file Rights from socket \n");
		
		
		
		
		
		c= send_file_descriptor(client_sockfd, cMsg->fileDesRd);
		if(c<0)
			err(-1,"cannot send read-file file descriptor ");
		i=read(client_sockfd,cMsg->wrFileName,sizeof(cMsg->wrFileName));
	
		if(i>0){
			
		    printf(" File name %s recieved from client\n",cMsg->wrFileName);	
		    cMsg->fileDesWr=open(cMsg->wrFileName,O_WRONLY|O_APPEND|O_CREAT);
		    if(cMsg->fileDesWr<0){
			err(-1,"fail to open file ");
			printf("%s \n",cMsg->wrFileName);
			exit(0);
		   }
			
		}
		
				
		else
			printf("cannot read write file from socket \n");
		
		
		
		i=read(client_sockfd,cMsg->wrRights,sizeof(cMsg->wrRights));
		if(i>0){
			
			
			
		   cap_rights_init(&wrights,CAP_FSTAT,cMsg->wrRights);
		   if(cap_rights_limit(cMsg->fileDesWr,&wrights)<0 )
			err(1,"cap_rights_limit() for writing failed");
		
					
			
		}
		
				
		else
			printf("cannot read  write file Rights from socket \n");
		
		
		
		c= send_file_descriptor(client_sockfd, cMsg->fileDesWr);
		if(c<0)
			err(-1,"cannot send write-file file descriptor \n ");
		
	   
	}	
	
		
	
	close(client_sockfd);
	free(cMsg);
	return 0;
}

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

static int
recv_file_descriptor(
  int socket) /* Socket from which the file descriptor is read */
{
 int sent_fd;
 struct msghdr message;
 struct iovec iov[1];
 struct cmsghdr *control_message = NULL;
 char ctrl_buf[CMSG_SPACE(sizeof(int))];
 char data[1];
 int res;

 memset(&message, 0, sizeof(struct msghdr));
 memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

 /* For the dummy data */
 iov[0].iov_base = data;
 iov[0].iov_len = sizeof(data);

 message.msg_name = NULL;
 message.msg_namelen = 0;
 message.msg_control = ctrl_buf;
 message.msg_controllen = CMSG_SPACE(sizeof(int));
 message.msg_iov = iov;
 message.msg_iovlen = 1;

 if((res = recvmsg(socket, &message, 0)) <= 0)
  return res;

 /* Iterate through header to find if there is a file descriptor */
 for(control_message = CMSG_FIRSTHDR(&message);
     control_message != NULL;
     control_message = CMSG_NXTHDR(&message,
                                   control_message))
 {
  if( (control_message->cmsg_level == SOL_SOCKET) &&
      (control_message->cmsg_type == SCM_RIGHTS) )
  {
   return *((int *) CMSG_DATA(control_message));
  }
 }

 return -1;
}



int main(){
	int server_sockfd,client_sockfd;
	int server_len,client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	int a,b,c,d,i;
	char readWords[128];
	char* status="server waiting for a connection ";
	char uppercase[128];
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
		a=recv_file_descriptor(client_sockfd);
	
		if(a==-1)
			err(-1,"can't read from socket");
		 b=recv_file_descriptor(client_sockfd);
			if(b==-1)
			err(-1,"can't read from socket");
		//cap_enter();
		while(read(a,readWords,sizeof(readWords))>0){
			for(i=0;i<strlen(readWords)+1;i++){
				uppercase[i]=toupper(readWords[i]);
			}
			write(b,uppercase,strlen(uppercase));
			write(b,newline,strlen(newline));
		}

		
	}	
	
	close(a);
	close(b);
	close(client_sockfd);
	
	return 0;
}
	

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

 int main(int argc,char* argv[]){

	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	int a,b,c,d;
	a=open("Reading.txt",O_RDONLY);
	if(a<0)
			err(-1,"fail to open file Reading.txt \n");
	b=open("Written.txt",O_WRONLY|O_APPEND|O_CREAT);
	if(b<0)
			err(-1,"fail to open file Writing.txt");
	sockfd= socket(AF_UNIX,SOCK_STREAM,0);
	address.sun_family=AF_UNIX;
	strcpy(address.sun_path,"server_socket");
	len=sizeof(address);
	result=connect(sockfd,(struct sockaddr*) &address,len);
	if(result== -1){
		perror("OOPS: Client cannot connect \n");
		exit(1);
	}
	while(result>-1){
		c= send_file_descriptor(sockfd, a);
			if(c<0)
				err(-1,"cannot send file file descriptor a ");	
		b= send_file_descriptor(sockfd, b);
			if(b<0)
				err(-1,"cannot send file file descriptor  b");	
		result=-1;
	}
	close(a);
	close(b);
	close(sockfd);
	
}


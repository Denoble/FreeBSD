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
int send_fd(int fd, int fd_to_send){
	struct iovec iov[1];
	struct msghdr msg;
	char buf[2]; /* send_fd()/recv_fd() 2-byte protocol */
	iov[0].iov_base = buf;
	iov[0].iov_len = 2;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	if (fd_to_send < 0) {
		msg.msg_control = NULL;
		msg.msg_controllen = 0;
		buf[1] = -fd_to_send; /* nonzero status means error */
		if (buf[1] == 0)
			buf[1] = 1; /* -256, etc. would screw up protocol */
	}
 	else {
		if (cmptr == NULL && (cmptr = malloc(CONTROLLEN)) == NULL)
			return(-1);
		cmptr->cmsg_level = SOL_SOCKET;
		cmptr->cmsg_type = SCM_RIGHTS;
		cmptr->cmsg_len = CONTROLLEN;
		msg.msg_control = cmptr;
		msg.msg_controllen = CONTROLLEN;
		*(int *)CMSG_DATA(cmptr) = fd_to_send; /* the fd to pass */
		buf[1] = 0; /* zero status means OK */
	}
	buf[0] = 0; /* null byte flag to recv_fd() */
	if (sendmsg(fd, &msg, 0) != 2)
		return(-1);
	return(0);

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
		c= send_fd(result, a);
			if(c!=0)
				err(-1,"cannot send file file descriptor a ");	
		b= send_fd(result, b);
			err(-1,"cannot send file file descriptor  b");	
		result==-1;
	}
	close(a);
	close(b);
	close(result);
	
}


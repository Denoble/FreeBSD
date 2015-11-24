#include<fcntl.h>
#BUFFSIZE 8192

int main(int argc, char* argv[]){
	int n,fd;
	char buf[BUFFSIZE];
	char line[MAXLINE];
	/* read file to cat from stdin*/
	while(fgets(line,stdin)!=NULL){
		if(line[strlen(line)-1]=='\n')
		line[strlen(line)-1]=0;/*replace new line with null*/
	}
}

#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/capability.h>
#include<err.h>
int main(){
	cap_enter();
	int a,b,c,d;
	char readWords[128];
	char uppercase[128];
	char* newline="\n";
	a=open("Reading.txt",O_RDONLY);
	if(a<0)
		err(-1,"fail to open file Reading.txt");
	b=open("Written.txt",O_WRONLY|O_CREAT|O_APPEND);
	if(a>0 && b>0){
		while((c=read(a,readWords,sizeof(readWords)))>0){
			for(d=0;d<strlen(readWords)+1;d++){
				uppercase[d]=toupper(readWords[d]);
			}
			write(b,uppercase,strlen(uppercase));
			write(b,newline,strlen(newline));
		}
	}
	close(a);
	close(b);
	return 0;
	
}

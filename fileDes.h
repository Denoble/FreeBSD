#include<stdio.h>// standard function declared here
#include<math.h>
#include"time.h"
#include<stdlib.h>
#define DATAMAX 50
 typedef struct{
	 int read_fileDes;
	 int write_fileDes;
	 char data[DATAMAX];
 }SocketMessage;

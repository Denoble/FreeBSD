
#include<stdlib.h>
#define DATAMAX 50
 typedef struct{
	 int fileDesRd;
	 int fileDesWr;
	 char data[DATAMAX];
	char rdFileName[DATAMAX];
	char wrFileName[DATAMAX];
	char rdRights[DATAMAX];
	char wrRights[DATAMAX];
 }SocketMessage;

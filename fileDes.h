
#include<stdlib.h>
#include<sys/capability.h>
#define DATAMAX 50

 typedef struct{
	 int fileDesRd;
	 int fileDesWr;
	 char data[DATAMAX];
	char rdFileName[DATAMAX];
	char wrFileName[DATAMAX];
	cap_rights_t rdRights;
	cap_rights_t wrRights;
 }SocketMessage;

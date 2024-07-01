
#include "linux_porting.h"

#define configMAC_ADDR0 0
#define configMAC_ADDR1 0
#define configMAC_ADDR2 0
#define configMAC_ADDR3 0
#define configMAC_ADDR4 0 
#define configMAC_ADDR5 0

#define configIP_ADDR0 127
#define configIP_ADDR1 0
#define configIP_ADDR2 0
#define configIP_ADDR3 1

#include "main_clocksync.c"

int main(int argc, char** argv)
{	
	if( argc < 2 )
		vTaskSyncMaster(NULL);
	else if( strcmp(argv[1], "master") == 0)
		vTaskSyncMaster(NULL);
	else if( strcmp(argv[1], "slave") == 0)
		vTaskSyncSlave(NULL);
	else
		return 1;
	return 0;
}

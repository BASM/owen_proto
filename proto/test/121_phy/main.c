#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "owen.h"

int hexdump(char* buff, int size)
{
	printf("Buff size: %i\n", size);
	int i;
	for(i=0; i<=size; i++){
		if(((i%16)==0)){
			printf("\n%4.4x ", i);
		}
		printf("%2.2x ", buff[i]);
	}
  return 0;
}


int main(void)
{
  printf("Hello\n");

  OwenDevice* od = owen_device_open("/dev/ttyUSB0", OWEN_DEV_USBAC4);
  if(od<0){
    perror("Error to open device");
    return 1;
  }
	             //0     1     2     3     4     5     6     7      8    9    10    11     12   13
	char head[]= {0x23, 0x47, 0x4b, 0x48, 0x47, 0x52, 0x4f, 0x54, 0x56, 0x55, 0x4d, 0x50, 0x53, 0x0d};//0x10, 0xf0,0xf0,0x01,0x02,0x00,0x01};
  char result[1000];
  int resize=sizeof(result);

  printf("HEAD: %s\n", head);
  owen_device_phy_transmit(od, head, sizeof(head), result, &resize);

  printf("Result:\n");
	hexdump(result, resize);

  return 0;
}

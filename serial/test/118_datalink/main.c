#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "owen.h"
int hexdump(unsigned char* buff, int size)
{
	int i;
	for(i=0; i<size; i++){
		if(((i%16)==0)){
			printf("\n%4.4x ", i);
		}
		printf("%2.2x ", buff[i]);
	}
  printf("\n");
  return 0;
}

int main(void)
{
  printf("Hello\n");
	             //0     1     2     3     4     5     6     7      8    9    10    11     12   13
	char head[]= {0x23, 0x47, 0x4b, 0x48, 0x47, 0x52, 0x4f, 0x54, 0x56, 0x55, 0x4d, 0x50, 0x53, 0x0d};
  char result[1000];
  int resize=sizeof(result);

  printf("Try parse etanol package..\n");
  OwenDatalink* odl = owen_datalink_new();
  int res = owen_datalink_setpackage(odl, head, sizeof(head));
  if(res==0){
    printf("parsing ok!\n");//: %i\n", res);
  }else{
    printf("parsing error!\n");//: %i\n", res);
    return 1;
  }
  owen_datalink_printpackage(odl);
  printf("Send layer:\n");
	hexdump(odl->buff, odl->size+2);
  int size=sizeof(result);
  printf("Try generate etanol package..\n");
  res = owen_datalink_getpackage(odl, result, &size);
  if(res ==0){
    if(size==sizeof(head)){
      printf("Package size compare!\n");
      if(!memcmp(result, head, size)){
        printf("Body compare!\n");
      }else{
        printf("Body don't compare :-(\n");
        return 1;
      }
    }else{
      printf("Package size don't compare :-(\n");
      return 1;
    }
  }
  
  printf("Test phy level... (send etalon package and print result...) \n");

  //TEST phy level...
  OwenDevice* dev = owen_device_open("/dev/ttyUSB0", OWEN_DEV_USBAC4);
  if(dev<0){
    perror("Error to open device");
    return 1;
  }

  OwenDatalink* od_result = owen_datalink_new();
  owen_device_datalink_transmit(dev, odl, od_result);
  printf("Result:\n");
	hexdump(od_result->buff, od_result->size+2);
  owen_datalink_printpackage(od_result);

  owen_datalink_free(odl);
  owen_datalink_free(od_result);

  return 0;
}

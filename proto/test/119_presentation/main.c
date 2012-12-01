#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>


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

static int print_hexdump(char* tab, unsigned char* buff, int size)
{
	int i;
  char str[17]={'\0'};
	for(i=0; i<size; i++){
		if(((i%16)==0)){
      if(str[0] != '\0')
        printf(" %s", str);
      memset(str, '\0', 17);
			printf("\n%s%4.4x ", tab, i);
		}
    if( (buff[i]>=0x20) && (buff[i]<=0x7e) )
      str[i%16]=buff[i];
    else
      str[i%16]='.';
		printf("%2.2x ", buff[i]);
	}
  printf(" %s", str);
  printf("\n");
  return 0;
}


int main(void)
{
  char result[1000];
  int resize=sizeof(result);

  printf("Hello\n");
  OwenDatalink* od = owen_datalink_new();
  od->addr=4;
  od->eaddr=0;
  od->remote=1;
  od->size=0;
  od->hash=owen_hashstr("PV");

  OwenDevice* dev = owen_device_open("/dev/ttyUSB0", OWEN_DEV_USBAC4);
  if(dev<0){
    perror("Error to open device");
    return 1;
  }

  OwenDatalink* od_result = owen_datalink_new();
  owen_device_datalink_transmit(dev, od, od_result);

  {
    char result[100];
    int size=100;
    int res = owen_datalink_getpackage(od, result, &size);
    print_hexdump("\t", result, size);
  }
  printf("Transmit:\n"); 
  owen_datalink_printpackage(od);
  printf("Result:\n");
  owen_datalink_printpackage(od_result);
  {
    char result[100];
    int size=100;
    int res = owen_datalink_getpackage(od_result, result, &size);
    print_hexdump("\t", result, size);
  }
  printf("CMD: %s\n", owen_strhash(od_result->hash));
  int i;
  switch(od_result->size){
    case 0: break;
    case 1:
      i=owen_isInt(od_result);
      printf("INT: %i (%x)\n", i,i);
      break;
    case 2:
      i=owen_isInt(od_result);
      printf("INT: %i (%x)\n", i,i);
      break;
    case 3:
      i=owen_isInt(od_result);
      printf("INT: %i (%x)\n", i,i);
      printf("FLOAT: %f\n", owen_isFloat(od_result));
      break;
    case 4:
      i=owen_isInt(od_result);
      printf("INT: %i (%x)\n", i,i);
      printf("FLOAT: %f\n", owen_isFloat(od_result));
      break;
  }

  owen_datalink_free(od);
  owen_datalink_free(od_result);

  return 0;
}

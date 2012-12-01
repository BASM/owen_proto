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

char *owen_M_get_listsupportdevices()
{
  return strdup("TMP200\0XXX\0");
}

int owen_issupportdevice(char *device){
  printf("Test device: %s\n", device);

  return 0;
}

int owen_free(char* dev){
  if(dev!=NULL)
    free(dev);
  return 0;
}


int main(void)
{
  printf("Proto APPLICATION LAYER\n");
  /*
  char *devices = owen_M_get_listsupportdevices();

  char *device=devices;
  while(*device!='\0'){
    if(owen_issupportdevice(device)){
      printf();
    }else{
      printf("Error, device %s get from owen_get_listsupportdevices() don't support\n", device);
      return 1;
    }
    device+=sizeof(device)+1;
  };

  owen_free(devices);
   */

  return 0;
}

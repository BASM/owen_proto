#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "owen.h"

OwenDevice* owen_device_open(char* device, int type){
  OwenDevice* od;

  if(type != OWEN_DEV_USBAC4){
    printf("Unknonw type of owen device\n");
    return 0;
  }
  od = calloc(sizeof(OwenDevice), 1);
  od->fd = open(device, O_RDWR);
  if(od->fd <0){
    perror("Can't to open OWEN device");
    return NULL;
  }

	////////////////// CONFIG /////////
	struct termios term;
	tcgetattr(od->fd, &term);
	cfmakeraw(&term);
	cfsetospeed(&term, B9600);

	term.c_cc[VTIME]=1;
	term.c_cc[VMIN]=100;

	tcsetattr(od->fd,TCSAFLUSH, &term);
	///////////////////////////////////
  return od;
}

int owen_device_phy_transmit(OwenDevice* od, char* buff, int buffsize, char* result, int *size)
{
  //FIXME test input params

  int res;
  res = write(od->fd, buff, buffsize);
	*size = read(od->fd, result, *size);
  return 0;
}

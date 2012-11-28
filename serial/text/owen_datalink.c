/*
 * owen_datalink.c  - Interface for OWEN protocol (DATA-LINK LAYER OF ISO MODEL)
 * Creation Date: 2012-11-26
 *
 * Copyright (C) 2012 Leonid Myravjev (asm@asm.pp.ru)
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */


#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "owen.h"

static int print_hexdump(char* tab, unsigned char* buff, int size)
{
	int i;
	for(i=0; i<size; i++){
		if(((i%16)==0)){
			printf("\n%s%4.4x ", tab, i);
		}
		printf("%2.2x ", buff[i]);
	}
  printf("\n");
  return 0;
}
#define MOTOW(a) ({ ((a>>8)|(a<<8))&0xFFFF; })

int owen_datalink_printpackage(OwenDatalink* od)
{
  printf("Package:\n");
  printf("\tAddres  : %x\n", od->addr);
  printf("\t     +  : %x\n", od->eaddr);
  printf("\tRemote  : %x\n", od->remote);
  printf("\tSize    : %x\n", od->size);
  printf("\tHash    : %x (LE: %x)\n", od->hash, MOTOW(od->hash));
  if(od->size>0){
    printf("\tData:");
    print_hexdump("\t\t", od->data, od->size);
  }
  printf("\tCRC     : %x\n", od->crc);

  return 0;
}


OwenDatalink* owen_datalink_new()
{
  OwenDatalink* od=calloc(sizeof(OwenDatalink), 1);
  return od;
}

enum {
  OWEN_DL_OK=0,
  OWEN_DL_BADHEADER=1,
  OWEN_DL_BADATA=2,
  OWEN_DL_BADCRC=3,
  OWEN_DL_BADBUFFSIZE=4,//
  OWEN_DL_ERRTRANSMIT=5,//
};

unsigned int Hash(uint8_t Byte, char nbit, unsigned int CRC)
{
  int i;
  for(i=0; i< nbit; i++, Byte <<=1) {
    if( (Byte^(CRC>>8))&0x80){
      CRC <<= 1; CRC ^= 0x8F57;
    }else
      CRC <<=1;
  }
  return CRC;
}

int owen_datalink_setpackage(OwenDatalink* od, char* buff, int resultsize)
{
  if((buff[0]!=0x23) || (buff[resultsize-1]!=0x0d))
    return OWEN_DL_BADHEADER; 

  unsigned char decode[100];
  int decsize=0;
  {
    //Декодируем
    int i;
    for(i=1;i<resultsize-1;i+=2){
      int a,b;
      a=buff[i  ]-'G';
      b=buff[i+1]-'G';
      if( (a<0) || (a>0x0F) || (b<0) || (b>0x0F)){
        return OWEN_DL_BADATA;
      }
      decode[i/2]= (a<<4) | b;
      decsize++;
    }
  }
  //CRC test
  unsigned int origcrc;
  {
    int i;
    unsigned int hash=0;
    for(i=0;i<decsize-2;i++){
      hash = Hash(decode[i], 8, hash);
    }
    origcrc =decode[decsize-1] | decode[decsize-2]<<8;
    if(origcrc!=(hash&0xffff)){
      return OWEN_DL_BADCRC;
    }
  }
  od->addr=decode[0];//FIXME add 
  od->eaddr=decode[1]>>5;//FIXME add 
  od->remote=(decode[1]>>4)&1;
  od->size=decode[1]&0x0F;
  char* hash=(char*)&od->hash;
  memcpy(&od->hash, &decode[2], 2);
  memcpy(od->data, &decode[4], od->size);
  od->crc=origcrc;
  return 0;
}

int owen_datalink_getpackage(OwenDatalink* od, char* buff, int *resultsize)
{
  char dl[4+2+15];//1(addr)+1(config)+2(Hash)+(блок данных 0..15) + 2 (CRC)
  int maxsize=*resultsize;
  uint8_t* bf=(uint8_t*)od;
  //Конвертим

  int i;
  int size=0;
  buff[size++]='#';
  int buffsize=1+1+2+2+od->size;
  uint16_t hash=0;

  for(i=0; i<buffsize; i++){
    if(i>=maxsize){
      printf("Fuckup!\n");
      return OWEN_DL_BADBUFFSIZE;
    }
    unsigned int a,b;
    if(i>=(buffsize-2)){
      a=hash>>12&0x0F;
      b=hash>>8 &0x0F;
      hash<<=8;
    }else{
      a=bf[i]>>  4 ;
      b=bf[i]& 0x0F;
      hash = Hash(bf[i], 8, hash);
    }
    buff[size++]=a+'G';
    buff[size++]=b+'G';
  }
  buff[size++]=0x0d;
  *resultsize=size;
  return 0;
}

int owen_datalink_free(OwenDatalink* od)
{
  free(od);
  return 0;
}

int owen_device_datalink_transmit(OwenDevice* device, OwenDatalink* out, OwenDatalink* input)
{
  char buff[MAXPHYSIZE];
  char result[MAXPHYSIZE];
  int size = sizeof(buff);
  int resize = sizeof(result);
  int res;

  if(!owen_datalink_getpackage(out, buff, &size)){
    if(!owen_device_phy_transmit(device, buff, size, result, &resize)){
      if(res=owen_datalink_setpackage(input, result, resize)){
        printf("Error parce result :-(\n");
        return res;
      }
    }else{
      printf("Error transmit :-(\n");
      return  OWEN_DL_ERRTRANSMIT;
    }

  }else{
    printf("Fuckup!\n");
    return -1;
  }

  return 0;
}

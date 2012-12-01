/*
 * owen_present.c  - Interface for OWEN protocol (PRESENTATION LAYER OF ISO MODEL)
 * Creation Date: 2012-12-01
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



static signed char owen_charconvert(char sign, char next)
{
  char res;
  char ch;
  if(sign=='.'){
              printf("DOT\n");
    return -1;

  }
  ch=toupper(sign);
  if( (ch>='0') && (ch<='9'))
    res=ch-'0';
  else if( (ch>='A') && (ch<='Z') )
    res=ch-'A'+ 10;
  else switch(sign){
    case '-': res=10+26+0; break;
    case '_': res=10+26+1; break;
    case '/': res=10+26+2; break;
    case ' ': res=10+26+3; break;
    default:
              printf("NOF\n");
              return -1;
  }

  res*=2;
  if(next=='.') res++;
  return res;
}

uint16_t owen_hashstr(char* parm)
{
  char i;
  uint16_t hash=0;
  int signum=0;
  do{
    signed char sig = owen_charconvert(*parm, *(parm+1));
    if(sig==-1) continue;
    hash = owen_hash(sig<<1, 7, hash);
    signum++;
  }while(*++parm != '\0');
  while(signum++<4){
    hash = owen_hash(78<<1, 7, hash);//забить пробелами
  }
  return ((hash<<8) | (hash >> 8))&0xffff;//FIXME работает тлько на little endian
}

char* owen_strhash(int hash){
  char* res=NULL;
  uint16_t rev=MOTOW(hash);
  switch(rev){
    case 0xb8df: return "PV";

    // NETWORK
    case 0x81d6: return "dev";
    case 0x2d5b: return "ver";
    case 0xb760: return "bPS";
    case 0x523f: return "Len";
    case 0xE8C4: return "PrtY";
    case 0xB72E: return "Sbit";
    case 0x1ED2: return "A.Len";
    case 0x9F62: return "Addr";
    case 0x0233: return "n.Err";
    case 0x1E25: return "rSdL";//BAG in documentation (rS.dL)
    case 0xcbf5: return "rS.dL";//Don't work, use rSdL!

    // ERROR

    default:     return "";
  }
  return res;
}


float owen_isFloat(OwenDatalink* od){
  char* src= od->data;
  union{
    unsigned char s[4];
    float f;
  } R;
  //FIXME only for little endian
  if(od->size==4)
    R.s[0]=od->data[3];
  else
    R.s[0]=0;
  R.s[1]=od->data[2];
  R.s[2]=od->data[1];
  R.s[3]=od->data[0];
  return R.f;
}

int owen_isInt(OwenDatalink* od){
  union{
    unsigned char s[4];
    uint32_t i;
  } R;
  R.i=0;
  int x;
  int m=od->size;
  for(x=0;od->size--;x++){
    R.s[x]=od->data[od->size];
  }

  return R.i;
}

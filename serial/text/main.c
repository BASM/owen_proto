//Файл для тестирования библиотеки

#include <stdio.h>
#include <seri.h>

typedef struct{
  int type;
} seri_device;

#define DEV_TMP200 1

#define PROT_MODBUS 1

seri_device seri_create_device(int type){
  seri_device ret={
    .type=type
  };

  return ret;
}

int send_data(const unsigned char *data, int len){
  int i;
  
  printf("Send %i bytes:", len);
  for(i=0;len--;i++){
    if((i%8)==0){
      printf(" ");
      if((i%16)==0){
        printf("\n");
        printf("%4.4x  ", i);
      }
    }
    printf("%2.2x ", data[i]);

  }
  printf("\n");
  return 0;
}

int main(void){
  seri_init();

  char buf[100];
  send_data(buf, 35);
  //seri_device dev;
  //seri_proto  proto;

  //seri_open_port(PORT_SELF, send_data, recive_data);
  /*
  proto = seri_proto_create(PROT_MODBUS);
  seri_proto_port(NULL)

  dev = seri_device_create(DEV_TMP200);
  seri_device_connect_port(&dev, &proto);
   */
  seri_quit();
  return 0;
}
